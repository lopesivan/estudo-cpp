#include <cstddef>
#include <iostream>

// 1. Declaração do template (sem corpo)
template <std::size_t N>
constexpr std::size_t factorial();

// 2. Especialização para 0
template <>
constexpr std::size_t factorial<0>()
{
    return 1;
}

// 3. Definição do template genérico
template <std::size_t N>
constexpr std::size_t factorial()
{
    return N * factorial<N - 1>();
}

int main()
{
    constexpr std::size_t result = factorial<5>();
    static_assert (result == 120, "O fatorial de 5 deve ser 120");
    std::cout << "O fatorial de 5 deve ser: " << result <<'\n';
    return 0;
}
