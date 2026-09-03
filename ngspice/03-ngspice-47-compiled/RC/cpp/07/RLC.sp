* Circuito RLC serie com fonte senoidal

.param A      = 10
.param f      = 1k
.param n      = 20
.param np     = 500

.param T      = {1/f}

.param tstart = {(n-1)*T}
.param tstop  = {n*T}

.param tstep  = {(tstop-tstart)/(np-1)}

V1 in 0 SIN(0 {A} {f})

R1 in n1 1k
L1 n1 out 100m
C1 out 0 1u

.tran {tstep} {tstop} {tstart}

.end
