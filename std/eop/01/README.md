```cpp
#include <tuple>
#include <iostream>

int main()
{
    std::tuple<int, int, int> rgb = {255, 128, 0};

    // Acesso via std::get<índice>
    int r = std::get<0> (rgb);
    int g = std::get<1> (rgb);
    int b = std::get<2> (rgb);

    std::cout << "r=" << r << " g=" << g << " b=" << b << std::endl;

    // Structured bindings (C++17) — mais legível
    auto [r2, g2, b2] = rgb;
    std::cout << "r=" << r2 << " g=" << g2 << " b=" << b2 << std::endl;

    return 0;
}
```

Saída:
```
r=255 g=128 b=0
r=255 g=128 b=0
```

`std::get<0>/<1>/<2>` acessa por posição (não por nome, tupla não tem `.r`/`.g`/`.b`). O structured binding (`auto [r, g, b] = rgb`) é a forma mais legível em C++17+, evita `std::get` repetido.

Se for usar RGB com frequência no código, uma `struct` nomeada continua mais clara que tupla — tupla é melhor pra casos genéricos/temporários (retorno múltiplo de função, por exemplo):

```cpp
std::tuple<int,int,int> get_color() { return {255, 0, 128}; }

auto [r, g, b] = get_color();
```
`std::make_tuple` cria uma tupla deduzindo os tipos automaticamente, sem precisar escrever `std::tuple<int,int,int>` explicitamente:

```cpp
#include <tuple>
#include <iostream>

int main()
{
    // std::make_tuple deduz os tipos dos argumentos
    auto rgb = std::make_tuple (255, 128, 0);
    // equivalente a: std::tuple<int, int, int> rgb(255, 128, 0);

    auto [r, g, b] = rgb;
    std::cout << "r=" << r << " g=" << g << " b=" << b << std::endl;

    // útil quando os tipos são diferentes ou verbosos
    auto misto = std::make_tuple (1, 3.14, "texto", 'c');
    // deduz: tuple<int, double, const char*, char>

    std::cout << std::get<0>(misto) << " "
              << std::get<1>(misto) << " "
              << std::get<2>(misto) << " "
              << std::get<3>(misto) << std::endl;

    return 0;
}
```

Diferença prática: com `std::tuple<int,int,int> rgb{255,128,0}` você declara o tipo; com `std::make_tuple` o compilador deduz — mais útil quando os tipos são longos/complexos ou quando a tupla é retornada de uma função:

```cpp
auto get_color()
{
    return std::make_tuple (255, 0, 128); // retorna tuple<int,int,int> deduzido
}
```

Desde C++17, com *class template argument deduction (CTAD)*, `std::tuple` também deduz sozinho (`std::tuple rgb{255,128,0}` funciona), então `make_tuple` ficou menos essencial — mas ainda é comum ver em código.

