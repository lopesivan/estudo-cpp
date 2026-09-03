#include <ngspice/sharedspice.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

struct SweepPoint
{
    double frequency;
    double current_rms;
};

int send_char(char* msg, int, void*)
{
    // Silenciamos a saída normal durante a varredura.
    // Se quiser depurar, descomente:
    //
    // if(msg)
    //     std::cout << "[ngspice] " << msg << '\n';

    return 0;
}

int controlled_exit(int status, bool, bool, int, void*)
{
    if(status != 0)
    {
        std::cerr << "ngspice terminou com status " << status
                  << '\n';
    }

    return 0;
}

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

double rms(std::span<const double> time,
           std::span<const double> signal)
{
    if(time.size() != signal.size())
    {
        throw std::runtime_error(
            "time e signal possuem tamanhos diferentes");
    }

    if(time.size() < 2)
    {
        throw std::runtime_error("Numero insuficiente de amostras");
    }

    double integral = 0.0;

    for(std::size_t i = 1; i < time.size(); ++i)
    {
        const double dt = time[i] - time[i - 1];

        const double x0 = signal[i - 1] * signal[i - 1];

        const double x1 = signal[i] * signal[i];

        integral += 0.5 * (x0 + x1) * dt;
    }

    const double duration = time.back() - time.front();

    return std::sqrt(integral / duration);
}

double simulate_current_rms(double frequency)
{
    // --------------------------------------------------------
    // Altera .param f
    //
    // O manual exige reset após alterparam.
    // --------------------------------------------------------

    ng_command("alterparam f=" + std::to_string(frequency));

    ng_command("reset");
    ng_command("run");

    const auto time = get_vector("time");

    const auto branch = get_vector("v1#branch");

    if(time.size() != branch.size())
    {
        throw std::runtime_error(
            "time e corrente possuem tamanhos diferentes");
    }

    // Para RMS, o sinal da corrente é irrelevante,
    // pois elevamos i(t) ao quadrado.
    return rms(time, branch);
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

        // ----------------------------------------------------
        // Varredura grosseira
        // ----------------------------------------------------

        constexpr double f_begin = 100.0;
        constexpr double f_end   = 1000.0;
        constexpr double f_step  = 10.0;

        std::vector<SweepPoint> sweep;

        double best_frequency = 0.0;

        double best_current_rms =
            -std::numeric_limits<double>::infinity();

        std::cout << std::fixed << std::setprecision(6);

        std::cout << "Varredura de frequencia\n"
                  << "------------------------------------------\n"
                  << std::setw(12) << "f [Hz]" << std::setw(18)
                  << "Irms [mA]" << '\n';

        for(double f = f_begin; f <= f_end; f += f_step)
        {
            const double irms = simulate_current_rms(f);

            sweep.push_back({.frequency = f, .current_rms = irms});

            std::cout << std::setw(12) << f

                      << std::setw(18) << irms * 1e3

                      << '\n';

            if(irms > best_current_rms)
            {
                best_current_rms = irms;
                best_frequency   = f;
            }
        }

        // ----------------------------------------------------
        // Refinamento ao redor do máximo encontrado
        // ----------------------------------------------------

        const double fine_begin = best_frequency - f_step;

        const double fine_end = best_frequency + f_step;

        constexpr double fine_step = 0.1;

        double refined_frequency = best_frequency;

        double refined_current = best_current_rms;

        for(double f = fine_begin; f <= fine_end; f += fine_step)
        {
            const double irms = simulate_current_rms(f);

            if(irms > refined_current)
            {
                refined_current   = irms;
                refined_frequency = f;
            }
        }

        // ----------------------------------------------------
        // Valor teórico
        // ----------------------------------------------------

        constexpr double L = 100e-3;

        constexpr double C = 1e-6;

        constexpr double pi = 3.14159265358979323846;

        const double theoretical =
            1.0 / (2.0 * pi * std::sqrt(L * C));

        // ----------------------------------------------------
        // Resultado
        // ----------------------------------------------------

        std::cout << "\nResultado\n"
                  << "------------------------------------------\n"

                  << "Frequencia numerica : " << refined_frequency
                  << " Hz\n"

                  << "Corrente RMS maxima : "
                  << refined_current * 1e3 << " mA\n"

                  << "Frequencia teorica  : " << theoretical
                  << " Hz\n"

                  << "Erro                : "
                  << refined_frequency - theoretical << " Hz\n";

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Erro: " << e.what() << '\n';

        return 1;
    }
}
