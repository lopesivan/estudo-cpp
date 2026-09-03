* Circuito RC com fonte senoidal

.param A      = 10
.param f      = 503.3
.param n      = 10
.param np     = 500

.param T      = {1/f}

.param tstart = {(n-1)*T}
.param tstop  = {n*T}

.param tstep  = {(tstop-tstart)/(np-1)}

*          R1         L1
*  in ───/\/\/──────LLLL────── out
*                                │
*                                C1
*                                │
*                               GND
V1 in 0 SIN(0 {A} {f})

R1 in n1 1k
L1 n1 out 100m
C1 out 0 1u

.tran {tstep} {tstop} {tstart}

.end
