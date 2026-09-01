```{cpp}
#include <iostream>
#include <numeric>  // inner_product
#include <vector>

int dotProduct(const std::vector<int>& vec1,
               const std::vector<int>& vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cout << "Erro: os vetores precisam ter o mesmo tamanho"
                  << std::endl;
        return 0;
    }
    return std::inner_product(
        vec1.begin(), vec1.end(), vec2.begin(), 0);
}
```

------------------------------------------------------------------------------

```{cpp}
#include <iostream>
#include <numeric>  // inner_product
#include <vector>

template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cout << "Erro: os vetores precisam ter o mesmo tamanho"
                  << std::endl;
        return T(0);
    }
    return std::inner_product(
        vec1.begin(), vec1.end(), vec2.begin(), T(0));
}
```

------------------------------------------------------------------------------

```{cpp}
#include <numeric>  // inner_product
#include <functional>
#include <vector>
#include <iostream>

template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    return inner_product(vec1.begin(),
                         vec1.end(),
                         vec2.begin(),
                         T{},
                         std::plus<>{},
                         std::multiplies<>{});
}
```

------------------------------------------------------------------------------

```{cpp}
#include <vector>
#include <iostream>
#include <execution>  // para política de execução paralela (opcional)
#include <numeric>

template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    if(vec1.size() != vec2.size())
        return T{};
    return std::transform_reduce(
        vec1.begin(), vec1.end(), vec2.begin(), T{});
    // ou paralelo: transform_reduce(std::execution::par,
    // vec1.begin(), vec1.end(), vec2.begin(), T{});
}
```

