Sim. No seu `plot.py`, você pode calcular a amplitude diretamente de cada vetor e colocar essa informação na legenda.

Como seus sinais são aproximadamente senoidais, uma forma simples é:

$$
A=\frac{V_{\max}-V_{\min}}{2}
$$

No seu código atual , ficaria assim:

```python
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("rc.dat", skiprows=1)

t = data[:, 0]
vin = data[:, 1]
vout = data[:, 2]

Ain = (vin.max() - vin.min()) / 2
Aout = (vout.max() - vout.min()) / 2

plt.plot(
    t * 1e3,
    vin,
    label=f"v(in)  A={Ain:.3f} V"
)

plt.plot(
    t * 1e3,
    vout,
    label=f"v(out) A={Aout:.3f} V"
)

plt.xlabel("Tempo [ms]")
plt.ylabel("Tensão [V]")
plt.grid()
plt.legend()
plt.show()
```

A legenda ficará aproximadamente:

```text
v(in)   A=9.99 V
v(out)  A=1.xx V
```

Para o seu RC em 1 kHz, teoricamente esperamos algo próximo de:

$$
A_{in}=10\,V
$$

e

$$
A_{out}
=
\frac{10}{\sqrt{1+(2\pi fRC)^2}}
\approx1.57\,V.
$$

Depois podemos acrescentar na mesma legenda também o **ganho**:

$$
\frac{A_{out}}{A_{in}}
$$

e a **defasagem entre `v(in)` e `v(out)`**, deixando o gráfico parecido com as informações que você observaria em um osciloscópio.

