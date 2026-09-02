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

