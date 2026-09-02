* Circuito RC com fonte senoidal

.param f = 1k       $ frequencia de 1k Hz
.param A = 10       $ Amplitude 10
.param T = {1/f}

V1 in   0 SIN(0 {A} {f})
R1 in out 1k
C1 out  0 1u

*---------------------------------------------------------------
* Parâmetros de simulação
*---------------------------------------------------------------
* Transiente: passo de impressão T/1000, simular 3 períodos
.tran {T/1000} {3*T}

.end
