#include <ngspice/sharedspice.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

constexpr double PI = 3.14159265358979323846;

struct Signal
{
    std::vector<double> time;
    std::vector<double> value;
};

struct SignalInfo
{
    double minimum;
    double maximum;
    double mean;
    double pp;
    double rms;
};

// ------------------------------------------------------------
// Callbacks libngspice
// ------------------------------------------------------------

int send_char(char*, int, void*)
{
    return 0;
}

int controlled_exit(int status, bool, bool, int, void*)
{
    if(status != 0)
        std::cerr << "ngspice terminou com status " << status
                  << '\n';

    return 0;
}

// ------------------------------------------------------------
// Comunicação com ngspice
// ------------------------------------------------------------

void ng_command(const std::string& command)
{
    std::vector<char> buffer(command.begin(), command.end());

    buffer.push_back('\0');

    if(ngSpice_Command(buffer.data()) != 0)
        throw std::runtime_error("Erro executando: " + command);
}

std::vector<double> get_vector(const std::string& name)
{
    std::vector<char> buffer(name.begin(), name.end());

    buffer.push_back('\0');

    pvector_info info = ngGet_Vec_Info(buffer.data());

    if(!info)
        throw std::runtime_error("Vetor nao encontrado: " + name);

    if(!info->v_realdata)
        throw std::runtime_error("Vetor nao possui dados reais: " +
                                 name);

    return {info->v_realdata, info->v_realdata + info->v_length};
}

// ------------------------------------------------------------
// Operações vetoriais
// ------------------------------------------------------------

std::vector<double> subtract(std::span<const double> a,
                             std::span<const double> b)
{
    if(a.size() != b.size())
        throw std::runtime_error("Vetores com tamanhos diferentes");

    std::vector<double> result(a.size());

    for(std::size_t i = 0; i < a.size(); ++i)
        result[i] = a[i] - b[i];

    return result;
}

std::vector<double> negate(std::span<const double> x)
{
    std::vector<double> result(x.size());

    for(std::size_t i = 0; i < x.size(); ++i)
        result[i] = -x[i];

    return result;
}

// ------------------------------------------------------------
// Interpolação linear
// ------------------------------------------------------------

double interpolate(std::span<const double> time,
                   std::span<const double> value,
                   double                  t)
{
    if(time.size() != value.size())
        throw std::runtime_error("time/value incompatíveis");

    if(t < time.front() || t > time.back())
        throw std::runtime_error("Interpolacao fora do intervalo");

    auto it = std::lower_bound(time.begin(), time.end(), t);

    if(it == time.begin())
        return value.front();

    if(it == time.end())
        return value.back();

    const std::size_t i =
        static_cast<std::size_t>(std::distance(time.begin(), it));

    if(time[i] == t)
        return value[i];

    const double t0 = time[i - 1];
    const double t1 = time[i];

    const double x0 = value[i - 1];
    const double x1 = value[i];

    const double alpha = (t - t0) / (t1 - t0);

    return x0 + alpha * (x1 - x0);
}

// ------------------------------------------------------------
// Recorta exatamente [t0,t1]
//
// As fronteiras são interpoladas.
// ------------------------------------------------------------

Signal crop(std::span<const double> time,
            std::span<const double> value,
            double                  t0,
            double                  t1)
{
    Signal result;

    result.time.push_back(t0);
    result.value.push_back(interpolate(time, value, t0));

    for(std::size_t i = 0; i < time.size(); ++i)
    {
        if(time[i] > t0 && time[i] < t1)
        {
            result.time.push_back(time[i]);
            result.value.push_back(value[i]);
        }
    }

    result.time.push_back(t1);
    result.value.push_back(interpolate(time, value, t1));

    return result;
}

// ------------------------------------------------------------
// Cruzamentos ascendentes por zero
//
// Interpolação linear entre as duas amostras.
// ------------------------------------------------------------

std::vector<double>
rising_zero_crossings(std::span<const double> time,
                      std::span<const double> signal)
{
    std::vector<double> crossings;

    for(std::size_t i = 1; i < signal.size(); ++i)
    {
        const double y0 = signal[i - 1];
        const double y1 = signal[i];

        if(y0 <= 0.0 && y1 > 0.0)
        {
            const double t0 = time[i - 1];
            const double t1 = time[i];

            const double tc = t0 + (-y0) * (t1 - t0) / (y1 - y0);

            crossings.push_back(tc);
        }
    }

    return crossings;
}

// ------------------------------------------------------------
// Fase de signal em relação a reference
// ------------------------------------------------------------

double phase_degrees(std::span<const double> time,
                     std::span<const double> reference,
                     std::span<const double> signal,
                     double                  reference_crossing,
                     double                  period)
{
    const auto crossings = rising_zero_crossings(time, signal);

    if(crossings.empty())
        throw std::runtime_error("Nao foi possível medir fase");

    auto best = std::min_element(
        crossings.begin(),
        crossings.end(),
        [&](double a, double b)
        {
            return std::abs(a - reference_crossing) <
                   std::abs(b - reference_crossing);
        });

    double dt = *best - reference_crossing;

    // Normaliza para +/- T/2
    while(dt > period / 2.0)
        dt -= period;

    while(dt < -period / 2.0)
        dt += period;

    return -360.0 * dt / period;
}

// ------------------------------------------------------------
// Estatísticas temporais
//
// IMPORTANTE:
//
// Como os pontos do transiente podem não ser uniformes,
// fazemos integração no tempo.
//
// Para x² usamos a integral EXATA considerando interpolação
// linear de x entre duas amostras:
//
// integral(x² dt)
// = dt/3 (x0² + x0*x1 + x1²)
// ------------------------------------------------------------

SignalInfo analyze(std::span<const double> time,
                   std::span<const double> value)
{
    if(time.size() != value.size())
        throw std::runtime_error("time/value incompatíveis");

    if(time.size() < 2)
        throw std::runtime_error("Poucas amostras");

    const auto [min_it, max_it] =
        std::minmax_element(value.begin(), value.end());

    double integral         = 0.0;
    double integral_squared = 0.0;

    for(std::size_t i = 1; i < value.size(); ++i)
    {
        const double dt = time[i] - time[i - 1];

        const double x0 = value[i - 1];
        const double x1 = value[i];

        // integral de x(t)
        integral += 0.5 * (x0 + x1) * dt;

        // integral de x²(t)
        integral_squared +=
            dt / 3.0 * (x0 * x0 + x0 * x1 + x1 * x1);
    }

    const double duration = time.back() - time.front();

    return {.minimum = *min_it,
            .maximum = *max_it,

            .mean = integral / duration,

            .pp = *max_it - *min_it,

            .rms = std::sqrt(integral_squared / duration)};
}

// ------------------------------------------------------------
// Potência média
//
// v(t) e i(t) são consideradas lineares dentro de cada
// intervalo.
//
// A integral do produto é calculada exatamente:
//
// dt/6 *
// (2*v0*i0 + v0*i1 + v1*i0 + 2*v1*i1)
// ------------------------------------------------------------

double average_power(std::span<const double> time,
                     std::span<const double> voltage,
                     std::span<const double> current)
{
    if(time.size() != voltage.size() ||
       time.size() != current.size())
        throw std::runtime_error("Vetores incompatíveis");

    double integral = 0.0;

    for(std::size_t i = 1; i < time.size(); ++i)
    {
        const double dt = time[i] - time[i - 1];

        const double v0 = voltage[i - 1];
        const double v1 = voltage[i];

        const double i0 = current[i - 1];
        const double i1 = current[i];

        integral +=
            dt / 6.0 *
            (2.0 * v0 * i0 + v0 * i1 + v1 * i0 + 2.0 * v1 * i1);
    }

    return integral / (time.back() - time.front());
}

// ------------------------------------------------------------

void print_signal(const std::string& name,
                  const SignalInfo&  x,
                  const std::string& unit)
{
    std::cout << "\n"
              << name << '\n'
              << "----------------------------------------\n"
              << "Minimo       : " << x.minimum << ' ' << unit
              << '\n'
              << "Maximo       : " << x.maximum << ' ' << unit
              << '\n'
              << "Pico-a-pico  : " << x.pp << ' ' << unit << '\n'
              << "Media        : " << x.mean << ' ' << unit << '\n'
              << "RMS          : " << x.rms << ' ' << unit << '\n';
}

// ------------------------------------------------------------

int main()
{
    try
    {
        if(ngSpice_Init(send_char,
                        nullptr,
                        controlled_exit,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr) != 0)
        {
            throw std::runtime_error(
                "Falha inicializando libngspice");
        }

        // ----------------------------------------------------
        // Carrega e executa circuito
        // ----------------------------------------------------

        ng_command("source questao07.sp");
        ng_command("run");

        // ----------------------------------------------------
        // Dados do ngspice
        // ----------------------------------------------------

        const auto time = get_vector("time");

        const auto v1 = get_vector("1");

        const auto v2 = get_vector("2");

        const auto v3 = get_vector("3");

        const auto vs_branch = get_vector("vs#branch");

        // Corrente do circuito:
        //
        // vs#branch usa a convenção da corrente entrando
        // no terminal positivo da fonte.
        //
        // Queremos corrente saindo da fonte em direção à carga.
        const auto current = negate(vs_branch);

        // ----------------------------------------------------
        // Tensões dos elementos
        // ----------------------------------------------------

        // Bobina real = L1 + R1
        const auto v_bobina = subtract(v1, v3);

        // Indutor
        const auto v_l1 = subtract(v1, v2);

        // Resistência interna da bobina
        const auto v_r1 = subtract(v2, v3);

        // R2
        const auto& v_r2 = v3;

        // ----------------------------------------------------
        // Detectar período usando a fonte
        // ----------------------------------------------------

        const auto source_crossings =
            rising_zero_crossings(time, v1);

        if(source_crossings.size() < 2)
            throw std::runtime_error(
                "Nao foi possivel detectar um periodo");

        const double t_begin =
            source_crossings[source_crossings.size() - 2];

        const double t_end = source_crossings.back();

        const double period = t_end - t_begin;

        const double frequency = 1.0 / period;

        // ----------------------------------------------------
        // Recortar EXATAMENTE o último período
        // ----------------------------------------------------

        const Signal vs = crop(time, v1, t_begin, t_end);

        const Signal vb = crop(time, v_bobina, t_begin, t_end);

        const Signal vl = crop(time, v_l1, t_begin, t_end);

        const Signal vr1 = crop(time, v_r1, t_begin, t_end);

        const Signal vr2 = crop(time, v_r2, t_begin, t_end);

        const Signal is = crop(time, current, t_begin, t_end);

        // ----------------------------------------------------
        // Estatísticas
        // ----------------------------------------------------

        const auto vs_info = analyze(vs.time, vs.value);

        const auto vb_info = analyze(vb.time, vb.value);

        const auto vl_info = analyze(vl.time, vl.value);

        const auto vr1_info = analyze(vr1.time, vr1.value);

        const auto vr2_info = analyze(vr2.time, vr2.value);

        const auto i_info = analyze(is.time, is.value);

        // ----------------------------------------------------
        // Fases
        // ----------------------------------------------------

        const double phi_bobina =
            phase_degrees(time, v1, v_bobina, t_begin, period);

        const double phi_r2 =
            phase_degrees(time, v1, v_r2, t_begin, period);

        const double phi_current =
            phase_degrees(time, v1, current, t_begin, period);

        // ----------------------------------------------------
        // Potências
        // ----------------------------------------------------

        // Fonte: corrente definida saindo do terminal positivo,
        // logo resultado positivo = potência fornecida.
        const double p_source =
            average_power(vs.time, vs.value, is.value);

        const double p_r1 =
            average_power(vr1.time, vr1.value, is.value);

        const double p_l1 =
            average_power(vl.time, vl.value, is.value);

        const double p_r2 =
            average_power(vr2.time, vr2.value, is.value);

        const double p_load = p_r1 + p_l1 + p_r2;

        // ----------------------------------------------------
        // P, Q, S e FP do circuito completo
        // ----------------------------------------------------

        const double apparent_power = vs_info.rms * i_info.rms;

        const double power_factor = p_source / apparent_power;

        // phi = fase da tensão - fase da corrente
        const double phi_load_deg = -phi_current;

        const double phi_load_rad = phi_load_deg * PI / 180.0;

        const double reactive_power =
            apparent_power * std::sin(phi_load_rad);

        // ----------------------------------------------------
        // Saída
        // ----------------------------------------------------

        std::cout << std::fixed << std::setprecision(6);

        std::cout << "\nANALISE DO CIRCUITO R-L-R\n"
                  << "========================================\n";

        std::cout << "\nJanela analisada\n"
                  << "----------------------------------------\n"
                  << "Inicio       : " << t_begin * 1e3 << " ms\n"

                  << "Fim          : " << t_end * 1e3 << " ms\n"

                  << "Periodo      : " << period * 1e3 << " ms\n"

                  << "Frequencia   : " << frequency << " Hz\n";

        print_signal("Fonte Vs", vs_info, "V");

        print_signal("Bobina real (L1 + R1)", vb_info, "V");

        print_signal("Indutor L1", vl_info, "V");

        print_signal("Resistencia interna R1", vr1_info, "V");

        print_signal("Resistor R2", vr2_info, "V");

        print_signal("Corrente", i_info, "A");

        std::cout << "\nFases em relacao a Vs\n"
                  << "----------------------------------------\n"
                  << "Vs            : 0.000000 deg\n"
                  << "Vbobina       : " << phi_bobina << " deg\n"
                  << "V_R2          : " << phi_r2 << " deg\n"
                  << "Corrente      : " << phi_current << " deg\n";

        std::cout << "\nPotencias do circuito\n"
                  << "----------------------------------------\n"

                  << "P             : " << p_source << " W\n"

                  << "Q             : " << reactive_power
                  << " var\n"

                  << "S             : " << apparent_power << " VA\n"

                  << "Fator potencia: " << power_factor << '\n'

                  << "Angulo phi    : " << phi_load_deg << " deg\n";

        std::cout << "\nPotencia ativa por componente\n"
                  << "----------------------------------------\n"

                  << "R1            : " << p_r1 << " W\n"

                  << "L1            : " << p_l1 << " W\n"

                  << "R2            : " << p_r2 << " W\n"

                  << "R1 + L1 + R2  : " << p_load << " W\n"

                  << "Fonte forneceu: " << p_source << " W\n"

                  << "Erro balanco  : " << p_source - p_load
                  << " W\n";

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Erro: " << e.what() << '\n';

        return 1;
    }
}
