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

struct SignalInfo
{
    double min;
    double max;
    double mean;
    double amplitude;
    double rms;
};

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

void ng_command(const std::string& command)
{
    std::vector<char> buffer(command.begin(), command.end());

    buffer.push_back('\0');

    if(ngSpice_Command(buffer.data()) != 0)
    {
        throw std::runtime_error(
            "Erro executando comando ngspice: " + command);
    }
}

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

        integral += 0.5 * (signal[i - 1] + signal[i]) * dt;

        integral_sq += 0.5 *
                       (signal[i - 1] * signal[i - 1] +
                        signal[i] * signal[i]) *
                       dt;
    }

    const double duration = time.back() - time.front();

    return {.min       = *min_it,
            .max       = *max_it,
            .mean      = integral / duration,
            .amplitude = (*max_it - *min_it) / 2.0,
            .rms       = std::sqrt(integral_sq / duration)};
}

double average_power(std::span<const double> time,
                     std::span<const double> voltage,
                     std::span<const double> current)
{
    if(time.size() != voltage.size() ||
       time.size() != current.size())
    {
        throw std::runtime_error("Vetores com tamanhos diferentes");
    }

    double integral = 0.0;

    for(std::size_t i = 1; i < time.size(); ++i)
    {
        const double dt = time[i] - time[i - 1];

        const double p0 = voltage[i - 1] * current[i - 1];

        const double p1 = voltage[i] * current[i];

        integral += 0.5 * (p0 + p1) * dt;
    }

    return integral / (time.back() - time.front());
}

std::vector<double> difference(std::span<const double> a,
                               std::span<const double> b)
{
    if(a.size() != b.size())
        throw std::runtime_error("Vetores com tamanhos diferentes");

    std::vector<double> result(a.size());

    std::ranges::transform(a, b, result.begin(), std::minus<>{});

    return result;
}

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
                  << label << ": " << std::right
                  << std::setw(value_width) << value * scale << ' '
                  << unit << '\n';
    };

    print("Minimo", signal.min);
    print("Maximo", signal.max);
    print("Media", signal.mean);
    print("Amplitude", signal.amplitude);
    print("RMS", signal.rms);
}

int main()
{
    try
    {
        const int status = ngSpice_Init(send_char,
                                        nullptr,
                                        controlled_exit,
                                        nullptr,
                                        nullptr,
                                        nullptr,
                                        nullptr);

        if(status != 0)
        {
            throw std::runtime_error(
                "Falha ao inicializar libngspice");
        }

        ng_command("source RLC.sp");
        ng_command("run");

        const auto time = get_vector("time");

        const auto vin = get_vector("in");

        const auto vn1 = get_vector("n1");

        const auto vout = get_vector("out");

        const auto v1_branch = get_vector("v1#branch");

        if(time.size() != vin.size() || time.size() != vn1.size() ||
           time.size() != vout.size() ||
           time.size() != v1_branch.size())
        {
            throw std::runtime_error(
                "Vetores com tamanhos diferentes");
        }

        std::vector<double> current(v1_branch.size());

        std::ranges::transform(v1_branch,
                               current.begin(),
                               [](double value) { return -value; });

        /*
         * Topologia esperada:
         *
         * V1 in 0
         * R1 in n1
         * L1 n1 out
         * C1 out 0
         *
         * Portanto:
         *
         * Vr = Vin  - Vn1
         * Vl = Vn1  - Vout
         * Vc = Vout
         */

        const auto vr = difference(vin, vn1);

        const auto vl = difference(vn1, vout);

        const auto& vc = vout;

        const auto vin_info = analyze(time, vin);

        const auto vr_info = analyze(time, vr);

        const auto vl_info = analyze(time, vl);

        const auto vc_info = analyze(time, vc);

        const auto current_info = analyze(time, current);

        const double p_source = average_power(time, vin, current);

        const double p_resistor = average_power(time, vr, current);

        const double p_inductor = average_power(time, vl, current);

        const double p_capacitor = average_power(time, vc, current);

        const double p_components =
            p_resistor + p_inductor + p_capacitor;

        std::cout << std::fixed << std::setprecision(6);

        std::cout << "\nNumero de amostras: " << time.size()
                  << '\n';

        std::cout << "Tempo inicial     : " << time.front() * 1e3
                  << " ms\n";

        std::cout << "Tempo final       : " << time.back() * 1e3
                  << " ms\n";

        print_signal("Fonte - v(in)", vin_info, "V");

        print_signal("Resistor - v(R1)", vr_info, "V");

        print_signal("Indutor - v(L1)", vl_info, "V");

        print_signal("Capacitor - v(C1)", vc_info, "V");

        print_signal("Corrente", current_info, "mA", 1e3);

        std::cout << "\nPotencia media\n"
                  << "--------------------------------\n"

                  << "  Fonte      : " << std::setw(12)
                  << p_source * 1e3 << " mW\n"

                  << "  Resistor   : " << std::setw(12)
                  << p_resistor * 1e3 << " mW\n"

                  << "  Indutor    : " << std::setw(12)
                  << p_inductor * 1e3 << " mW\n"

                  << "  Capacitor  : " << std::setw(12)
                  << p_capacitor * 1e3 << " mW\n"

                  << "  R + L + C  : " << std::setw(12)
                  << p_components * 1e3 << " mW\n"

                  << "  Diferenca  : " << std::setw(12)
                  << (p_source - p_components) * 1e3 << " mW\n";

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Erro: " << e.what() << '\n';

        return 1;
    }
}
