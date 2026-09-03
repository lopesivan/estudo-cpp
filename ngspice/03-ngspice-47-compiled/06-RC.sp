* Circuito RC com fonte senoidal

.param A      = 10       $ Amplitude 10
.param f      = 1k       $ frequencia de 1k Hz
.param n      = 2
.param np     = 150     $ número de pontos
.param T      = {1/f}
.param tstop  = {n*T}
.param tstart = {T/100}
.param tstep  = {(tstop-tstart)/(np-1)}

V1 in   0 SIN(0 {A} {f})
R1 in out 1k
C1 out  0 1u

*---------------------------------------------------------------
* Parâmetros de simulação
*---------------------------------------------------------------
.tran {tstep} {tstop} {tstart}

.control
    run
.endc

.end
