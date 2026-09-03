Em um circuito **RLC**, a frequência de ressonância é a frequência em que o efeito do indutor e do capacitor se **compensam**.

Para entender isso, considere seu RLC série:

```text
             R            L            C
       ┌───/\/\/───LLLLLLLL─────||─────┐
       │                          → I   │
     ~ V                              │
       │                              │
       └──────────────────────────────┘
```

O resistor possui resistência:

$$
R
$$

O indutor possui reatância:

$$
X_L = \omega L = 2\pi fL
$$

e o capacitor:

$$
X_C=\frac{1}{\omega C}
=\frac{1}{2\pi fC}
$$

Observe o comportamento quando aumentamos a frequência:

* \(X_L\) **aumenta** com a frequência;
* \(X_C\) **diminui** com a frequência.

A **ressonância** acontece exatamente quando:

$$
\boxed{X_L=X_C}
$$

Portanto:

$$
\omega L=\frac{1}{\omega C}
$$

Multiplicando por \(\omega C\):

$$
\omega^2LC=1
$$

logo:

$$
\omega_0=\frac{1}{\sqrt{LC}}
$$

Como:

$$
\omega_0=2\pi f_0
$$

temos a famosa expressão:

$$
\boxed{
f_0=
\frac{1}{2\pi\sqrt{LC}}
}
$$

## O que acontece fisicamente no RLC série?

A impedância é:

$$
Z=R+j\left(X_L-X_C\right)
$$

ou:

$$
Z=
R+j\left(
\omega L-\frac{1}{\omega C}
\right)
$$

Na ressonância:

$$
X_L=X_C
$$

portanto:

$$
X_L-X_C=0
$$

e:

$$
\boxed{Z=R}
$$

Isso é extremamente importante.

O circuito inteiro passa a se comportar, visto pela fonte, como se fosse **puramente resistivo**.

Consequentemente:

$$
I=\frac{V}{R}
$$

e a corrente atinge seu **máximo** no RLC série.

Além disso, corrente e tensão da fonte ficam em fase:

$$
\boxed{\phi=0^\circ}
$$

e o fator de potência torna-se:

$$
\boxed{FP=1}
$$

### Um exemplo

Suponha que coloquemos no seu `RLC.sp`:

```spice
R1 in n1 1k
L1 n1 out 100m
C1 out 0 1u
```

Então:

$$
L=100\,mH=0.1\,H
$$

e:

$$
C=1\,\mu F=10^{-6}F
$$

A frequência de ressonância será:

$$
f_0=
\frac{1}
{2\pi\sqrt{0.1(10^{-6})}}
$$

aproximadamente:

$$
\boxed{f_0\approx503.3\ Hz}
$$

Portanto, se sua fonte estiver em:

```spice
.param f = 1k
```

ela **não está na ressonância**.

Se colocarmos:

```spice
.param f = 503.3
```

ficaremos aproximadamente na frequência de ressonância.

E aí nosso programa C++ deverá mostrar algo muito interessante:

```text
Fonte
    Vrms ≈ ...

Corrente
    Irms → máxima

Potência média
    Resistor  → máxima
    Indutor   → aproximadamente 0 W
    Capacitor → aproximadamente 0 W
```

Mas atenção: dizer que a **potência média** de L e C é zero não significa que eles não trocam energia. Durante cada ciclo, o capacitor armazena energia no campo elétrico e o indutor no campo magnético. Na ressonância, essa troca entre L e C é central para o comportamento do circuito.

No nosso projeto com `libngspice`, podemos fazer algo ainda melhor: em vez de simplesmente calcular \(f_0\) pela fórmula, podemos fazer o **C++ variar a frequência da fonte e encontrar numericamente a frequência em que a corrente RMS é máxima**. Essa seria uma excelente forma de demonstrar a ressonância usando o próprio simulador.

