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

    std::cout << std::get<0> (misto) << " "
              << std::get<1> (misto) << " "
              << std::get<2> (misto) << " "
              << std::get<3> (misto) << std::endl;

    return 0;
}
