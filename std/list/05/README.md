Para imprimir apenas os 3 primeiros ou 3 últimos elementos
de uma lista, você tem várias opções:

## Para imprimir os 3 primeiros elementos:

### Opção 1: Usando um contador
```cpp
int count = 0;
for (auto& i : l)
{
    if (count >= 3) break;
    std::cout << "Number: " << i << std::endl;
    count++;
}
```

### Opção 2: Usando iteradores
```cpp
auto it = l.begin();
for (int i = 0; i < 3 && it != l.end(); ++i, ++it)
{
    std::cout << "Number: " << *it << std::endl;
}
```

### Opção 3: Usando `std::advance` (C++11)
```cpp
auto it = l.begin();
auto end = l.begin();
std::advance(end, std::min(3, (int)l.size()));
for (; it != end; ++it)
{
    std::cout << "Number: " << *it << std::endl;
}
```

## Para imprimir os 3 últimos elementos:

### Opção 1: Usando `std::list::rbegin()` (reverso)
```cpp
int count = 0;
for (auto it = l.rbegin(); it != l.rend() && count < 3; ++it, ++count)
{
    std::cout << "Number: " << *it << std::endl;
}
```

### Opção 2: Avançando do início
```cpp
auto it = l.begin();
int size = l.size();
std::advance(it, std::max(0, size - 3));  // Pula para os 3 últimos
for (; it != l.end(); ++it)
{
    std::cout << "Number: " << *it << std::endl;
}
```

### Opção 3: Usando `std::prev` (C++11)
```cpp
auto it = l.end();
for (int i = 0; i < 3 && it != l.begin(); ++i)
{
    --it;
    std::cout << "Number: " << *it << std::endl;
}
```

## Exemplo completo:

```cpp
#include <iostream>
#include <list>

int main()
{
    std::list<int> l = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "--- 3 PRIMEIROS ---\n";
    int count = 0;
    for (auto& i : l)
    {
        if (count >= 3) break;
        std::cout << "Number: " << i << std::endl;
        count++;
    }

    std::cout << "\n--- 3 ÚLTIMOS ---\n";
    count = 0;
    for (auto it = l.rbegin(); it != l.rend() && count < 3; ++it, ++count)
    {
        std::cout << "Number: " << *it << std::endl;
    }

    return 0;
}
```

**Saída:**
```
--- 3 PRIMEIROS ---
Number: 1
Number: 2
Number: 3

--- 3 ÚLTIMOS ---
Number: 10
Number: 9
Number: 8
```

## Funções auxiliares (reutilizáveis):

```cpp
template<typename T>
void printFirstN(const std::list<T>& l, int n)
{
    int count = 0;
    for (const auto& i : l)
    {
        if (count >= n) break;
        std::cout << "Number: " << i << std::endl;
        count++;
    }
}

template<typename T>
void printLastN(const std::list<T>& l, int n)
{
    int count = 0;
    for (auto it = l.rbegin(); it != l.rend() && count < n; ++it, ++count)
    {
        std::cout << "Number: " << *it << std::endl;
    }
}

// Uso:
printFirstN(l, 3);  // 3 primeiros
printLastN(l, 3);   // 3 últimos
```

A **Opção 1** com contador é a mais simples e legível para os primeiros elementos, enquanto para os últimos, a iteração reversa com `rbegin()` é a mais direta e eficiente.
