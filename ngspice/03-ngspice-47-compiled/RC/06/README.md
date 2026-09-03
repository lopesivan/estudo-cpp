Sim. Como você já está passando os vetores para Python através de `rc.dat`, eu faria algo semelhante para as **medidas escalares** do ngspice.

Dentro do `.control`, depois dos `meas`, os resultados das medições ficam disponíveis como vetores escalares. Então podemos gravá-los em outro arquivo com `wrdata`.

Por exemplo:

```spice id="4sqb8c"
.control
    run

    * Vetores da simulação
    set wr_singlescale
    set wr_vecnames
    option numdgt=16
    wrdata rc.dat v(in) v(out)

    * Interseções
    meas tran tcross1 WHEN v(in)=v(out) CROSS=1
    meas tran vcross1 FIND v(in) WHEN v(in)=v(out) CROSS=1

    meas tran tcross2 WHEN v(in)=v(out) CROSS=2
    meas tran vcross2 FIND v(in) WHEN v(in)=v(out) CROSS=2

    * Passa as medidas para arquivo
    wrdata measures.dat tcross1 vcross1 tcross2 vcross2
.endc
```

A ideia fica:

```text
                   ┌── rc.dat ──────> time, vin, vout
ngspice ───────────┤
                   └── measures.dat -> tcross1, vcross1, ...
```

No Python, você lê os escalares separadamente.

Por exemplo:

```python
measures = np.loadtxt("measures.dat", skiprows=1)

print(measures)
```

Mas existe uma questão com `wrdata`: esses resultados de `.meas` são **escalares**, enquanto `v(in)` e `v(out)` são vetores temporais. Para integração ngspice → Python, eu prefiro uma solução ainda mais limpa: fazer o ngspice escrever explicitamente apenas os números em um arquivo usando `echo`/redirecionamento do interpretador, ou deixar o Python calcular as interseções a partir de `rc.dat`.

Na arquitetura que você está construindo, eu separaria assim:

$$
\boxed{\text{ngspice} \rightarrow \text{dados brutos}}
$$

```text
time
v(in)
v(out)
```

e:

$$
\boxed{\text{Python} \rightarrow \text{processamento}}
$$

```text
amplitude
RMS
cruzamentos
defasagem
ganho
FFT
```

Isso tem uma vantagem importante: seu Python recebe os **mesmos dados simulados** e você pode implementar qualquer algoritmo de análise sem precisar modificar `RC.sp` toda vez.

Por outro lado, se seu objetivo agora é especificamente aprender **como extrair resultados calculados pelo próprio ngspice e passá-los para Python**, então vale fazermos isso: podemos criar um `measures.dat` com uma estrutura simples como

```text
tcross1  0.000...
vcross1  ...
tcross2  0.000...
vcross2  ...
```

e adaptar o `plot.py` para ler **`rc.dat` + `measures.dat`**, usando os cruzamentos calculados pelo ngspice em vez de recalculá-los no Python.


Exato — isso aconteceu porque `wrdata` tratou `tcross1`, `vcross1`, `tcross2`, `vcross2` como **vetores escalares associados à escala atual `time`** e replicou esses valores ao longo de todas as linhas.

Ou seja, seu arquivo contém uma escala temporal na primeira coluna e, nas demais colunas, os mesmos quatro escalares repetidos:

```text
time       tcross1       vcross1       tcross2       vcross2
...
```

Para o Python, você não precisa ler o arquivo inteiro. Basta ler a primeira linha de dados:

```python
import numpy as np

m = np.loadtxt("measures.dat", skiprows=1, max_rows=1)

time0, tcross1, vcross1, tcross2, vcross2 = m

print("tcross1 =", tcross1)
print("vcross1 =", vcross1)
print("tcross2 =", tcross2)
print("vcross2 =", vcross2)
```

Com seus valores:

$$
t_{\text{cross1}} \approx 0.4590951\text{ ms}
$$

$$
V_{\text{cross1}} \approx 2.530605\text{ V}
$$

$$
t_{\text{cross2}} \approx 0.9842860\text{ ms}
$$

$$
V_{\text{cross2}} \approx -0.9828172\text{ V}
$$

Então, para marcar esses pontos no gráfico:

```python
tcross = np.array([tcross1, tcross2])
vcross = np.array([vcross1, vcross2])

plt.scatter(
    tcross * 1e3,
    vcross,
    label="Interseções"
)
```

A arquitetura pode ficar assim:

```text
rc.dat
  -> time
  -> v(in)
  -> v(out)

measures.dat
  -> tcross1
  -> vcross1
  -> tcross2
  -> vcross2
```

No seu caso, como `measures.dat` repete os escalares, usar `max_rows=1` é suficiente e evita carregar dados redundantes.

Se quiser eliminar essa repetição no próprio ngspice, dá para gerar um arquivo de medidas mais enxuto; mas para o Python, do jeito que está, já funciona perfeitamente.

