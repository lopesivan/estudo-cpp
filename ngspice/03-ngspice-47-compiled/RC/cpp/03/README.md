O que falhou foi a **interpretação dos dados**, não a integração com `libngspice`.

Há dois problemas:

1. você simulou só `3 ms`, sendo que \(RC=1\,ms\). O transitório ainda não morreu;
2. `mean()` e `rms()` estavam tratando as 500 amostras como uniformemente espaçadas, mas o ngspice usa passo adaptativo. O manual diz que `tstep` é um incremento sugerido e que a análise transiente usa seus próprios passos de cálculo. 

### 1. Vamos primeiro corrigir o `RC.sp`

O próprio ngspice tem exatamente o mecanismo que precisamos: ele sempre simula desde \(t=0\), mas, quando fornecemos `tstart`, **não armazena os dados anteriores a `tstart`**. 

Então podemos deixar o circuito estabilizar durante 9 períodos e armazenar somente o décimo período:

```spice
* Circuito RC com fonte senoidal

.param A      = 10
.param f      = 1k
.param n      = 10
.param np     = 500

.param T      = {1/f}

.param tstart = {(n-1)*T}
.param tstop  = {n*T}

.param tstep  = {(tstop-tstart)/(np-1)}

V1 in  0 SIN(0 {A} {f})
R1 in out 1k
C1 out 0 1u

.tran {tstep} {tstop} {tstart}

.end
```

Agora temos conceitualmente:

```text
0 ms                                      9 ms       10 ms
│------------------------------------------│-----------│
        circuito sendo estabilizado            dados
        dados NÃO armazenados                 armazenados
```

Ou seja:

$$
0\rightarrow9\,ms
$$

serve para eliminar o transitório, enquanto:

$$
9\rightarrow10\,ms
$$

é exatamente um período que será entregue ao C++.

Isso é melhor do que simular três períodos e tentar recortar depois no C++.

---

### 2. Corrigir o cálculo de média e RMS

Antes fizemos:

```cpp
mean =
    (x1 + x2 + ... + xn) / n;
```

e:

```cpp
rms =
    sqrt((x1*x1 + ... + xn*xn) / n);
```

Isso pressupõe:

$$
\Delta t_1=\Delta t_2=\cdots
$$

Mas não devemos pressupor isso com os pontos transientes do ngspice.

Vamos calcular corretamente no domínio do tempo:

$$
V_{\text{médio}}
=
\frac{1}{t_f-t_i}
\int_{t_i}^{t_f}v(t)\,dt
$$

e

$$
V_{\mathrm{RMS}}
=
\sqrt{
\frac{1}{t_f-t_i}
\int_{t_i}^{t_f}v^2(t)\,dt
}.
$$

Usaremos integração trapezoidal.

Substitua sua `analyze()` por:

```cpp
SignalInfo analyze(
    std::span<const double> time,
    std::span<const double> signal)
{
    if(time.size() != signal.size())
        throw std::runtime_error(
            "time e signal possuem tamanhos diferentes"
        );

    if(signal.size() < 2)
        throw std::runtime_error(
            "Numero insuficiente de amostras"
        );

    const auto [min_it, max_it] =
        std::ranges::minmax_element(signal);

    double integral     = 0.0;
    double integral_sq  = 0.0;

    for(std::size_t i = 1; i < signal.size(); ++i)
    {
        const double dt =
            time[i] - time[i - 1];

        // Integral de v(t)
        integral +=
            0.5 *
            (signal[i - 1] + signal[i]) *
            dt;

        // Integral de v²(t)
        integral_sq +=
            0.5 *
            (
                signal[i - 1] * signal[i - 1] +
                signal[i]     * signal[i]
            ) *
            dt;
    }

    const double duration =
        time.back() - time.front();

    const double mean =
        integral / duration;

    const double rms =
        std::sqrt(
            integral_sq / duration
        );

    return {
        .min       = *min_it,
        .max       = *max_it,
        .mean      = mean,
        .amplitude = (*max_it - *min_it) / 2.0,
        .rms       = rms
    };
}
```

Agora ela recebe:

```cpp
time
signal
```

porque o tempo passou a fazer parte do cálculo.

Então no `main()` mudamos:

```cpp
const auto vin_info  = analyze(vin);
const auto vout_info = analyze(vout);
```

para:

```cpp
const auto vin_info =
    analyze(time, vin);

const auto vout_info =
    analyze(time, vout);
```

---

### 3. O restante continua igual

A aquisição pela `libngspice` continua exatamente como está:

```cpp
const auto time = get_vector("time");
const auto vin  = get_vector("in");
const auto vout = get_vector("out");
```

Então nosso fluxo continua inteiramente em C++:

```text
RC.sp
   │
   ▼
libngspice
   │
   ├── time
   ├── v(in)
   └── v(out)
          │
          ▼
   std::vector<double>
          │
          ▼
       analyze()
          │
          ├── mínimo
          ├── máximo
          ├── média temporal
          ├── amplitude
          └── RMS temporal
```

### 4. O que esperamos agora

Para `v(in)`:

$$
A_{in}=10\,V
$$

e:

$$
V_{in,RMS}
=
\frac{10}{\sqrt2}
\approx7.071068\,V.
$$

Para o RC:

$$
|H|
=
\frac{1}
{\sqrt{1+(2\pi fRC)^2}}
$$

com

$$
f=1000,\quad R=1000,\quad C=1\mu F
$$

resulta aproximadamente:

$$
|H|\approx0.15718.
$$

Logo:

$$
A_{out}\approx1.5718\,V
$$

e:

$$
V_{out,RMS}
\approx
\frac{1.5718}{\sqrt2}
\approx1.1114\,V.
$$

A saída deverá ficar aproximadamente:

```text
Numero de amostras: 500
Tempo inicial     : 9.xxx ms
Tempo final       : 10.000000 ms

v(in)
--------------------------------
  Minimo    :   -10.000000 V
  Maximo    :    10.000000 V
  Media     :     0.000000 V
  Amplitude :    10.000000 V
  RMS       :     7.071068 V

v(out)
--------------------------------
  Minimo    :    -1.572... V
  Maximo    :     1.572... V
  Media     :     ~0 V
  Amplitude :     1.572... V
  RMS       :     1.111... V

Relacao de amplitudes
--------------------------------
  Aout/Ain  :     0.157...
```

Há ainda uma pequena questão: `time.front()` provavelmente não será **exatamente** `9.000000 ms`, pelo mesmo motivo que anteriormente obtivemos `0.010363 ms` em vez de `0.010000 ms`. Isso decorre dos passos internos do transiente. 

Faça essas duas alterações e rode novamente. O resultado será uma base muito melhor para depois calcularmos **período, frequência e defasagem diretamente em C++20**.




apos simulacao:


Agora os resultados estão coerentes.

Para a entrada:

$$
A_{in}\approx 9.999909\text{ V}
$$

e:

$$
V_{RMS,in}\approx 7.076241\text{ V}
$$

O RMS teórico de uma senoide de amplitude \(10\text{ V}\) é:

$$
\frac{10}{\sqrt 2}\approx 7.071068\text{ V}
$$

Então o erro é pequeno, cerca de \(0.07\%\).

Para a saída:

$$
A_{out}\approx 1.571763\text{ V}
$$

e a relação:

$$
\frac{A_{out}}{A_{in}}
=
0.157178
$$

bate praticamente com o valor teórico do RC em \(1\,\text{kHz}\):

$$
|H(j\omega)|
=
\frac{1}{\sqrt{1+(\omega RC)^2}}
\approx 0.1571777
$$

Logo:

$$
10\times0.1571777
\approx1.571777\text{ V}
$$

Seu resultado:

```text
Amplitude : 1.571763 V
Aout/Ain  : 0.157178
```

está excelente.

Também é importante notar isto:

```text
Tempo inicial : 9.001462 ms
Tempo final   : 10.000000 ms
```

O primeiro ponto não caiu exatamente em `9 ms`, por causa do passo adaptativo interno do ngspice. Isso explica parte da pequena diferença no RMS e na média.

Agora temos uma base sólida em C++20:

```text
libngspice
   ↓
time, vin, vout
   ↓
std::vector<double>
   ↓
análise temporal correta
   ├── mínimo
   ├── máximo
   ├── média
   ├── amplitude
   └── RMS
```

O próximo passo natural é calcular, ainda sem plotar:

* período;
* frequência;
* cruzamentos por zero;
* atraso temporal entre `vin` e `vout`;
* defasagem em graus.

Isso nos permitirá chegar em algo como:

```text
Frequencia        : 1000.000 Hz
Periodo           : 1.000000 ms
Atraso v(out)     : 0.224... ms
Defasagem         : -80.96 deg
Ganho             : 0.157178
Ganho             : -16.07 dB
```

e tudo diretamente a partir dos vetores retornados pela `libngspice`.

