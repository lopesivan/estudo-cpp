* Circuito RC com fonte senoidal

.param A      = 10       ; Amplitude 10
.param f      = 1k       ; frequencia de 1k Hz
.param n      = 3        ; numero de períodos
.param np     = 100      ; número de pontos
.param T      = {1/f}    ; período
.param tstart = T/100    ; tempo inicial
.param tstop  = n*T      ; tempo final
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
    plot v(in) v(out)
.endc

.end
