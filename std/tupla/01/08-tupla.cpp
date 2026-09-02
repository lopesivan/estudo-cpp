#include <tuple>
#include <iostream>
#include <type_traits>

template <class T>
T transform_one (const T& val, int s)
{
    if constexpr (std::is_same_v<T, char>)
    {
        return val; // char só passa, sem multiplicar
    }
    else
    {
        return val * s;
    }
}

template <class Tuple, size_t... I>
auto transform_impl (const Tuple& t, int s, std::index_sequence<I...>)
{
    return std::make_tuple (transform_one (std::get<I> (t), s)...);
}

template <class... Args>
auto transform_tuple (const std::tuple<Args...>& t, int s)
{
    return transform_impl (t, s, std::index_sequence_for<Args...> {});
}

int main()
{
    std::tuple<int, double, char> t = {1, 3.14, 'x'};

    // gera uma nova tupla com os valores transformados
    auto resultado = transform_tuple (t, 17);

    // carrega em variáveis separadas via structured bindings
    auto [a, b, c] = resultado;

    std::cout << "a=" << a << " b=" << b << " c=" << c << std::endl;

    return 0;
}
