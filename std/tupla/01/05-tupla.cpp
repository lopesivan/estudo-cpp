#include <tuple>
#include <iostream>

int main()
{
    std::tuple<int, double, char> t = {1, 3.14, 'x'};

    std::apply ([] (auto&& ... args)
    {
        ((std::cout << args << " "), ...); // fold expression, C++17
    }, t);

    std::cout << std::endl;
}
