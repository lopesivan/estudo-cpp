Esse código é um excelente exemplo de **C++ moderno** com uso de:

* `std::ranges` (C++20),
* `constexpr` + `auto&&` genéricos,
* `std::bind`, **lambdas** e **funções de ordem superior**,
* `static_assert` com containers,
* `std::partial_sum` da STL numérica.

Vamos explicar **bloco por bloco** para que fique claro.

---

## 📌 Inclusões:

```cpp
#include <algorithm>   // all_of, any_of, none_of, copy
#include <functional>  // std::bind, std::modulus
#include <iostream>    // std::cout
#include <iterator>    // std::ostream_iterator
#include <numeric>     // std::partial_sum
#include <vector>      // std::vector
```

Inclui várias bibliotecas da STL usadas para algoritmos,
funções e containers.

---

## 📦 Alias de namespace:

```cpp
namespace ranges = std::ranges;
```

* Isso cria um **atalho** para `std::ranges` (introduzido no C++20),
* Permite usar `ranges::all_of`, `ranges::none_of` etc.,
que são versões **mais seguras e poderosas** dos algoritmos
tradicionais.

---

## 🧠 Função personalizada `some_of`

```cpp
constexpr bool some_of(auto&& r, auto&& pred)
{
    return not (ranges::all_of(r, pred) or ranges::none_of(r, pred));
}
```

* Função genérica `some_of`: verifica se **alguns**, mas
**não todos**, os elementos de `r` satisfazem o predicado
`pred`.
* Ela retorna `true` se:

  * **Nem todos** satisfazem (`!all_of`)
  * **E pelo menos um** satisfaz (negando `none_of`)
* O uso de `constexpr` permite usar essa função **em tempo de compilação**.

---

## ✅ Testes em tempo de compilação

```cpp
constexpr auto w = {1, 2, 3};

static_assert(!some_of(w, [](int x) { return x < 1; })); // nenhum < 1
static_assert( some_of(w, [](int x) { return x < 2; })); // só o 1 < 2
static_assert(!some_of(w, [](int x) { return x < 4; })); // todos < 4
```

* Testes usando `static_assert`, verificados em **tempo de compilação**!
* Demonstra a força do `constexpr` no C++ moderno.

---

## 🔢 `main()` começa: vetor e `partial_sum`

```cpp
std::vector<int> v(10, 2); // vetor com dez números 2
std::partial_sum(v.cbegin(), v.cend(), v.begin());
```

* Cria um vetor `v = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}`.
* `partial_sum` faz:

  ```cpp
  v = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
  ```

---

## 🖨️ Exibe os elementos com `ranges::copy`

```cpp
ranges::copy(v, std::ostream_iterator<int>(std::cout, " "));
```

* Usa `ranges::copy` para imprimir todos os elementos do vetor separados por espaço.

---

## 🧪 Testes com `ranges::all_of` e `ranges::none_of`

```cpp
if (ranges::all_of(v.cbegin(), v.cend(), [](int i) { return i % 2 == 0; }))
    std::cout << "All numbers are even\n";

if (ranges::none_of(v, std::bind(std::modulus<int>(), std::placeholders::_1, 2)))
    std::cout << "None of them are odd\n";
```

* `all_of`: verifica se **todos** os números são pares.
* `none_of`: verifica se **nenhum** número é ímpar.
* A segunda usa `std::bind` com `std::modulus` para testar divisão por 2.

---

## 🔁 Lambda gerador de predicados: `DivisibleBy`

```cpp
auto DivisibleBy = [](int d)
{
    return [d](int m) { return m % d == 0; };
};
```

* `DivisibleBy(7)` cria uma **função que testa se um número é divisível por 7**.
* Usado com `ranges::any_of`:

```cpp
if (ranges::any_of(v, DivisibleBy(7)))
    std::cout << "At least one number is divisible by 7\n";
```

* Como 14 é divisível por 7, a condição é verdadeira.

---

## ✅ Resumo geral:

| Parte           | O que faz                                                 |
| --------------- | --------------------------------------------------------- |
| `some_of`       | Testa se **alguns mas não todos** satisfazem um predicado |
| `static_assert` | Verifica a lógica em tempo de compilação                  |
| `partial_sum`   | Gera soma acumulada                                       |
| `ranges::*`     | Usa algoritmos modernos da STL com `std::ranges`          |
| `DivisibleBy`   | Exemplo de **lambda que retorna lambda**                  |

---


