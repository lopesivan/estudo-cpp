#include <ngspice/sharedspice.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

// ------------------------------------------------------------
// Callbacks ngspice
// ------------------------------------------------------------

int send_char(char* msg, int, void*)
{
    if(msg)
        std::cout << "[ngspice] " << msg << '\n';

    return 0;
}

int controlled_exit(int status, bool, bool, int, void*)
{
    std::cout << "[ngspice] terminou com status " << status << '\n';

    return 0;
}

// ------------------------------------------------------------
// Executa comando no ngspice
// ------------------------------------------------------------

void ng_command(const std::string& command)
{
    std::vector<char> buffer(command.begin(), command.end());

    buffer.push_back('\0');

    const int status = ngSpice_Command(buffer.data());

    if(status != 0)
    {
        throw std::runtime_error(
            "Erro executando comando ngspice: " + command);
    }
}

// ------------------------------------------------------------
// Recupera vetor interno do ngspice
// ------------------------------------------------------------

std::vector<double> get_vector(const std::string& name)
{
    std::vector<char> buffer(name.begin(), name.end());

    buffer.push_back('\0');

    pvector_info info = ngGet_Vec_Info(buffer.data());

    if(!info)
    {
        throw std::runtime_error("Vetor nao encontrado: " + name);
    }

    if(!info->v_realdata)
    {
        throw std::runtime_error("Vetor nao possui dados reais: " +
                                 name);
    }

    return {info->v_realdata, info->v_realdata + info->v_length};
}

// ------------------------------------------------------------
// Informações estatísticas
// ------------------------------------------------------------

struct SignalInfo
{
    double min;
    double max;
    double mean;
    double amplitude;
    double rms;
};

// ------------------------------------------------------------
// Analisa sinal usando integração temporal
// ------------------------------------------------------------

SignalInfo analyze(std::span<const double> time,
                   std::span<const double> signal)
{
    if(time.size() != signal.size())
    {
        throw std::runtime_error(
            "time e signal possuem tamanhos diferentes");
    }

    if(signal.size() < 2)
    {
        throw std::runtime_error("Numero insuficiente de amostras");
    }

    const auto [min_it, max_it] =
        std::ranges::minmax_element(signal);

    double integral    = 0.0;
    double integral_sq = 0.0;

    for(std::size_t i = 1; i < signal.size(); ++i)
    {
        const double dt = time[i] - time[i - 1];

        // Integral de x(t)
        integral += 0.5 * (signal[i - 1] + signal[i]) * dt;

        // Integral de x²(t)
        integral_sq += 0.5 *
                       (signal[i - 1] * signal[i - 1] +
                        signal[i] * signal[i]) *
                       dt;
    }

    const double duration = time.back() - time.front();

    const double mean = integral / duration;

    const double rms = std::sqrt(integral_sq / duration);

    return {.min       = *min_it,
            .max       = *max_it,
            .mean      = mean,
            .amplitude = (*max_it - *min_it) / 2.0,
            .rms       = rms};
}

// ------------------------------------------------------------
// Potência média
//
// P = (1/T) integral v(t)i(t) dt
// ------------------------------------------------------------

double average_power(std::span<const double> time,
                     std::span<const double> voltage,
                     std::span<const double> current)
{
    if(time.size() != voltage.size() ||
       time.size() != current.size())
    {
        throw std::runtime_error("Vetores com tamanhos diferentes");
    }

    if(time.size() < 2)
    {
        throw std::runtime_error("Numero insuficiente de amostras");
    }

    double integral = 0.0;

    for(std::size_t i = 1; i < time.size(); ++i)
    {
        const double dt = time[i] - time[i - 1];

        const double p0 = voltage[i - 1] * current[i - 1];

        const double p1 = voltage[i] * current[i];

        integral += 0.5 * (p0 + p1) * dt;
    }

    const double duration = time.back() - time.front();

    return integral / duration;
}

// ------------------------------------------------------------
// Impressão de grandezas elétricas
// ------------------------------------------------------------

void print_signal(const std::string& name,
                  const SignalInfo&  signal,
                  const std::string& unit,
                  double             scale = 1.0)
{
    constexpr int label_width = 10;
    constexpr int value_width = 12;

    std::cout << '\n'
              << name << '\n'
              << "--------------------------------\n";

    auto print = [&](const std::string& label, double value)
    {
        std::cout << "  " << std::left << std::setw(label_width)
                  << label

                  << ": "

                  << std::right << std::setw(value_width)
                  << value * scale

                  << ' ' << unit << '\n';
    };

    print("Minimo", signal.min);
    print("Maximo", signal.max);
    print("Media", signal.mean);
    print("Amplitude", signal.amplitude);
    print("RMS", signal.rms);
}

// ------------------------------------------------------------
// main
// ------------------------------------------------------------

int main()
{
    try
    {
        // ----------------------------------------------------
        // Inicializa libngspice
        // ----------------------------------------------------

        const int init_status = ngSpice_Init(send_char,
                                             nullptr,
                                             controlled_exit,
                                             nullptr,
                                             nullptr,
                                             nullptr,
                                             nullptr);

        if(init_status != 0)
        {
            throw std::runtime_error(
                "Falha ao inicializar libngspice");
        }

        // ----------------------------------------------------
        // Carrega circuito e executa simulação
        // ----------------------------------------------------

        ng_command("source RC.sp");
        ng_command("run");

        // ----------------------------------------------------
        // Recupera vetores do ngspice
        // ----------------------------------------------------

        const auto time = get_vector("time");

        const auto vin = get_vector("in");

        const auto vout = get_vector("out");

        const auto v1_branch = get_vector("v1#branch");

        // ----------------------------------------------------
        // Validação
        // ----------------------------------------------------

        if(time.size() != vin.size() ||
           time.size() != vout.size() ||
           time.size() != v1_branch.size())
        {
            throw std::runtime_error(
                "Vetores com tamanhos diferentes");
        }

        // ----------------------------------------------------
        // Corrente do circuito
        //
        // v1#branch usa a convenção interna do ngspice.
        // Invertemos o sinal para definir:
        //
        // fonte -> R1 -> C1 -> terra
        // ----------------------------------------------------

        std::vector<double> current(v1_branch.size());

        std::ranges::transform(v1_branch,
                               current.begin(),
                               [](double value) { return -value; });

        // ----------------------------------------------------
        // Tensão sobre o resistor
        //
        // Vr = Vin - Vout
        // ----------------------------------------------------

        std::vector<double> vr(vin.size());

        std::ranges::transform(
            vin, vout, vr.begin(), std::minus<>{});

        // ----------------------------------------------------
        // Análises
        // ----------------------------------------------------

        const auto vin_info = analyze(time, vin);

        const auto vout_info = analyze(time, vout);

        const auto vr_info = analyze(time, vr);

        const auto current_info = analyze(time, current);

        // ----------------------------------------------------
        // Potências médias
        // ----------------------------------------------------

        // Resistor:
        //
        // corrente entra no terminal positivo de Vr
        //
        const double p_resistor = average_power(time, vr, current);

        // Capacitor:
        //
        // Vout é a tensão superior do capacitor.
        // A corrente entra nesse terminal.
        //
        const double p_capacitor =
            average_power(time, vout, current);

        // Fonte:
        //
        // current foi definida saindo do terminal
        // positivo da fonte.
        //
        // Portanto este valor representa potência
        // fornecida pela fonte.
        //
        const double p_source = average_power(time, vin, current);

        // ----------------------------------------------------
        // Ganho
        // ----------------------------------------------------

        const double gain =
            vout_info.amplitude / vin_info.amplitude;

        // ----------------------------------------------------
        // Saída
        // ----------------------------------------------------

        std::cout << std::fixed << std::setprecision(6);

        std::cout << "\nNumero de amostras: " << time.size()
                  << '\n';

        std::cout << "Tempo inicial     : " << time.front() * 1e3
                  << " ms\n";

        std::cout << "Tempo final       : " << time.back() * 1e3
                  << " ms\n";

        // ----------------------------------------------------
        // Tensões
        // ----------------------------------------------------

        print_signal("v(in)", vin_info, "V");

        print_signal("v(out)", vout_info, "V");

        print_signal("v(R1)", vr_info, "V");

        // ----------------------------------------------------
        // Corrente
        // ----------------------------------------------------

        print_signal("Corrente", current_info, "mA", 1e3);

        // ----------------------------------------------------
        // Ganho
        // ----------------------------------------------------

        std::cout << "\nRelacao de amplitudes\n"
                  << "--------------------------------\n"
                  << "  Aout/Ain  : " << std::setw(12) << gain
                  << '\n';

        // ----------------------------------------------------
        // Potência média
        // ----------------------------------------------------

        std::cout << "\nPotencia media\n"
                  << "--------------------------------\n"

                  << "  Fonte      : " << std::setw(12)
                  << p_source * 1e3 << " mW\n"

                  << "  Resistor   : " << std::setw(12)
                  << p_resistor * 1e3 << " mW\n"

                  << "  Capacitor  : " << std::setw(12)
                  << p_capacitor * 1e3 << " mW\n"

                  << "  R + C      : " << std::setw(12)
                  << (p_resistor + p_capacitor) * 1e3 << " mW\n";

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Erro: " << e.what() << '\n';

        return 1;
    }
}
