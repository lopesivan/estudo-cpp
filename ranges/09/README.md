# std::span

```cpp
std::span<int> linha(matriz[0], 3);
```

---

## 🧠 O que é `std::span`?

`std::span` é uma **view não proprietária** (não aloca
memória) sobre uma sequência contínua de elementos. Ele
**não copia os dados**, apenas **aponta** para eles, como
um **"ponteiro moderno com tamanho embutido"**.

Disponível a partir do **C++20**, e declarado em:

```cpp
#include <span>
```

---

## ✅ Significado da linha:

```cpp
std::span<int> linha(matriz[0], 3);
```

### 📌 `matriz[0]`:

* Isso retorna um **`int*`**, ou seja, um ponteiro para
o primeiro elemento da **primeira linha** da matriz `int
matriz[3][3]`.

### 📌 `3`:

* É o **número de elementos** que esse `span` vai abranger
— neste caso, 3 inteiros da primeira linha.

### 📌 Resultado:

* `linha` agora é um **range** sobre os 3 elementos da linha
`matriz[0]`, que pode ser iterado, modificado, usado com
`views`, etc.

---

## ✅ O que isso te permite fazer

```cpp
for (int& x : linha)
    x *= 2; // dobra os valores da primeira linha

for (int x : linha)
    std::cout << x << " ";  // imprime: 2 4 6 (se era 1 2 3)
```

Ou com `views::transform`:

```cpp
auto dobrado = linha | std::views::transform([](int x) { return x * 2; });

for (int x : dobrado)
    std::cout << x << " ";
```

---

## 🧪 Exemplo completo:

```cpp
#include <iostream>
#include <span>
#include <ranges>

int main() {
    int matriz[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::span<int> linha(matriz[0], 3); // view da primeira linha

    std::cout << "Dobrado: ";
    for (int x : linha | std::views::transform([](int x) { return x * 2; }))
        std::cout << x << " ";

    std::cout << "\nOriginal: ";
    for (int x : linha)
        std::cout << x << " ";
}
```

---

## ✅ Vantagens de `std::span`

| Vantagem                                    | Descrição                                 |
| ------------------------------------------- | ----------------------------------------- |
| Sem cópia                                   | Trabalha direto sobre os dados existentes |
| Tamanho embutido                            | Mais seguro que ponteiros crus            |
| Compatível com `ranges`                     | Ideal para C++20 funcional                |
| Pode ser usado com arrays C ou `std::array` | Alta flexibilidade                        |

---


