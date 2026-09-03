Vamos transformar a parte de potência em uma análise **por componente**, usando a convenção passiva:

$$
S=V_{\mathrm{RMS}}I_{\mathrm{RMS}}
$$

$$
P=\frac1T\int_0^T v(t)i(t)\,dt
$$

$$
Q=S\sin\phi
$$

$$
FP=\frac{P}{S}=\cos\phi
$$

onde, para cada elemento,

$$
\phi=\phi_V-\phi_I.
$$

No seu circuito série, a mesma corrente passa por `R1`, `L1` e `R2`.

Adicione esta estrutura perto de `SignalInfo`:

```cpp
struct PowerInfo
{
    double voltage_rms;
    double current_rms;

    double phase_voltage;
    double phase_current;
    double phase;

    double active;      // P [W]
    double reactive;    // Q [var]
    double apparent;    // S [VA]

    double power_factor;
};
```

Depois adicione esta função para normalizar ângulos:

```cpp
double normalize_phase(double phase)
{
    while(phase > 180.0)
        phase -= 360.0;

    while(phase <= -180.0)
        phase += 360.0;

    return phase;
}
```

Agora uma função genérica para analisar a potência de **qualquer elemento**:

```cpp
PowerInfo analyze_power(
    std::span<const double> time,
    std::span<const double> voltage,
    std::span<const double> current,
    double phase_voltage,
    double phase_current)
{
    const auto voltage_info =
        analyze(time, voltage);

    const auto current_info =
        analyze(time, current);

    const double phase =
        normalize_phase(
            phase_voltage - phase_current
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
            phase * PI / 180.0
        );

    const double power_factor =
        apparent > 0.0
            ? active / apparent
            : 0.0;

    return {
        .voltage_rms    = voltage_info.rms,
        .current_rms    = current_info.rms,

        .phase_voltage  = phase_voltage,
        .phase_current  = phase_current,
        .phase          = phase,

        .active         = active,
        .reactive       = reactive,
        .apparent       = apparent,

        .power_factor   = power_factor
    };
}
```

E uma função para imprimir a tabela:

```cpp
void print_power_row(
    const std::string& name,
    const PowerInfo& p)
{
    std::cout
        << std::left
        << std::setw(10)
        << name

        << std::right
        << std::setw(12)
        << p.voltage_rms

        << std::setw(12)
        << p.current_rms

        << std::setw(12)
        << p.active

        << std::setw(12)
        << p.reactive

        << std::setw(12)
        << p.apparent

        << std::setw(12)
        << p.power_factor

        << std::setw(12)
        << p.phase

        << '\n';
}
```

No seu `main()`, precisamos calcular também a fase de `R1` e `L1`.

Depois de:

```cpp
const double phi_current =
    phase_degrees(
        time,
        v1,
        current,
        t_begin,
        period
    );
```

adicione:

```cpp
const double phi_l1 =
    phase_degrees(
        time,
        v1,
        v_l1,
        t_begin,
        period
    );

const double phi_r1 =
    phase_degrees(
        time,
        v1,
        v_r1,
        t_begin,
        period
    );

const double phi_r2_element =
    phase_degrees(
        time,
        v1,
        v_r2,
        t_begin,
        period
    );
```

Agora fazemos a análise completa de cada elemento:

```cpp
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
        phi_r2_element,
        phi_current
    );
```

Para a carga completa:

```cpp
const PowerInfo power_total =
    analyze_power(
        vs.time,
        vs.value,
        is.value,
        0.0,
        phi_current
    );
```

Agora podemos substituir aquela parte antiga:

```cpp
std::cout
    << "\nPotencia ativa por componente\n"
    ...
```

por uma tabela muito mais útil:

```cpp
std::cout
    << "\nANALISE DE POTENCIA POR COMPONENTE\n"
    << "=========================================================================="
    << "====================\n";

std::cout
    << std::left
    << std::setw(10) << "Elemento"

    << std::right
    << std::setw(12) << "Vrms[V]"
    << std::setw(12) << "Irms[A]"
    << std::setw(12) << "P[W]"
    << std::setw(12) << "Q[var]"
    << std::setw(12) << "S[VA]"
    << std::setw(12) << "FP"
    << std::setw(12) << "Phi[deg]"
    << '\n';

std::cout
    << "--------------------------------------------------------------------------"
    << "--------------------\n";

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
    << "--------------------------------------------------------------------------"
    << "--------------------\n";

print_power_row(
    "TOTAL",
    power_total
);
```

Eu acrescentaria ainda o balanço de \(P\) e \(Q\):

```cpp
const double p_components =
    power_r1.active +
    power_l1.active +
    power_r2.active;

const double q_components =
    power_r1.reactive +
    power_l1.reactive +
    power_r2.reactive;
```

e:

```cpp
std::cout
    << "\nBalanco de potencia\n"
    << "----------------------------------------\n"

    << "P fonte/carga     : "
    << power_total.active
    << " W\n"

    << "P componentes     : "
    << p_components
    << " W\n"

    << "Erro P            : "
    << power_total.active - p_components
    << " W\n"

    << '\n'

    << "Q total           : "
    << power_total.reactive
    << " var\n"

    << "Q componentes     : "
    << q_components
    << " var\n"

    << "Erro Q            : "
    << power_total.reactive - q_components
    << " var\n";
```

Para o seu circuito, devemos obter aproximadamente algo deste tipo:

```text
ANALISE DE POTENCIA POR COMPONENTE
==============================================================================================

Elemento       Vrms[V]     Irms[A]        P[W]      Q[var]       S[VA]          FP    Phi[deg]
----------------------------------------------------------------------------------------------
R1           17.4588      2.80026      48.8892      ~0          48.889       1.000       ~0
L1           82.1411      2.80026      ~0          230.0       230.0         ~0        +90
R2           70.0065      2.80026     196.0365      ~0         196.037        1.000       ~0
----------------------------------------------------------------------------------------------
TOTAL       119.9889      2.80026     244.9254     230.016     336.000         0.729      43.202
```

Isso revela algo muito importante fisicamente.

Para `R1`:

$$
\phi\approx0^\circ
$$

portanto:

$$
Q_{R1}\approx0
$$

e:

$$
P_{R1}\approx48.89\text{ W}.
$$

Para `R2` ocorre a mesma coisa:

$$
Q_{R2}\approx0
$$

$$
P_{R2}\approx196.04\text{ W}.
$$

Já no indutor:

$$
\phi_L\approx90^\circ
$$

portanto:

$$
P_L\approx0
$$

e praticamente toda sua potência aparente é reativa:

$$
Q_L\approx230\text{ var}.
$$

Assim:

$$
P_{\text{total}}
=
P_{R1}+P_{R2}
$$

e:

$$
Q_{\text{total}}
\approx Q_L.
$$

Para esse circuito específico, portanto:

$$
\boxed{
P\approx244.93\text{ W}
}
$$

$$
\boxed{
Q\approx230.02\text{ var}
}
$$

$$
\boxed{
S\approx336.00\text{ VA}
}
$$

$$
\boxed{
FP\approx0.729
}
$$

com caráter **indutivo**.

O mais interessante é que agora nosso C++ consegue distinguir automaticamente três comportamentos: `R1/R2` consomem potência ativa, `L1` troca potência reativa, e a fonte fornece a combinação vetorial dessas duas componentes.

