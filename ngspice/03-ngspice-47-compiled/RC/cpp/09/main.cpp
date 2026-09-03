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

struct PowerInfo
{
    double voltage_rms;
    double current_rms;

    double phase_voltage;
    double phase_current;
    double phase;

    double active;        // P [W]
    double reactive;      // Q [var]
    double apparent;      // S [VA]

    double power_factor;
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
    {
        std::cerr
            << "ngspice terminou com status "
            << status
            << '\n';
    }

    return 0;
}

// ------------------------------------------------------------
// Comandos ngspice
// ------------------------------------------------------------

void ng_command(const std::string& command)
{
    std::vector<char> buffer(
        command.begin(),
        command.end()
    );

    buffer.push_back('\0');

    if(ngSpice_Command(buffer.data()) != 0)
    {
        throw std::runtime_error(
            "Erro executando comando ngspice: "
            + command
        );
    }
}

std::vector<double> get_vector(
    const std::string& name)
{
    std::vector<char> buffer(
        name.begin(),
        name.end()
    );

    buffer.push_back('\0');

    pvector_info info =
        ngGet_Vec_Info(buffer.data());

    if(!info)
    {
        throw std::runtime_error(
            "Vetor nao encontrado: "
            + name
        );
    }

    if(!info->v_realdata)
    {
        throw std::runtime_error(
            "Vetor nao possui dados reais: "
            + name
        );
    }

    return {
        info->v_realdata,
        info->v_realdata + info->v_length
    };
}

// ------------------------------------------------------------
// Operações vetoriais
// ------------------------------------------------------------

std::vector<double> subtract(
    std::span<const double> a,
    std::span<const double> b)
{
    if(a.size() != b.size())
    {
        throw std::runtime_error(
            "Vetores com tamanhos diferentes"
        );
    }

    std::vector<double> result(a.size());

    for(std::size_t i = 0; i < a.size(); ++i)
        result[i] = a[i] - b[i];

    return result;
}

std::vector<double> negate(
    std::span<const double> x)
{
    std::vector<double> result(x.size());

    for(std::size_t i = 0; i < x.size(); ++i)
        result[i] = -x[i];

    return result;
}

// ------------------------------------------------------------
// Interpolação linear
// ------------------------------------------------------------

double interpolate(
    std::span<const double> time,
    std::span<const double> value,
    double t)
{
    if(time.size() != value.size())
    {
        throw std::runtime_error(
            "time/value incompatíveis"
        );
    }

    if(t < time.front() || t > time.back())
    {
        throw std::runtime_error(
            "Interpolacao fora do intervalo"
        );
    }

    auto it =
        std::lower_bound(
            time.begin(),
            time.end(),
            t
        );

    if(it == time.begin())
        return value.front();

    if(it == time.end())
        return value.back();

    const std::size_t i =
        static_cast<std::size_t>(
            std::distance(
                time.begin(),
                it
            )
        );

    if(time[i] == t)
        return value[i];

    const double t0 =
        time[i - 1];

    const double t1 =
        time[i];

    const double x0 =
        value[i - 1];

    const double x1 =
        value[i];

    const double alpha =
        (t - t0) /
        (t1 - t0);

    return x0 +
           alpha * (x1 - x0);
}

// ------------------------------------------------------------
// Recorta intervalo [t0, t1]
// ------------------------------------------------------------

Signal crop(
    std::span<const double> time,
    std::span<const double> value,
    double t0,
    double t1)
{
    Signal result;

    result.time.push_back(t0);

    result.value.push_back(
        interpolate(
            time,
            value,
            t0
        )
    );

    for(std::size_t i = 0;
        i < time.size();
        ++i)
    {
        if(time[i] > t0 &&
           time[i] < t1)
        {
            result.time.push_back(
                time[i]
            );

            result.value.push_back(
                value[i]
            );
        }
    }

    result.time.push_back(t1);

    result.value.push_back(
        interpolate(
            time,
            value,
            t1
        )
    );

    return result;
}

// ------------------------------------------------------------
// Cruzamentos ascendentes por zero
// ------------------------------------------------------------

std::vector<double> rising_zero_crossings(
    std::span<const double> time,
    std::span<const double> signal)
{
    if(time.size() != signal.size())
    {
        throw std::runtime_error(
            "time/signal incompatíveis"
        );
    }

    std::vector<double> crossings;

    for(std::size_t i = 1;
        i < signal.size();
        ++i)
    {
        const double y0 =
            signal[i - 1];

        const double y1 =
            signal[i];

        if(y0 <= 0.0 &&
           y1 > 0.0)
        {
            const double t0 =
                time[i - 1];

            const double t1 =
                time[i];

            const double crossing =
                t0 +
                (-y0) *
                (t1 - t0) /
                (y1 - y0);

            crossings.push_back(
                crossing
            );
        }
    }

    return crossings;
}

// ------------------------------------------------------------
// Normalização angular
// ------------------------------------------------------------

double normalize_phase(double phase)
{
    while(phase > 180.0)
        phase -= 360.0;

    while(phase <= -180.0)
        phase += 360.0;

    return phase;
}

// ------------------------------------------------------------
// Fase de signal em relação a reference
// ------------------------------------------------------------

double phase_degrees(
    std::span<const double> time,
    std::span<const double> signal,
    double reference_crossing,
    double period)
{
    const auto crossings =
        rising_zero_crossings(
            time,
            signal
        );

    if(crossings.empty())
    {
        throw std::runtime_error(
            "Nao foi possivel medir fase"
        );
    }

    auto best =
        std::min_element(
            crossings.begin(),
            crossings.end(),

            [&](double a, double b)
            {
                return
                    std::abs(
                        a -
                        reference_crossing
                    )
                    <
                    std::abs(
                        b -
                        reference_crossing
                    );
            }
        );

    double dt =
        *best -
        reference_crossing;

    while(dt > period / 2.0)
        dt -= period;

    while(dt < -period / 2.0)
        dt += period;

    return
        -360.0 *
        dt /
        period;
}

// ------------------------------------------------------------
// Estatísticas de sinal
// ------------------------------------------------------------

SignalInfo analyze(
    std::span<const double> time,
    std::span<const double> value)
{
    if(time.size() != value.size())
    {
        throw std::runtime_error(
            "time/value incompatíveis"
        );
    }

    if(time.size() < 2)
    {
        throw std::runtime_error(
            "Poucas amostras"
        );
    }

    const auto [min_it, max_it] =
        std::minmax_element(
            value.begin(),
            value.end()
        );

    double integral =
        0.0;

    double integral_squared =
        0.0;

    for(std::size_t i = 1;
        i < value.size();
        ++i)
    {
        const double dt =
            time[i] -
            time[i - 1];

        const double x0 =
            value[i - 1];

        const double x1 =
            value[i];

        // Integral exata para interpolação linear
        integral +=
            0.5 *
            (x0 + x1) *
            dt;

        // Integral de x²(t) supondo x linear
        integral_squared +=
            dt / 3.0 *
            (
                x0 * x0 +
                x0 * x1 +
                x1 * x1
            );
    }

    const double duration =
        time.back() -
        time.front();

    return {
        .minimum =
            *min_it,

        .maximum =
            *max_it,

        .mean =
            integral /
            duration,

        .pp =
            *max_it -
            *min_it,

        .rms =
            std::sqrt(
                integral_squared /
                duration
            )
    };
}

// ------------------------------------------------------------
// Potência média
// ------------------------------------------------------------

double average_power(
    std::span<const double> time,
    std::span<const double> voltage,
    std::span<const double> current)
{
    if(time.size() != voltage.size() ||
       time.size() != current.size())
    {
        throw std::runtime_error(
            "Vetores incompatíveis"
        );
    }

    if(time.size() < 2)
    {
        throw std::runtime_error(
            "Poucas amostras"
        );
    }

    double integral =
        0.0;

    for(std::size_t i = 1;
        i < time.size();
        ++i)
    {
        const double dt =
            time[i] -
            time[i - 1];

        const double v0 =
            voltage[i - 1];

        const double v1 =
            voltage[i];

        const double i0 =
            current[i - 1];

        const double i1 =
            current[i];

        // Integral exata do produto de duas funções lineares
        integral +=
            dt / 6.0 *
            (
                2.0 * v0 * i0 +
                      v0 * i1 +
                      v1 * i0 +
                2.0 * v1 * i1
            );
    }

    return
        integral /
        (
            time.back() -
            time.front()
        );
}

// ------------------------------------------------------------
// Análise completa de potência
// ------------------------------------------------------------

PowerInfo analyze_power(
    std::span<const double> time,
    std::span<const double> voltage,
    std::span<const double> current,
    double phase_voltage,
    double phase_current)
{
    const SignalInfo voltage_info =
        analyze(
            time,
            voltage
        );

    const SignalInfo current_info =
        analyze(
            time,
            current
        );

    const double phase =
        normalize_phase(
            phase_voltage -
            phase_current
        );

    const double apparent =
        voltage_info.rms *
        current_info.rms;

    const double active =
        average_power(
            time,
            voltage,
            current
        );

    const double reactive =
        apparent *
        std::sin(
            phase *
            PI /
            180.0
        );

    const double power_factor =
        apparent > 0.0
            ? active / apparent
            : 0.0;

    return {
        .voltage_rms =
            voltage_info.rms,

        .current_rms =
            current_info.rms,

        .phase_voltage =
            phase_voltage,

        .phase_current =
            phase_current,

        .phase =
            phase,

        .active =
            active,

        .reactive =
            reactive,

        .apparent =
            apparent,

        .power_factor =
            power_factor
    };
}

// ------------------------------------------------------------
// Impressão de sinais
// ------------------------------------------------------------

void print_signal(
    const std::string& name,
    const SignalInfo& signal,
    const std::string& unit)
{
    std::cout
        << '\n'
        << name
        << '\n'

        << "----------------------------------------\n"

        << "Minimo       : "
        << signal.minimum
        << ' '
        << unit
        << '\n'

        << "Maximo       : "
        << signal.maximum
        << ' '
        << unit
        << '\n'

        << "Pico-a-pico  : "
        << signal.pp
        << ' '
        << unit
        << '\n'

        << "Media        : "
        << signal.mean
        << ' '
        << unit
        << '\n'

        << "RMS          : "
        << signal.rms
        << ' '
        << unit
        << '\n';
}

// ------------------------------------------------------------
// Impressão da tabela de potência
// ------------------------------------------------------------

void print_power_row(
    const std::string& name,
    const PowerInfo& p)
{
    std::cout
        << std::left
        << std::setw(10)
        << name

        << std::right
        << std::setw(13)
        << p.voltage_rms

        << std::setw(13)
        << p.current_rms

        << std::setw(13)
        << p.active

        << std::setw(13)
        << p.reactive

        << std::setw(13)
        << p.apparent

        << std::setw(13)
        << p.power_factor

        << std::setw(13)
        << p.phase

        << '\n';
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

        if(ngSpice_Init(
               send_char,
               nullptr,
               controlled_exit,
               nullptr,
               nullptr,
               nullptr,
               nullptr) != 0)
        {
            throw std::runtime_error(
                "Falha inicializando libngspice"
            );
        }

        // ----------------------------------------------------
        // Carrega circuito
        // ----------------------------------------------------

        ng_command(
            "source questao07.sp"
        );

        ng_command(
            "run"
        );

        // ----------------------------------------------------
        // Vetores do ngspice
        // ----------------------------------------------------

        const auto time =
            get_vector("time");

        const auto v_node1 =
            get_vector("1");

        const auto v_node2 =
            get_vector("2");

        const auto v_node3 =
            get_vector("3");

        const auto source_branch =
            get_vector("vs#branch");

        // ----------------------------------------------------
        // Corrente
        //
        // vs#branch é definida entrando no terminal positivo
        // da fonte.
        //
        // Queremos corrente da fonte para a carga.
        // ----------------------------------------------------

        const auto current =
            negate(
                source_branch
            );

        // ----------------------------------------------------
        // Tensões dos componentes
        // ----------------------------------------------------

        // L1 está entre 1 e 2
        const auto v_l1 =
            subtract(
                v_node1,
                v_node2
            );

        // R1 está entre 2 e 3
        const auto v_r1 =
            subtract(
                v_node2,
                v_node3
            );

        // R2 está entre 3 e terra
        const auto& v_r2 =
            v_node3;

        // Bobina real = L1 + R1
        const auto v_bobina =
            subtract(
                v_node1,
                v_node3
            );

        // ----------------------------------------------------
        // Detecta período da fonte
        // ----------------------------------------------------

        const auto source_crossings =
            rising_zero_crossings(
                time,
                v_node1
            );

        if(source_crossings.size() < 2)
        {
            throw std::runtime_error(
                "Nao foi possivel detectar periodo"
            );
        }

        const double t_begin =
            source_crossings[
                source_crossings.size() - 2
            ];

        const double t_end =
            source_crossings.back();

        const double period =
            t_end -
            t_begin;

        const double frequency =
            1.0 /
            period;

        // ----------------------------------------------------
        // Recorta último período completo
        // ----------------------------------------------------

        const Signal vs =
            crop(
                time,
                v_node1,
                t_begin,
                t_end
            );

        const Signal vb =
            crop(
                time,
                v_bobina,
                t_begin,
                t_end
            );

        const Signal vl =
            crop(
                time,
                v_l1,
                t_begin,
                t_end
            );

        const Signal vr1 =
            crop(
                time,
                v_r1,
                t_begin,
                t_end
            );

        const Signal vr2 =
            crop(
                time,
                v_r2,
                t_begin,
                t_end
            );

        const Signal is =
            crop(
                time,
                current,
                t_begin,
                t_end
            );

        // ----------------------------------------------------
        // Estatísticas dos sinais
        // ----------------------------------------------------

        const SignalInfo vs_info =
            analyze(
                vs.time,
                vs.value
            );

        const SignalInfo vb_info =
            analyze(
                vb.time,
                vb.value
            );

        const SignalInfo vl_info =
            analyze(
                vl.time,
                vl.value
            );

        const SignalInfo vr1_info =
            analyze(
                vr1.time,
                vr1.value
            );

        const SignalInfo vr2_info =
            analyze(
                vr2.time,
                vr2.value
            );

        const SignalInfo current_info =
            analyze(
                is.time,
                is.value
            );

        // ----------------------------------------------------
        // Fases absolutas em relação à fonte
        // ----------------------------------------------------

        constexpr double phi_vs =
            0.0;

        const double phi_bobina =
            phase_degrees(
                time,
                v_bobina,
                t_begin,
                period
            );

        const double phi_l1 =
            phase_degrees(
                time,
                v_l1,
                t_begin,
                period
            );

        const double phi_r1 =
            phase_degrees(
                time,
                v_r1,
                t_begin,
                period
            );

        const double phi_r2 =
            phase_degrees(
                time,
                v_r2,
                t_begin,
                period
            );

        const double phi_current =
            phase_degrees(
                time,
                current,
                t_begin,
                period
            );

        // ----------------------------------------------------
        // Potência por componente
        // ----------------------------------------------------

        const PowerInfo power_r1 =
            analyze_power(
                vr1.time,
                vr1.value,
                is.value,
                phi_r1,
                phi_current
            );

        const PowerInfo power_l1 =
            analyze_power(
                vl.time,
                vl.value,
                is.value,
                phi_l1,
                phi_current
            );

        const PowerInfo power_r2 =
            analyze_power(
                vr2.time,
                vr2.value,
                is.value,
                phi_r2,
                phi_current
            );

        // Circuito total visto pela fonte
        const PowerInfo power_total =
            analyze_power(
                vs.time,
                vs.value,
                is.value,
                phi_vs,
                phi_current
            );

        // ----------------------------------------------------
        // Balanço
        // ----------------------------------------------------

        const double p_components =
            power_r1.active +
            power_l1.active +
            power_r2.active;

        const double q_components =
            power_r1.reactive +
            power_l1.reactive +
            power_r2.reactive;

        // ----------------------------------------------------
        // Saída
        // ----------------------------------------------------

        std::cout
            << std::fixed
            << std::setprecision(6);

        std::cout
            << "\nANALISE DO CIRCUITO R-L-R\n"
            << "========================================\n";

        std::cout
            << "\nJanela analisada\n"
            << "----------------------------------------\n"

            << "Inicio       : "
            << t_begin * 1e3
            << " ms\n"

            << "Fim          : "
            << t_end * 1e3
            << " ms\n"

            << "Periodo      : "
            << period * 1e3
            << " ms\n"

            << "Frequencia   : "
            << frequency
            << " Hz\n";

        // ----------------------------------------------------
        // Sinais
        // ----------------------------------------------------

        print_signal(
            "Fonte Vs",
            vs_info,
            "V"
        );

        print_signal(
            "Bobina real (L1 + R1)",
            vb_info,
            "V"
        );

        print_signal(
            "Indutor L1",
            vl_info,
            "V"
        );

        print_signal(
            "Resistencia interna R1",
            vr1_info,
            "V"
        );

        print_signal(
            "Resistor R2",
            vr2_info,
            "V"
        );

        print_signal(
            "Corrente",
            current_info,
            "A"
        );

        // ----------------------------------------------------
        // Fases
        // ----------------------------------------------------

        std::cout
            << "\nFASES EM RELACAO A Vs\n"
            << "========================================\n"

            << "Vs             : "
            << phi_vs
            << " deg\n"

            << "Bobina         : "
            << phi_bobina
            << " deg\n"

            << "L1             : "
            << phi_l1
            << " deg\n"

            << "R1             : "
            << phi_r1
            << " deg\n"

            << "R2             : "
            << phi_r2
            << " deg\n"

            << "Corrente       : "
            << phi_current
            << " deg\n";

        // ----------------------------------------------------
        // Tabela
        // ----------------------------------------------------

        std::cout
            << "\nANALISE DE POTENCIA POR COMPONENTE\n"
            << "=======================================================================================================\n";

        std::cout
            << std::left
            << std::setw(10)
            << "Elemento"

            << std::right
            << std::setw(13)
            << "Vrms[V]"

            << std::setw(13)
            << "Irms[A]"

            << std::setw(13)
            << "P[W]"

            << std::setw(13)
            << "Q[var]"

            << std::setw(13)
            << "S[VA]"

            << std::setw(13)
            << "FP"

            << std::setw(13)
            << "Phi[deg]"

            << '\n';

        std::cout
            << "-------------------------------------------------------------------------------------------------------\n";

        print_power_row(
            "R1",
            power_r1
        );

        print_power_row(
            "L1",
            power_l1
        );

        print_power_row(
            "R2",
            power_r2
        );

        std::cout
            << "-------------------------------------------------------------------------------------------------------\n";

        print_power_row(
            "TOTAL",
            power_total
        );

        // ----------------------------------------------------
        // Balanço
        // ----------------------------------------------------

        std::cout
            << "\nBALANCO DE POTENCIA\n"
            << "========================================\n"

            << "P total          : "
            << power_total.active
            << " W\n"

            << "P componentes    : "
            << p_components
            << " W\n"

            << "Erro P           : "
            << power_total.active -
               p_components
            << " W\n"

            << '\n'

            << "Q total          : "
            << power_total.reactive
            << " var\n"

            << "Q componentes    : "
            << q_components
            << " var\n"

            << "Erro Q           : "
            << power_total.reactive -
               q_components
            << " var\n";

        // ----------------------------------------------------
        // Classificação do circuito
        // ----------------------------------------------------

        std::cout
            << "\nCARACTERISTICA DA CARGA\n"
            << "========================================\n";

        if(power_total.reactive > 1e-6)
        {
            std::cout
                << "Carga indutiva\n"
                << "Corrente atrasada em relacao a tensao\n";
        }
        else if(power_total.reactive < -1e-6)
        {
            std::cout
                << "Carga capacitiva\n"
                << "Corrente adiantada em relacao a tensao\n";
        }
        else
        {
            std::cout
                << "Carga aproximadamente resistiva\n";
        }

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr
            << "Erro: "
            << e.what()
            << '\n';

        return 1;
    }
}
