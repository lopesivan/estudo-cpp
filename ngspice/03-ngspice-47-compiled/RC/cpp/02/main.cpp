#include <ngspice/sharedspice.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

// ------------------------------------------------------------
// Callbacks do ngspice
// ------------------------------------------------------------

int send_char(char* msg, int, void*)
{
    if(msg)
        std::cout << "[ngspice] " << msg << '\n';

    return 0;
}

int controlled_exit(int status, bool, bool, int, void*)
{
    std::cout
        << "[ngspice] terminou com status "
        << status << '\n';

    return 0;
}

// ------------------------------------------------------------
// Executa um comando no ngspice
// ------------------------------------------------------------

void ng_command(const std::string& command)
{
    std::vector<char> buffer(command.begin(), command.end());
    buffer.push_back('\0');

    const int status = ngSpice_Command(buffer.data());

    if(status != 0)
        throw std::runtime_error(
            "Erro executando comando ngspice: " + command
        );
}

// ------------------------------------------------------------
// Copia um vetor interno do ngspice para std::vector<double>
// ------------------------------------------------------------

std::vector<double> get_vector(const std::string& name)
{
    std::vector<char> buffer(name.begin(), name.end());
    buffer.push_back('\0');

    pvector_info info =
        ngGet_Vec_Info(buffer.data());

    if(!info)
        throw std::runtime_error(
            "Vetor nao encontrado: " + name
        );

    if(!info->v_realdata)
        throw std::runtime_error(
            "Vetor nao possui dados reais: " + name
        );

    return {
        info->v_realdata,
        info->v_realdata + info->v_length
    };
}

// ------------------------------------------------------------
// Estatísticas
// ------------------------------------------------------------

struct SignalInfo
{
    double min;
    double max;
    double mean;
    double amplitude;
    double rms;
};

SignalInfo analyze(std::span<const double> signal)
{
    if(signal.empty())
        throw std::runtime_error("Sinal vazio");

    const auto [min_it, max_it] =
        std::ranges::minmax_element(signal);

    const double mean =
        std::accumulate(
            signal.begin(),
            signal.end(),
            0.0
        ) / static_cast<double>(signal.size());

    const double sum_squared =
        std::transform_reduce(
            signal.begin(),
            signal.end(),
            0.0,
            std::plus<> {},
            [](double x)
            {
                return x * x;
            }
        );

    const double rms =
        std::sqrt(
            sum_squared /
            static_cast<double>(signal.size())
        );

    return {
        .min       = *min_it,
        .max       = *max_it,
        .mean      = mean,
        .amplitude = (*max_it - *min_it) / 2.0,
        .rms       = rms
    };
}

// ------------------------------------------------------------
// Impressão
// ------------------------------------------------------------

void print_signal(
    const std::string& name,
    const SignalInfo& s)
{
    constexpr int label_width = 10;
    constexpr int value_width = 12;

    std::cout
        << '\n'
        << name << '\n'
        << "--------------------------------\n";

    auto print =
        [&](const std::string& label, double value)
        {
            std::cout
                << "  "
                << std::left
                << std::setw(label_width)
                << label

                << ": "

                << std::right
                << std::setw(value_width)
                << value

                << " V\n";
        };

    print("Minimo",    s.min);
    print("Maximo",    s.max);
    print("Media",     s.mean);
    print("Amplitude", s.amplitude);
    print("RMS",       s.rms);
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

        const int status =
            ngSpice_Init(
                send_char,
                nullptr,
                controlled_exit,
                nullptr,
                nullptr,
                nullptr,
                nullptr
            );

        if(status != 0)
            throw std::runtime_error(
                "Falha ao inicializar libngspice"
            );

        // ----------------------------------------------------
        // Carrega o circuito
        // ----------------------------------------------------

        ng_command("source RC.sp");

        // ----------------------------------------------------
        // Executa .tran
        // ----------------------------------------------------

        ng_command("run");

        // ----------------------------------------------------
        // Recupera diretamente os vetores do ngspice
        // ----------------------------------------------------

        const auto time = get_vector("time");
        const auto vin  = get_vector("in");
        const auto vout = get_vector("out");

        if(time.size() != vin.size() ||
           time.size() != vout.size())
        {
            throw std::runtime_error(
                "Vetores com tamanhos diferentes"
            );
        }

        // ----------------------------------------------------
        // Analisa
        // ----------------------------------------------------

        const auto vin_info  = analyze(vin);
        const auto vout_info = analyze(vout);

        // ----------------------------------------------------
        // Resultado
        // ----------------------------------------------------

        std::cout
            << std::fixed
            << std::setprecision(6);

        std::cout
            << "\nNumero de amostras: "
            << time.size()
            << '\n';

        std::cout
            << "Tempo inicial     : "
            << time.front() * 1e3
            << " ms\n";

        std::cout
            << "Tempo final       : "
            << time.back() * 1e3
            << " ms\n";

        print_signal("v(in)", vin_info);
        print_signal("v(out)", vout_info);

        const double gain =
            vout_info.amplitude /
            vin_info.amplitude;

        std::cout
            << "\nRelacao de amplitudes\n"
            << "--------------------------------\n"
            << "  Aout/Ain  : "
            << gain
            << '\n';

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
