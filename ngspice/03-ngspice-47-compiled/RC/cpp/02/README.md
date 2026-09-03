O resultado confirma que a integração C++20 ↔ `libngspice` está funcionando corretamente: o circuito foi carregado, a análise transiente produziu 500 amostras e os vetores `time`, `in` e `out` foram recuperados diretamente da memória do ngspice.

O ponto importante agora é que os números de `v(out)` ainda estão contaminados pelo **transitório inicial**:

```text
v(out)
  Minimo    :    -1.492398 V
  Maximo    :     2.544519 V
  Media     :     0.490598 V
  Amplitude :     2.018458 V
```

Para o seu circuito

$$
R=1\,k\Omega,\qquad C=1\,\mu F,\qquad f=1\,kHz
$$

temos:

$$
\omega RC=2\pi(1000)(1000)(1\times10^{-6})
=2\pi
$$

e a magnitude teórica do filtro RC passa-baixas é

$$
|H(j\omega)|
=
\frac{1}{\sqrt{1+(\omega RC)^2}}
$$

portanto

$$
|H|
\approx
\frac{1}{\sqrt{1+(2\pi)^2}}
\approx0.15718.
$$

Com entrada de amplitude \(10\,V\):

$$
A_{out}\approx10(0.15718)
\approx1.5718\,V.
$$

Então este valor:

```text
Aout/Ain = 0.201849
```

ainda não é o ganho em regime permanente. Esperamos algo próximo de:

```text
Aout/Ain ≈ 0.1572
```

## Próximo passo: analisar apenas o último período

Como sua frequência é:

$$
f=1\,kHz
$$

o período é:

$$
T=\frac1f=1\,ms.
$$

Sua simulação termina em:

```text
3.000000 ms
```

Portanto podemos analisar somente:

```text
2.000 ms → 3.000 ms
```

Sem copiar dados, podemos aproveitar C++20 e `std::span`.

Adicione esta função:

```cpp
std::span<const double> last_period(
    std::span<const double> time,
    std::span<const double> signal,
    double period)
{
    if(time.empty() || signal.empty())
        throw std::runtime_error("Vetor vazio");

    const double start =
        time.back() - period;

    const auto it =
        std::ranges::lower_bound(time, start);

    const auto index =
        static_cast<std::size_t>(
            std::distance(time.begin(), it)
        );

    return signal.subspan(index);
}
```

Então, em vez de:

```cpp
const auto vin_info  = analyze(vin);
const auto vout_info = analyze(vout);
```

use:

```cpp
constexpr double frequency = 1'000.0;

const double period =
    1.0 / frequency;

const auto vin_last =
    last_period(time, vin, period);

const auto vout_last =
    last_period(time, vout, period);

const auto vin_info =
    analyze(vin_last);

const auto vout_info =
    analyze(vout_last);
```

A arquitetura fica interessante porque `std::span` não cria outro vetor:

```text
std::vector<double> vin
│
├───────────── primeiros períodos ─────────────┐
│                                              │
│                              último período  │
│                              ┌───────────────┤
│                              │ std::span     │
└──────────────────────────────┴───────────────┘
```

Ou seja, isto:

```cpp
const auto vin_last =
    last_period(time, vin, period);
```

não copia 150 ou 170 `double`s. O `span` simplesmente referencia uma parte do `std::vector`.

Eu também mudaria a saída para deixar explícito o que estamos medindo:

```text
Analise do ultimo periodo
Intervalo: 2.000000 ms -> 3.000000 ms

v(in)
--------------------------------
  Minimo    :    -9.999...
  Maximo    :     9.999...
  Media     :     0.000...
  Amplitude :     9.999...
  RMS       :     7.071...

v(out)
--------------------------------
  Minimo    :    -1.571...
  Maximo    :     1.571...
  Media     :     0.000...
  Amplitude :     1.571...
  RMS       :     1.111...

Relacao de amplitudes
--------------------------------
  Aout/Ain  :     0.157...
```

E aqui aparece uma melhoria importante de projeto: **não deveríamos colocar `1000.0` novamente no C++**, pois essa frequência já está definida no `RC.sp` como `.param f = 1k`. Na próxima etapa podemos fazer o C++ obter a frequência do próprio ngspice, evitando ter duas fontes de verdade para o mesmo parâmetro.

