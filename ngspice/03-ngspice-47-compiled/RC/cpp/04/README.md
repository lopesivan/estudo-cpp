Sim. Como o circuito é série, a corrente é a mesma em `R1` e `C1`. O caminho mais simples é recuperar `v1#branch` da `libngspice`; o manual confirma que correntes de ramo aparecem como vetores `xx#branch`. 

Adicione no `main()`:

```cpp
const auto current_source = get_vector("v1#branch");
```

Como queremos definir a corrente no sentido:

```text
fonte -> R1 -> C1 -> terra
```

use:

```cpp
std::vector<double> current(current_source.size());

std::ranges::transform(
    current_source,
    current.begin(),
    [](double x)
    {
        return -x;
    }
);
```

Agora temos:

```cpp
time
vin
vout
current
```

Para esse circuito:

```text
      i(t) →
 V1 ───── R1 ───── C1
       vin     vout
```

as tensões são:

```cpp
v_source = vin
v_resistor = vin - vout
v_capacitor = vout
```

Podemos então calcular potência média diretamente de:

$$
p(t)=v(t)i(t)
$$

e:

$$
P=\frac{1}{T}\int p(t)\,dt
$$

Crie:

```cpp
double average_power(
    std::span<const double> time,
    std::span<const double> voltage,
    std::span<const double> current)
{
    if(time.size() != voltage.size() ||
       time.size() != current.size())
    {
        throw std::runtime_error(
            "Vetores com tamanhos diferentes"
        );
    }

    if(time.size() < 2)
        throw std::runtime_error(
            "Numero insuficiente de amostras"
        );

    double integral = 0.0;

    for(std::size_t i = 1; i < time.size(); ++i)
    {
        const double dt =
            time[i] - time[i - 1];

        const double p0 =
            voltage[i - 1] * current[i - 1];

        const double p1 =
            voltage[i] * current[i];

        integral +=
            0.5 * (p0 + p1) * dt;
    }

    const double duration =
        time.back() - time.front();

    return integral / duration;
}
```

Também podemos analisar a própria corrente com a função `analyze()` que já temos:

```cpp
const auto current_info =
    analyze(time, current);
```

Como `analyze()` atualmente imprime unidades em volts, eu separaria a impressão da corrente:

```cpp
void print_current(
    const SignalInfo& s)
{
    constexpr int label_width = 10;
    constexpr int value_width = 12;

    std::cout
        << "\nCorrente\n"
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
                << value * 1e3
                << " mA\n";
        };

    print("Minimo",    s.min);
    print("Maximo",    s.max);
    print("Media",     s.mean);
    print("Amplitude", s.amplitude);
    print("RMS",       s.rms);
}
```

Agora montamos as tensões dos componentes:

```cpp
std::vector<double> vr(vin.size());

std::ranges::transform(
    vin,
    vout,
    vr.begin(),
    std::minus<> {}
);
```

Ou seja:

```cpp
vr[i] = vin[i] - vout[i];
```

Para as potências:

```cpp
const double p_resistor =
    average_power(time, vr, current);

const double p_capacitor =
    average_power(time, vout, current);
```

Para a fonte existe uma diferença importante de convenção. Como definimos `current` como corrente **saindo do terminal positivo da fonte**, a potência fornecida pela fonte pode ser escrita:

```cpp
const double p_source =
    average_power(time, vin, current);
```

Esse valor representa potência **entregue** pela fonte.

Então imprima:

```cpp
std::cout
    << "\nPotencia media\n"
    << "--------------------------------\n"

    << "  Fonte      : "
    << std::setw(12)
    << p_source
    << " W\n"

    << "  Resistor   : "
    << std::setw(12)
    << p_resistor
    << " W\n"

    << "  Capacitor  : "
    << std::setw(12)
    << p_capacitor
    << " W\n";
```

No seu `main()`, a parte principal ficará assim:

```cpp
const auto time = get_vector("time");
const auto vin  = get_vector("in");
const auto vout = get_vector("out");

const auto current_source =
    get_vector("v1#branch");

std::vector<double> current(current_source.size());

std::ranges::transform(
    current_source,
    current.begin(),
    [](double x)
    {
        return -x;
    }
);

std::vector<double> vr(vin.size());

std::ranges::transform(
    vin,
    vout,
    vr.begin(),
    std::minus<> {}
);

const auto vin_info =
    analyze(time, vin);

const auto vout_info =
    analyze(time, vout);

const auto current_info =
    analyze(time, current);

const double p_resistor =
    average_power(time, vr, current);

const double p_capacitor =
    average_power(time, vout, current);

const double p_source =
    average_power(time, vin, current);
```

E depois:

```cpp
print_signal("v(in)", vin_info);
print_signal("v(out)", vout_info);
print_current(current_info);

std::cout
    << "\nPotencia media\n"
    << "--------------------------------\n"
    << "  Fonte      : "
    << std::setw(12)
    << p_source
    << " W\n"
    << "  Resistor   : "
    << std::setw(12)
    << p_resistor
    << " W\n"
    << "  Capacitor  : "
    << std::setw(12)
    << p_capacitor
    << " W\n";
```

Para o seu circuito, esperamos aproximadamente:

$$
I_{RMS}
=
\frac{V_{RMS}}{|Z|}
$$

com

$$
|Z|
=
\sqrt{R^2+X_C^2}
$$

e:

$$
X_C
=
\frac{1}{2\pi fC}
\approx159.15\Omega.
$$

Assim:

$$
|Z|
\approx
\sqrt{1000^2+159.15^2}
\approx1012.6\Omega
$$

e:

$$
I_{RMS}
\approx
\frac{7.071}{1012.6}
\approx6.98\,mA.
$$

A potência média no resistor deve ficar próxima de:

$$
P_R=I_{RMS}^2R
\approx48.8\,mW.
$$

Já o capacitor ideal deverá apresentar potência média próxima de:

$$
P_C\approx0.
$$

A fonte deverá fornecer aproximadamente a mesma potência absorvida pelo resistor:

$$
P_{\text{fonte}}\approx48.8\,mW.
$$

Então uma saída coerente seria da ordem de:

```text
Corrente
--------------------------------
  Minimo    :    -9.87... mA
  Maximo    :     9.87... mA
  Media     :     0.00... mA
  Amplitude :     9.87... mA
  RMS       :     6.98... mA

Potencia media
--------------------------------
  Fonte      :     0.0488... W
  Resistor   :     0.0488... W
  Capacitor  :     ~0.0000 W
```

O passo seguinte pode ser separar corretamente **potência ativa \(P\)**, **reativa \(Q\)** e **aparente \(S\)** para fonte, resistor e capacitor.

