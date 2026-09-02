* Circuito RC com fonte senoidal

.param A      = 10       $ Amplitude 10
.param f      = 1k       $ frequencia de 1k Hz
.param n      = 3
.param T      = {1/f}
.param tstart = T/1000
.param tstop  = n*T

V1 in   0 SIN(0 {A} {f})
R1 in out 1k
C1 out  0 1u

*---------------------------------------------------------------
* Parâmetros de simulação
*---------------------------------------------------------------
* Transiente: passo de impressão T/1000, simular 3 períodos
.tran {tstart} {tstop}

.control
    run
    plot v(in) v(out)

.endc

.end
