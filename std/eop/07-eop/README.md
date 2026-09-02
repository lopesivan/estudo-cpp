Ambiguidade real: minha versão colide com `std::for_each` via ADL. Corrigindo com namespace próprio.

Compilado e testado.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

namespace eop {
template <class I, class F>
F for_each(I first, I last, F f)
{
    while (first != last)
    {
        f(*first);
        ++first;
    }
    return f; // devolve f (pode carregar estado acumulado)
}
}

int main()
{
    std::vector<int> v{1, 2, 3, 4, 5};

    // 1) efeito colateral (imprimir)
    eop::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ' '; });
    std::cout << '\n';

    // 2) std::for_each da biblioteca padrao -- mesma ideia
    std::for_each(v.begin(), v.end(), [](int& x) { x *= 10; });
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // 3) funtor com estado: for_each devolve o funtor final
    struct Somador {
        int total = 0;
        void operator()(int x) { total += x; }
    };
    Somador s = eop::for_each(v.begin(), v.end(), Somador{});
    std::cout << "soma = " << s.total << '\n';
}
```

Saída real:
```
1 2 3 4 5 
10 20 30 40 50 
soma = 150
```

## O algoritmo

```cpp
template <class I, class F>
F for_each(I first, I last, F f)
{
    while (first != last) { f(*first); ++first; }
    return f;
}
```

Mesmo esqueleto de `distance`, mas com propósito diferente: em vez de contar aplicações de `f` sobre `x`, aplica `f` a **cada elemento** desreferenciado (`*first`) e devolve `f` no final — não um contador. Isso é o que permite o caso 3: se `F` for um funtor com estado (`Somador`), o estado acumulado sobrevive e volta pro chamador via retorno por valor.

## `std::for_each` real (biblioteca padrão)

`#include <algorithm>` já traz exatamente isso — assinatura idêntica, mesmo comportamento (inclusive devolve `f` desde C++11, viabilizando o padrão do funtor acumulador). É por isso que minha versão precisou ir pra `namespace eop` — colidia por ADL com a da `std`.

## Pré-condição, como nos outros do EoP

`[first, last)` precisa ser um intervalo válido e alcançável (`last` chegável a partir de `first` incrementando) — mesma exigência estrutural de `distance`, só que aqui a "utilidade" é side-effect ou acumulação via `F`, não contagem.


Da função `f(x) = sin(x)`, integrada de `a = 0` até `b = π`:

```cpp
double f(double x) { return std::sin(x); }
```

$$\int_0^{\pi} \sin(x)\,dx = [-\cos(x)]_0^{\pi} = -\cos(\pi) - (-\cos(0)) = 1 + 1 = 2$$

Por isso `exata = 2.0` no código — é o valor analítico fechado, usado só para conferir o resultado numérico (`resultado = 2`, erro real `2.45e-08`).


Não — é o `std::sin` de verdade, chamado diretamente, sem aproximação nem substituição:

```cpp
double f(double x) { return std::sin(x); }
```

E dentro do trapézio:
```cpp
[&](double acc, int i) { return acc + fn(a + i * h); }
```

`fn` aqui é exatamente esse `f`, então cada termo da soma é uma chamada real a `std::sin(x)` (implementação da biblioteca padrão, precisão de máquina) — não é o seno aproximado por ângulo pequeno que fizemos há algumas mensagens atrás.

## Por que a dúvida faz sentido

Nesta conversa apareceram **dois usos diferentes** de seno, e é fácil misturar:

| Exemplo | O que fez com seno |
|---|---|
| Cálculo de `sin(θ)` via bisseção + ângulo duplo | **Calculou** o seno do zero, sem usar `std::sin` — só a aproximação `sinθ≈θ` no fundo + fórmula de duplicação subindo |
| Integral `∫sin(x)dx` de `0` a `π` | **Usa** `std::sin` como função dada, pronta da biblioteca — o alvo aqui é a área sob a curva, não reimplementar o seno |

No exemplo da integral, `sin` é só a função escolhida para testar `trapezoidal_accumulate` (porque tem integral exata conhecida, `= 2`, fácil de verificar). Poderia ser qualquer outra função — `x*x`, `exp(x)`, etc. — o algoritmo de integração não sabe nem se importa que é seno.

Confirmado: `std::sin` bate exatamente com os valores conhecidos (`sin(0)=0`, `sin(π/2)=1`, `sin(π)≈0`, com erro de ponto flutuante `~1.2e-16`, típico da implementação real da libm). Nenhuma descaracterização — é a função trigonométrica padrão, íntegra.

