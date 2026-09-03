Vamos fazer uma varredura de frequência no próprio C++20 usando a `libngspice`. O mecanismo correto para alterar `.param f` é `alterparam f=...` seguido obrigatoriamente de `reset`; o manual diz explicitamente que `reset` é necessário para a alteração entrar em vigor.  

Primeiro, use um `RLC.sp` realmente RLC série:

```spice id="fp3r9s"
* Circuito RLC serie com fonte senoidal

.param A      = 10
.param f      = 1k
.param n      = 20
.param np     = 500

.param T      = {1/f}

.param tstart = {(n-1)*T}
.param tstop  = {n*T}

.param tstep  = {(tstop-tstart)/(np-1)}

V1 in 0 SIN(0 {A} {f})

R1 in n1 1k
L1 n1 out 100m
C1 out 0 1u

.tran {tstep} {tstop} {tstart}

.end
```

Para esses valores,

$$
L=0.1\,H,\qquad C=1\,\mu F
$$

a frequência teórica é aproximadamente:

$$
f_0=\frac{1}{2\pi\sqrt{LC}}\approx503.3\text{ Hz}.
$$

O programa abaixo procura numericamente a frequência em que `Irms` é máxima.

```cpp
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
        std::cerr
            << "ngspice terminou com status "
            << status
            << '\n';
    }

    return 0;
}

void ng_command(const std::string& command)
{
    std::vector<char> buffer(
        command.begin(),
        command.end()
    );

    buffer.push_back('\0');

    const int status =
        ngSpice_Command(buffer.data());

    if(status != 0)
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

double rms(
    std::span<const double> time,
    std::span<const double> signal)
{
    if(time.size() != signal.size())
    {
        throw std::runtime_error(
            "time e signal possuem tamanhos diferentes"
        );
    }

    if(time.size() < 2)
    {
        throw std::runtime_error(
            "Numero insuficiente de amostras"
        );
    }

    double integral = 0.0;

    for(std::size_t i = 1;
        i < time.size();
        ++i)
    {
        const double dt =
            time[i] - time[i - 1];

        const double x0 =
            signal[i - 1] * signal[i - 1];

        const double x1 =
            signal[i] * signal[i];

        integral +=
            0.5 * (x0 + x1) * dt;
    }

    const double duration =
        time.back() - time.front();

    return std::sqrt(
        integral / duration
    );
}

double simulate_current_rms(double frequency)
{
    // --------------------------------------------------------
    // Altera .param f
    //
    // O manual exige reset após alterparam.
    // --------------------------------------------------------

    ng_command(
        "alterparam f="
        + std::to_string(frequency)
    );

    ng_command("reset");
    ng_command("run");

    const auto time =
        get_vector("time");

    const auto branch =
        get_vector("v1#branch");

    if(time.size() != branch.size())
    {
        throw std::runtime_error(
            "time e corrente possuem tamanhos diferentes"
        );
    }

    // Para RMS, o sinal da corrente é irrelevante,
    // pois elevamos i(t) ao quadrado.
    return rms(time, branch);
}

int main()
{
    try
    {
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
        {
            throw std::runtime_error(
                "Falha ao inicializar libngspice"
            );
        }

        ng_command("source RLC.sp");

        // ----------------------------------------------------
        // Varredura grosseira
        // ----------------------------------------------------

        constexpr double f_begin = 100.0;
        constexpr double f_end   = 1000.0;
        constexpr double f_step  = 10.0;

        std::vector<SweepPoint> sweep;

        double best_frequency =
            0.0;

        double best_current_rms =
            -std::numeric_limits<double>::infinity();

        std::cout
            << std::fixed
            << std::setprecision(6);

        std::cout
            << "Varredura de frequencia\n"
            << "------------------------------------------\n"
            << std::setw(12)
            << "f [Hz]"
            << std::setw(18)
            << "Irms [mA]"
            << '\n';

        for(double f = f_begin;
            f <= f_end;
            f += f_step)
        {
            const double irms =
                simulate_current_rms(f);

            sweep.push_back({
                .frequency   = f,
                .current_rms = irms
            });

            std::cout
                << std::setw(12)
                << f

                << std::setw(18)
                << irms * 1e3

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

        const double fine_begin =
            best_frequency - f_step;

        const double fine_end =
            best_frequency + f_step;

        constexpr double fine_step =
            0.1;

        double refined_frequency =
            best_frequency;

        double refined_current =
            best_current_rms;

        for(double f = fine_begin;
            f <= fine_end;
            f += fine_step)
        {
            const double irms =
                simulate_current_rms(f);

            if(irms > refined_current)
            {
                refined_current   = irms;
                refined_frequency = f;
            }
        }

        // ----------------------------------------------------
        // Valor teórico
        // ----------------------------------------------------

        constexpr double L =
            100e-3;

        constexpr double C =
            1e-6;

        constexpr double pi =
            3.14159265358979323846;

        const double theoretical =
            1.0 /
            (
                2.0 *
                pi *
                std::sqrt(L * C)
            );

        // ----------------------------------------------------
        // Resultado
        // ----------------------------------------------------

        std::cout
            << "\nResultado\n"
            << "------------------------------------------\n"

            << "Frequencia numerica : "
            << refined_frequency
            << " Hz\n"

            << "Corrente RMS maxima : "
            << refined_current * 1e3
            << " mA\n"

            << "Frequencia teorica  : "
            << theoretical
            << " Hz\n"

            << "Erro                : "
            << refined_frequency - theoretical
            << " Hz\n";

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
```

A lógica é:

```text
100 Hz
   ↓
alterparam f=100
reset
run
Irms

110 Hz
   ↓
alterparam f=110
reset
run
Irms

...

500 Hz
   ↓
Irms muito alta

...

1000 Hz
```

O primeiro `for` encontra aproximadamente onde está o pico:

```cpp
for(double f = 100.0; f <= 1000.0; f += 10.0)
```

Então, se o máximo ocorrer em:

```text
500 Hz
```

fazemos uma segunda busca mais fina aproximadamente entre:

```text
490 Hz ... 510 Hz
```

com resolução:

```cpp
0.1 Hz
```

Por isso esperamos algo próximo de:

```text
Resultado
------------------------------------------
Frequencia numerica : 503.300000 Hz
Corrente RMS maxima : 7.071... mA
Frequencia teorica  : 503.292121 Hz
Erro                : 0.007879 Hz
```

No nosso circuito, na ressonância:

$$
Z=R=1000\,\Omega
$$

e a fonte tem amplitude \(10\,V\), portanto:

$$
V_{RMS}=\frac{10}{\sqrt2}=7.071\,V
$$

e:

$$
I_{RMS,\max}
=
\frac{7.071}{1000}
\approx
7.071\,mA.
$$

Isso nos dá duas verificações independentes: o **pico de corrente deve ocorrer perto de 503.3 Hz** e esse pico deve ser aproximadamente **7.071 mA**.

