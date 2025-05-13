✅ **C++23 dá continuidade ao suporte ao recurso `import`**,
que faz parte do **sistema de módulos**, uma grande
modernização da linguagem iniciada no **C++20**.

---

## 📦 O que é `import` em C++?

O `import` é usado para **substituir os antigos `#include`**, oferecendo:

* **Compilação mais rápida** (evita múltiplas inclusões e parsing repetido),
* **Melhor encapsulamento** (exposição controlada),
* **Dependências explícitas** (sem macrobagunça 😄).

---

## ✅ Exemplo básico em C++20/C++23

### 🔹 Módulo: `math_utils.ixx`

```cpp
export module math_utils;

export int soma(int a, int b) {
    return a + b;
}
```

### 🔹 Programa: `main.cpp`

```cpp
import math_utils;
#include <iostream>

int main() {
    std::cout << soma(2, 3) << "\n";
}
```

---

## 🛠️ Como compilar com suporte a módulos

O suporte a `import` **ainda não é universal** em todos os
compiladores e toolchains. Aqui vai um resumo:

### ✅ `g++` (GCC 13+):

```bash
g++ -std=c++20 -fmodules-ts math_utils.ixx main.cpp -o prog
```

### ✅ `clang++` (parcial em C++20, melhor em C++23+)

```bash
clang++ -std=c++23 -fmodules math_utils.ixx main.cpp -o prog
```

### ✅ `MSVC` (Visual Studio 2019+):

* Suporte a módulos com `/interface`, `/module`, `/exportHeader`, etc.

---

## 📦 Módulos padrão em C++23

Além de módulos personalizados, o C++23 também avança
no suporte a **módulos da biblioteca padrão** (denominados
**std modules**). Exemplos:

```cpp
import std;              // importa tudo (se suportado)
import std.io;           // I/O (parcialmente disponível)
```

*Nota: ainda depende fortemente do compilador.*

---

## 🧠 Resumo

| Questão                   | Resposta                                                   |
| ------------------------- | ---------------------------------------------------------- |
| `import` existe em C++23? | ✅ Sim (continuação do C++20)                               |
| Substitui `#include`?     | ✅ Em muitos casos, sim                                     |
| É amplamente suportado?   | ⚠️ Ainda em evolução, principalmente no GCC e Clang        |
| Ganhos?                   | 🚀 Compilação mais rápida, melhor organização e isolamento |

---

