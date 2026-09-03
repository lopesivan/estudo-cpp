Os resultados da simulação estão muito bons e praticamente reproduzem os valores da Questão 07.

A primeira coisa a notar é que no transiente você usou:

```spice
VS 1 0 sin(0 A f)
.param A=169.71
```

No `SIN(VOFF VAMP FREQ)`, esse `A=169.71 V` é **amplitude de pico**, não RMS. Portanto o RMS esperado é:

$$
V_{s,\mathrm{RMS}}
=
\frac{169.71}{\sqrt2}
\approx 120.00\text{ V}
$$

e o ngspice encontrou:

```text
vs_rms = 120.003 V
```

Perfeito.

Para a tensão da bobina, `v(13)=v(1)-v(3)`:

```text
v13_rms = 83.9860 V
```

comparado ao valor do problema:

$$
84\text{ V}
$$

Também praticamente exato.

Para `R2`:

```text
v2_rms = 70.0148 V
```

contra:

$$
70\text{ V}
$$

Novamente excelente.

### Fases

Sua medição de período encontrou:

```text
t_meas = 1.66667e-02 s
```

ou:

$$
T=16.6667\,ms
$$

então:

$$
f=\frac1T\approx60\,Hz.
$$

O uso de `TRIG ... TARG ...` está de acordo com a sintaxe documentada pelo manual para medir diferenças de tempo entre cruzamentos. 

Para `V13`:

```text
tdel_v1    = -1.60746 ms
phi_v1_deg = 34.7212°
```

Você definiu:

```spice
.meas tran phi_V1_deg param='-360*f*tdel_V1'
```

Como o atraso saiu negativo:

$$
\Delta t=-1.60746\,ms
$$

temos:

$$
\phi
=
-360(60)(-1.60746\times10^{-3})
$$

$$
\phi\approx+34.72^\circ.
$$

Isso bate com:

$$
\boxed{V_1\approx84\angle34.7^\circ\text{ V RMS}}
$$

quando a fonte é tomada como referência em \(0^\circ\).

Para `V2`:

```text
tdel_v2    = +1.99908 ms
phi_v2_deg = -43.1802°
```

logo:

$$
\phi_2
=
-360(60)(1.99908\times10^{-3})
$$

$$
\boxed{\phi_2\approx-43.18^\circ}.
$$

Também bate praticamente com seu cálculo:

$$
-43.213^\circ.
$$

### Uma distinção importante no comentário da questão

No cabeçalho você escreveu:

```text
_Vs = <169.71 V, 0°>
_V1 = <118.79 V, 34.792°>
_V2 = <98.995 V, -43.213°>
```

Esses são módulos de **pico**.

Já as medidas laboratoriais e seus `.meas ... RMS` são:

$$
V_s=120\,V
$$

$$
V_1=84\,V
$$

$$
V_2=70\,V.
$$

São exatamente os mesmos sinais, apenas em representações diferentes:

$$
169.71/\sqrt2\approx120
$$

$$
118.79/\sqrt2\approx84
$$

$$
98.995/\sqrt2\approx70.
$$

Eu deixaria isso explícito no comentário para evitar confusão:

```spice
* Fasores em valor de PICO:
*   _Vs = <169.71 V, 0°>
*   _V1 = <118.79 V, 34.792°>
*   _V2 = <98.995 V, -43.213°>
*
* Valores RMS correspondentes:
*   Vs_rms = 120 V
*   V1_rms = 84 V
*   V2_rms = 70 V
```

### Outro ajuste pequeno

Você tem:

```spice
.param w={2*3.14*f}
```

Embora isso nem esteja sendo usado no circuito atual, prefira não usar `3.14` se quiser precisão. Pode usar uma constante mais precisa:

```spice
.param pi=3.141592653589793
.param w={2*pi*f}
```

Seu valor teórico já usa:

$$
\omega=376.991\text{ rad/s},
$$

enquanto `2*3.14*60` daria apenas:

$$
376.8\text{ rad/s}.
$$

### Sobre as 845 linhas

Você especificou:

```spice
.tran 0.1ms {5*T}
```

mas recebeu:

```text
No. of Data Rows : 845
```

Isso não significa que `.tran` falhou. O ngspice realiza a integração transiente com controle adaptativo de passo; o `Tstep` não significa simplesmente “gere exatamente \(T_{stop}/T_{step}\) pontos”. O manual documenta a forma:

```text
tran Tstep Tstop [Tstart [Tmax]]
```

e a análise transiente possui controle próprio do passo de integração. 

No seu caso, isso não prejudicou as medidas porque você explicitamente limitou:

```spice
from='4*T' to='5*T'
```

ou seja, o ngspice calculou RMS, pico e pico-a-pico apenas no último ciclo.

### Resultado consolidado

Sua simulação produziu:

| Grandeza    | Teórico / problema |    ngspice |
| ----------- | -----------------: | ---------: |
| \(V_s\) RMS |              120 V |  120.003 V |
| \(V_1\) RMS |               84 V |   83.986 V |
| \(V_2\) RMS |               70 V |  70.0148 V |
| \(\phi_1\)  |           +34.792° |   +34.721° |
| \(\phi_2\)  |           −43.213° |   −43.180° |
| \(T\)       |         16.6667 ms | 16.6667 ms |

Portanto, **o modelo SPICE está validando muito bem a solução fasorial da questão**.

E isso também nos dá um ótimo próximo exercício para a integração C++: pegar exatamente esse circuito e fazer o programa C++20 obter diretamente pela `libngspice` os três sinais `v(1)`, `v(13)` e `v(3)` e calcular sozinho RMS, período e defasagens, sem usar nenhum `.meas` no arquivo SPICE.





Sim. Eu reorganizaria o `SPICE` por blocos funcionais, padronizaria os nomes dos parâmetros, deixaria claro o que é pico e o que é RMS, e separaria circuito, análise e medições.

```spice
****************************************************************
* Questao 07
* Circuito R-L-R em regime senoidal - 60 Hz
****************************************************************

****************************************************************
* Dados do problema
****************************************************************

* Fonte senoidal:
*   Vs(RMS)  = 120 V
*   Vs(pico) = 169.71 V
*   f        = 60 Hz

.param VS_PK = 169.71
.param FREQ  = 60

* Elementos:
.param R1_VALUE = 6.2347
.param R2_VALUE = 25
.param L1_VALUE = 77.8m

****************************************************************
* Parametros derivados
****************************************************************

.param PI = 3.141592653589793
.param T  = {1/FREQ}
.param W  = {2*PI*FREQ}

****************************************************************
* Valores teoricos de referencia
****************************************************************

* Frequencia angular:
*
*   w = 2*pi*f
*     = 376.991 rad/s
*
* Reatancia indutiva:
*
*   XL = w*L
*      = 29.345 Ohm
*
* Impedancia da bobina:
*
*   Zbobina = R1 + j*XL
*           = 6.2347 + j29.345
*           = 30.000 < 78.005 deg Ohm
*
* Impedancia total:
*
*   Ztotal = (R1 + R2) + j*XL
*          = 31.2347 + j29.345
*          = 42.857 < 43.213 deg Ohm
*
* Corrente:
*
*   I = 3.9598 < -43.213 deg A
*
* Tensoes de pico:
*
*   Vs = 169.71  <   0.000 deg V
*   V1 = 118.79  <  34.792 deg V
*   V2 =  98.995 < -43.213 deg V
*
* Tensoes RMS correspondentes:
*
*   Vs = 120 V
*   V1 =  84 V
*   V2 =  70 V

****************************************************************
* Circuito
****************************************************************

* Fonte:
*
* SIN(VOFF VAMPL FREQ)
*
* VS_PK e amplitude de pico.

VS 1 0 SIN(0 {VS_PK} {FREQ})

* Circuito serie:
*
*        L1          R1          R2
*  1 ----LLLL---- 2 --/\/\-- 3 --/\/\-- 0
*
* V1 = tensao entre os nos 1 e 3
* V2 = tensao sobre R2 = v(3)

L1 1 2 {L1_VALUE}
R1 2 3 {R1_VALUE}
R2 3 0 {R2_VALUE}

****************************************************************
* Fonte controlada auxiliar
****************************************************************

* E13 apenas cria um no cuja tensao e:
*
*   v(13) = v(1) - v(3)
*
* portanto:
*
*   v(13) = tensao sobre a bobina real (L1 + R1)

E13 13 0 1 3 1

****************************************************************
* Analise transiente
****************************************************************

* Simular 5 periodos.
*
* As medidas abaixo utilizam somente o ultimo periodo:
*
*   4*T -> 5*T
*
* Isso reduz a influencia do transitorio inicial.

.tran 0.1ms {5*T}

****************************************************************
* Medidas - Fonte
****************************************************************

.meas tran Vs_pp
+ PP v(1)
+ FROM='4*T'
+ TO='5*T'

.meas tran Vs_rms
+ RMS v(1)
+ FROM='4*T'
+ TO='5*T'

.meas tran Vs_max
+ MAX v(1)
+ FROM='4*T'
+ TO='5*T'

****************************************************************
* Medidas - Bobina real (L1 + R1)
****************************************************************

* v(13) = v(1) - v(3)

.meas tran V1_pp
+ PP v(13)
+ FROM='4*T'
+ TO='5*T'

.meas tran V1_rms
+ RMS v(13)
+ FROM='4*T'
+ TO='5*T'

.meas tran V1_max
+ MAX v(13)
+ FROM='4*T'
+ TO='5*T'

****************************************************************
* Medidas - R2
****************************************************************

* R2 esta entre o no 3 e o terra.
*
* Portanto:
*
*   V2 = v(3)

.meas tran V2_pp
+ PP v(3)
+ FROM='4*T'
+ TO='5*T'

.meas tran V2_rms
+ RMS v(3)
+ FROM='4*T'
+ TO='5*T'

.meas tran V2_max
+ MAX v(3)
+ FROM='4*T'
+ TO='5*T'

****************************************************************
* Periodo medido
****************************************************************

.meas tran T_meas
+ TRIG v(1) VAL=0 RISE=1
+ TARG v(1) VAL=0 RISE=2

****************************************************************
* Defasagem de V1 em relacao a Vs
****************************************************************

.meas tran tdel_V1
+ TRIG v(1)  VAL=0 RISE=1
+ TARG v(13) VAL=0 RISE=1

.meas tran phi_V1_deg
+ PARAM='-360*FREQ*tdel_V1'

****************************************************************
* Defasagem de V2 em relacao a Vs
****************************************************************

.meas tran tdel_V2
+ TRIG v(1) VAL=0 RISE=1
+ TARG v(3) VAL=0 RISE=1

.meas tran phi_V2_deg
+ PARAM='-360*FREQ*tdel_V2'

****************************************************************
* Controle
****************************************************************

.control

    run

    * Exemplos para visualizacao:
    *
    * plot v(1) v(13) v(3)
    *
    * v(1)  -> fonte
    * v(13) -> bobina real
    * v(3)  -> resistor R2

.endc

.end
```

Algumas mudanças foram especialmente importantes. Troquei nomes genéricos como `A` e `f` por `VS_PK` e `FREQ`, porque olhando o arquivo meses depois fica imediatamente claro que `169.71` é tensão de **pico**. Também transformei `R1`, `R2` e `L1` em parâmetros, então os valores físicos do circuito ficam concentrados no início.

Mantive `E13`, mas expliquei explicitamente sua finalidade. Ela não representa um componente físico do circuito; funciona como um ponto auxiliar para termos:

$$
v(13)=v(1)-v(3),
$$

que corresponde à tensão conjunta sobre `L1 + R1`, isto é, a bobina real.

Também padronizei as medidas para `V1` e `V2`, em vez de misturar `V13` com a nomenclatura da questão. Isso torna mais direta a correspondência:

```text
Vs     -> v(1)
V1     -> v(13) = tensão da bobina
V2     -> v(3)  = tensão em R2
```

Eu manteria essa organização como base para depois retirarmos gradualmente os `.meas` e fazermos essas mesmas medições diretamente pelo nosso programa C++20 com `libngspice`.




Está funcionando como deveria. Esse resultado confirma que o arquivo reorganizado não alterou a simulação.

Os valores principais continuam batendo com a questão:

$$
V_s \approx 120.003\text{ V RMS}
$$

$$
V_1 \approx 83.986\text{ V RMS}
$$

$$
V_2 \approx 70.015\text{ V RMS}
$$

e as fases:

$$
\phi_{V1}\approx +34.721^\circ
$$

$$
\phi_{V2}\approx -43.180^\circ
$$

O período medido foi:

$$
T=0.0166667\text{ s}=16.6667\text{ ms}
$$

portanto:

$$
f=\frac1T\approx 60\text{ Hz}.
$$

A linha:

```text
ngspice 423 ->
```

é apenas o prompt interativo do ngspice. O número `423` é o contador interno de comandos/sessão; não é erro.

Também está interessante observar a sequência física do circuito. Como `R2 = 25 Ω` e:

$$
V_{R2,\mathrm{RMS}}=70.0148\text{ V},
$$

podemos obter diretamente a corrente:

$$
I_\mathrm{RMS}
=
\frac{70.0148}{25}
\approx
2.80059\text{ A}.
$$

Aqui aparece um ponto importante: esse valor **não coincide** com o comentário teórico que está no seu arquivo:

```text
I = 3.9598 A
```

Isso ocorre porque naquele comentário `98.995 V` era tratado como tensão de pico sobre `R2`:

$$
I_{\text{pico}}
=
\frac{98.995}{25}
=
3.9598\text{ A}
$$

e o correspondente RMS é:

$$
I_\mathrm{RMS}
=
\frac{3.9598}{\sqrt2}
\approx2.80\text{ A}.
$$

Portanto não há contradição:

```text
Grandeza              Pico            RMS
------------------------------------------------
Vs                  169.71 V       120.00 V
V1                  118.79 V        84.00 V
V2                   98.995 V       70.00 V
I                     3.9598 A       2.800 A
```

Isso sugere uma melhoria adicional no cabeçalho do SPICE: não chamar simplesmente

```text
_I = <3.9598 A, -43.213°>
```

sem indicar a convenção. Eu escreveria explicitamente:

```spice
* Fasores expressos em valores de PICO:
*
*   Vs_pk = 169.71 <   0.000 deg V
*   V1_pk = 118.79 <  34.792 deg V
*   V2_pk =  98.995 < -43.213 deg V
*   I_pk  =   3.9598 < -43.213 deg A
*
* Valores RMS:
*
*   Vs_rms = 120.00 V
*   V1_rms =  84.00 V
*   V2_rms =  70.00 V
*   I_rms  =   2.800 A
```

Assim fica impossível confundir pico com RMS.

E agora seu arquivo está em uma posição muito boa para acrescentarmos mais quatro medições úteis diretamente no ngspice:

$$
I_\mathrm{RMS},\qquad
P,\qquad
S,\qquad
FP
$$

para verificarmos corrente, potência ativa, potência aparente e fator de potência desse circuito.









Esse resultado ficou excelente. O programa C++ está reproduzindo a análise fasorial com erro numérico muito pequeno e, mais importante, o balanço de potência fechou praticamente exatamente.

Os valores principais:

$$
V_s \approx 119.989\text{ V RMS}
$$

$$
V_{\text{bobina}}\approx83.976\text{ V RMS}
$$

$$
V_{R2}\approx70.007\text{ V RMS}
$$

e:

$$
I\approx2.80026\text{ A RMS}
$$

batem com o problema original de \(120\text{ V}\), \(84\text{ V}\) e \(70\text{ V}\).

A frequência medida também ficou praticamente perfeita:

$$
f=59.999996\text{ Hz}
$$

e a fase da corrente:

$$
\phi_I=-43.201993^\circ
$$

confirma que o circuito é indutivo: a corrente está atrasada em relação à fonte.

O fator de potência:

$$
FP=0.728944
$$

é coerente com:

$$
FP=\cos(43.201993^\circ)\approx0.729.
$$

Na parte de potência, o resultado é ainda mais interessante:

$$
P=244.925448\text{ W}
$$

$$
Q=230.016445\text{ var}
$$

$$
S=336.000142\text{ VA}
$$

e essas grandezas satisfazem:

$$
S^2\approx P^2+Q^2.
$$

Além disso, o detalhamento por componente confirma o comportamento físico:

```text
R1 : 48.889153 W
L1 : -0.000215 W
R2 : 196.036510 W
```

O indutor ideal tem potência média praticamente nula, como esperado. Esse pequeno valor negativo de \(-0.215\text{ mW}\) é apenas erro numérico de integração/interpolação.

E o balanço:

```text
R1 + L1 + R2  = 244.925448 W
Fonte forneceu = 244.925448 W
Erro balanco   = ~0 W
```

é uma ótima validação de todo o método.

Também vale destacar que agora o programa não está apenas “lendo o ngspice”; ele já está funcionando como um analisador de circuitos em regime senoidal. A próxima evolução natural seria calcular também:

$$
Q_L,\quad Q_{R1},\quad Q_{R2}
$$

individualmente e montar uma tabela completa por elemento com:

$$
V_{RMS},\ I_{RMS},\ P,\ Q,\ S,\ FP,\ \phi.
$$

Isso deixaria a análise muito próxima de um pequeno analisador de potência em C++ sobre a `libngspice`.

