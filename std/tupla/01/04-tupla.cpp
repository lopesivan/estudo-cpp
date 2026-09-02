#include <tuple>
#include <iostream>

int main()
{
    std::tuple<int, double, char, bool, int> t = {1, 3.14, 'x', true, 99};

    auto [a, b, c, d, e] = t;
    std::cout << a << " " << b << " " << c << " " << d << " " << e << std::endl;

    // tupla vazia também é válida
    std::tuple<> vazia;

    // tupla com 1 elemento
    std::tuple<int> um{42};

    return 0;
}
