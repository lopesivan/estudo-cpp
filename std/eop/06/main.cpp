Compilado e testado — `std::accumulate`, o algoritmo que citei lá atrás para resolver o problema que `distance` não conseguia (acumular valores, não só contar).

```cpp
#include <numeric>   // std::accumulate

namespace eop {
template <class I, class T, class Op>
T accumulate(I first, I last, T init, Op op)
{
    T acc = init;
    while (first != last)
    {
        acc = op(acc, *first);
        ++first;
    }
    return acc;
}
}

int main()
{
    std::vector<int> v{1, 2, 3, 4, 5};

    int soma_std = std::accumulate(v.begin(), v.end(), 0);
    // soma_std = 15

    long long produto = std::accumulate(v.begin(), v.end(), 1LL,
        [](long long acc, int x) { return acc * x; });
    // produto = 120

    std::vector<std::string> palavras{"eop", "e", "algoritmos", "genericos"};
    std::string frase = std::accumulate(
        std::next(palavras.begin()), palavras.end(), palavras[0],
        [](std::string acc, const std::string& w) { return acc + " " + w; });
    // frase = "eop e algoritmos genericos"
}
```

Saída real:
```
soma (eop::accumulate) = 15
soma (std::accumulate) = 15
produto (std::accumulate) = 120
frase (std::accumulate) = eop e algoritmos genericos
```

## Estrutura, mesmo esqueleto da família

```cpp
while (first != last) { acc = op(acc, *first); ++first; }
```

Comparando os três que já compilamos:

| Algoritmo | Estado carregado | O que devolve |
|---|---|---|
| `distance` | só o contador `n` | quantos passos |
| `for_each` | o funtor `f` inteiro (pode ter estado) | o próprio `f` |
| `accumulate` | acumulador `acc` de tipo `T` explícito | `acc` final |

`accumulate` é estritamente mais flexível que os outros dois pra esse tipo de tarefa — dá pra somar (`+`), multiplicar (`*`), concatenar strings, ou qualquer `Op: (T, valor) -> T` que você passar. É por isso que, no exemplo anterior da integral, eu não usei `distance` pra somar as avaliações do trapézio — teria sido o erro que você intuiu: forçar a peça errada no problema errado.
