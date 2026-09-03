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

.end
