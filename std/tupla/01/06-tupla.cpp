#include <tuple>
#include <iostream>

template <class Tuple, size_t... I>
void print_impl (const Tuple& t, std::index_sequence<I...>)
{
    ((std::cout << std::get<I> (t) << " "), ...);
}

template <class... Args>
void print_tuple (const std::tuple<Args...>& t)
{
    print_impl (t, std::index_sequence_for<Args...> {});
}

int main()
{
    std::tuple<int, double, char> t = {1, 3.14, 'x'};
    print_tuple (t);

    // std::apply ([] (auto&& ... args)
    // {
    //     ((std::cout << args << " "), ...); // fold expression, C++17
    // }, t);

    std::cout << std::endl;
}
