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
