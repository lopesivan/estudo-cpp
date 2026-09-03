#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Signal
{
    std::vector<double> values;

    [[nodiscard]]
    double min() const
    {
        return *std::ranges::min_element(values);
    }

    [[nodiscard]]
    double max() const
    {
        return *std::ranges::max_element(values);
    }

    [[nodiscard]]
    double mean() const
    {
        return std::reduce(values.begin(), values.end(), 0.0) /
               static_cast<double>(values.size());
    }

    [[nodiscard]]
    double amplitude() const
    {
        return (max() - min()) / 2.0;
    }

    [[nodiscard]]
    double rms() const
    {
        const double sum =
            std::transform_reduce(values.begin(),
                                  values.end(),
                                  0.0,
                                  std::plus<>{},
                                  [](double x) { return x * x; });

        return std::sqrt(sum / static_cast<double>(values.size()));
    }
};

struct TransientData
{
    std::vector<double> time;
    Signal              vin;
    Signal              vout;
};

TransientData load_data(const std::string& filename)
{
    std::ifstream file(filename);

    if(!file)
        throw std::runtime_error("Nao foi possivel abrir " +
                                 filename);

    TransientData data;

    // Ignora:
    //
    // time    v(in)    v(out)
    //
    std::string header;
    std::getline(file, header);

    double t;
    double vin;
    double vout;

    while(file >> t >> vin >> vout)
    {
        data.time.push_back(t);
        data.vin.values.push_back(vin);
        data.vout.values.push_back(vout);
    }

    return data;
}

void print_signal(const std::string& name, const Signal& signal)
{
    constexpr int label_width = 10;
    constexpr int value_width = 12;

    std::cout << name << '\n'
              << "--------------------------------\n"

              << "  " << std::left << std::setw(label_width)
              << "Minimo"
              << ": " << std::right << std::setw(value_width)
              << signal.min() << " V\n"

              << "  " << std::left << std::setw(label_width)
              << "Maximo"
              << ": " << std::right << std::setw(value_width)
              << signal.max() << " V\n"

              << "  " << std::left << std::setw(label_width)
              << "Media"
              << ": " << std::right << std::setw(value_width)
              << signal.mean() << " V\n"

              << "  " << std::left << std::setw(label_width)
              << "Amplitude"
              << ": " << std::right << std::setw(value_width)
              << signal.amplitude() << " V\n"

              << "  " << std::left << std::setw(label_width)
              << "RMS"
              << ": " << std::right << std::setw(value_width)
              << signal.rms() << " V\n";
}

int main()
{
    try
    {
        const auto data = load_data("rc.dat");

        std::cout << std::fixed << std::setprecision(6);

        std::cout << "Numero de amostras: " << data.time.size()
                  << "\n\n";

        print_signal("v(in)", data.vin);

        std::cout << '\n';

        print_signal("v(out)", data.vout);

        const double gain =
            data.vout.amplitude() / data.vin.amplitude();

        std::cout << "\nRelacao de amplitudes\n"
                  << "  Aout/Ain = " << gain << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << "Erro: " << e.what() << '\n';
        return 1;
    }
}
