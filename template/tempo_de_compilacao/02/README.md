`std::multiplies` é usado com `std::inner_product` pra
produto escalar (dot product):

```cpp
#include <iostream>
#include <functional>
#include <numeric>
#include <vector>

int main()
{
    std::vector<float> a = {1.0f, 2.0f, 3.0f};
    std::vector<float> b = {4.0f, 5.0f, 6.0f};

    // dot product: soma dos produtos elemento a elemento
    float dot = std::inner_product (
        a.begin(), a.end(),
        b.begin(),
        0.0f,                        // valor inicial
        std::plus<float>{},          // operação de combinação (soma)
        std::multiplies<float>{}     // operação de produto por par
    );

    std::cout << "Produto escalar: " << dot << std::endl;
    // (1*4) + (2*5) + (3*6) = 4 + 10 + 18 = 32
}
```

Saída:
```
Produto escalar: 32
```

Se você quis **produto vetorial (cross product)** —
resultado é outro vetor, não um escalar, e só existe em
3D. `std::multiplies` sozinho não cobre isso porque envolve
subtração entre termos:

```cpp
#include <array>

std::array<float, 3> cross (const std::array<float, 3>& a, const std::array<float, 3>& b)
{
    return {
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    };
}
```




```cpp
#include <array>
#include <vector>
#include <iostream>

int main()
{
    std::array<float, 3> arr = {1.0f, 2.0f, 3.0f};

    // Converter array -> vector
    std::vector<float> vec (arr.begin(), arr.end());
    // ou: std::vector<float> vec (arr.data(), arr.data() + arr.size());

    // Acessar x, y, z — vector não tem .x/.y/.z (não é struct), só índice
    float x = vec[0];
    float y = vec[1];
    float z = vec[2];

    std::cout << "x=" << x << " y=" << y << " z=" << z << std::endl;

    return 0;
}
```

Se quer acessar por **nome** (`v.x`, `v.y`, `v.z`), nem
`array` nem `vector` oferecem isso — são containers
genéricos indexados por posição. Pra isso precisa de uma
struct própria:

```cpp
struct Vec3
{
    float x, y, z;
};

int main()
{
    Vec3 v{1.0f, 2.0f, 3.0f};
    std::cout << "x=" << v.x << " y=" << v.y << " z=" << v.z << std::endl;
}
```

Se o objetivo é fazer produto vetorial/escalar com nomes
legíveis, `Vec3` é o caminho certo — `vector`/`array`
fazem mais sentido quando o tamanho é genérico ou dinâmico,
não fixo em 3D com semântica geométrica.

