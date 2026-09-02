A `std::iota` (definida na biblioteca `<numeric>`) preenche um intervalo de elementos com **valores sequenciais crescentes**, começando por um valor inicial e incrementando com `++` a cada elemento.

---

### Assinatura e Funcionamento

```cpp
template< class ForwardIterator, class T >
void iota( ForwardIterator first, ForwardIterator last, T value );

```

1. **`first`, `last`:** O intervalo de memória/coleta que será preenchido (`[first, last)`).
2. **`value`:** O valor atribuído ao primeiro elemento. Os subsequentes recebem `value + 1`, `value + 2`, etc.

---

### Exemplo Básico

```cpp
#include <iostream>
#include <vector>
#include <numeric> // Necessário para std::iota

int main() {
    std::vector<int> vec(5);

    // Preenche 'vec' com: 10, 11, 12, 13, 14
    std::iota(vec.begin(), vec.end(), 10);

    for (int n : vec) {
        std::cout << n << " ";
    }
}

```

---

### Principais Casos de Uso

* **Linearizar índices:** Inicializar um vetor de índices `[0, 1, 2, ..., N-1]` para ordenação indireta (`std::sort` com comparador customizado).
* **Preencher matrizes contíguas:** Como no exemplo da `MatrixND`, preencher sequencialmente todo o bloco de memória contíguo de `data_` sem precisar de loops `for` aninhados.
* **Gerar sequências de caracteres:** Funciona com qualquer tipo que suporte incremento (`++`), como `char`:
```cpp
std::vector<char> alfabeto(26);
std::iota(alfabeto.begin(), alfabeto.end(), 'a'); // 'a', 'b', 'c'...

```
