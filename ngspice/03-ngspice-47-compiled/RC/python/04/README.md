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
