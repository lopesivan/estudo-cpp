#include <tuple>
#include <iostream>

std::tuple<int, int, int> get_color()
{
    return std::make_tuple (255, 128, 0);
}

int main()
{
    auto [r, g, b] = get_color();

    std::cout << "r=" << r << " g=" << g << " b=" << b << std::endl;

    return 0;
}
