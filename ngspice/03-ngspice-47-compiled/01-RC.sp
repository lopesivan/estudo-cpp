* Circuito RC com fonte senoidal

.param A      = 10       ; Amplitude 10
.param f      = 1k       ; frequencia de 1k Hz
.param n      = 3        ; numero de períodos
.param np     = 100     ; número de pontos
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
* No netlist que gerou essa saída, o $T_{step}$ utilizado
* fez com que o cálculo de pontos teóricos resultasse
* em 100 intervalos, gerando 101 pontos de 0 a 100
* ($\text{Índice } 0 \text{ até } 100$).

.control
    run
    print time
.endc

.end
