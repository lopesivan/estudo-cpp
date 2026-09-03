* Circuito RC com fonte senoidal

.param A      = 10
.param f      = 1k
.param n      = 3
.param np     = 500
.param T      = {1/f}
.param tstart = T/100
.param tstop  = n*T
.param tstep  = {(tstop-tstart)/(np-1)}

V1 in   0 SIN(0 {A} {f})
R1 in out 1k
C1 out  0 1u

.tran {tstep} {tstop} {tstart}

.end
