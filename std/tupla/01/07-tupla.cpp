#include <tuple>
#include <iostream>
#include <type_traits>

template <class T>
void print_one (const T& val, int s)
{
    if constexpr (std::is_same_v<T, char>)
    {
        std::cout << val << " ";
    }
    else
    {
        std::cout << (val * s) << " ";
    }
}

template <class Tuple, size_t... I>
void print_impl (const Tuple& t, int s, std::index_sequence<I...>)
{
    (print_one (std::get<I> (t), s), ...);
}

template <class... Args>
void print_tuple (const std::tuple<Args...>& t, int s)
{
    print_impl (t, s, std::index_sequence_for<Args...> {});
}

int main()
{
    std::tuple<int, double, char> t = {1, 3.14, 'x'};
    print_tuple (t, 10);
    std::cout << std::endl;
}
