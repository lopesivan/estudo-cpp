Não. `std::array<T, N>` tem `N` fixo em compile-time — faz
parte do tipo. Não existe `push_back`, `resize` nem `insert`.

```cpp
std::array<int, 5> arr;
arr.resize(10); // erro de compilação — método não existe
```

Se precisa crescer, as opções são:

**1. Use `std::vector`** (a resposta certa na maioria dos casos):
```cpp
std::vector<int> v = {5, 3, 1, 4, 2};
v.push_back(99); // cresce sem problema
```

**2. Crie um novo `std::array` maior** (só funciona se o
novo tamanho também é conhecido em compile-time):
```cpp
std::array<int, 5> arr = {5, 3, 1, 4, 2};
std::array<int, 10> bigger{};
std::copy(arr.begin(), arr.end(), bigger.begin());
```

Se o tamanho final não é conhecido até rodar o programa
(input do usuário, tamanho de arquivo, etc.), `std::array`
simplesmente não serve — use `vector`.


Sim. `{}` (list-init vazia) faz **value-initialization**
de todos os elementos — para `int`, isso é zero.

```cpp
std::array<int, 10> bigger{};
// bigger = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
```

Sem o `{}`:
```cpp
std::array<int, 10> bigger;
// valores indeterminados (lixo de memória) — igual a int arr[10] sem inicializar
```

`std::array` não tem construtor tipo `vector(n, valor)`. Opções:

**1. `.fill()`** — mais idiomático, preenche todos os elementos depois de construído:
```cpp
std::array<int, 10> arr;
arr.fill(7);
// arr = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7}
```

**2. Listar todos os valores na inicialização** (só viável para arrays pequenos):
```cpp
std::array<int, 5> arr = {7, 7, 7, 7, 7};
```

**3. `std::fill`** (mesmo efeito que `.fill()`, via `<algorithm>`):
```cpp
#include <algorithm>
std::fill(arr.begin(), arr.end(), 7);
```

Prefira `.fill(7)` — é o método dedicado do próprio `std::array`.
