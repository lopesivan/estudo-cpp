// Inclui várias bibliotecas da STL usadas para algoritmos,
// funções e containers.
#include <algorithm>   // all_of, any_of, none_of, copy
#include <functional>  // std::bind, std::modulus
#include <iostream>    // std::cout
#include <iterator>    // std::ostream_iterator
#include <numeric>     // std::partial_sum
#include <vector>      // std::vector


namespace ranges = std::ranges;
// * Isso cria um **atalho** para `std::ranges` (introduzido no C++20),
// * Permite usar `ranges::all_of`, `ranges::none_of` etc.,
// que são versões **mais seguras e poderosas** dos algoritmos
// tradicionais.

/*
 *  * Função genérica `some_of`: verifica se **alguns**, mas
 *  **não todos**, os elementos de `r` satisfazem o predicado
 *  `pred`.
 *  * Ela retorna `true` se:
 *
 *  * **Nem todos** satisfazem (`!all_of`)
 *  * **E pelo menos um** satisfaz (negando `none_of`)
 *  * O uso de `constexpr` permite usar essa função **em tempo de compilação**.
 */
constexpr bool some_of (auto&& r, auto&& pred) // some but not all
{
    return not (ranges::all_of (r, pred) or ranges::none_of (r, pred));
}

constexpr auto w = {1, 2, 3};

static_assert (!some_of (w, [] (int x)
{
    return x < 1;
}));

static_assert ( some_of (w, [] (int x)
{
    return x < 2;
}));

static_assert (!some_of (w, [] (int x)
{
    return x < 4;
}));

int main()
{
    std::vector<int> v (10, 2);
    std::partial_sum (v.cbegin(), v.cend(), v.begin());
    std::cout << "Among the numbers: ";
    ranges::copy (v, std::ostream_iterator<int> (std::cout, " "));
    std::cout << '\n';

    if (ranges::all_of (v.cbegin(), v.cend(), [] (int i)
{
    return i % 2 == 0;
}))
    std::cout << "All numbers are even\n";

    if (ranges::none_of (v, std::bind (std::modulus<int>(), std::placeholders::_1, 2)))
        std::cout << "None of them are odd\n";

    auto DivisibleBy = [] (int d)
    {
        return [d] (int m)
        {
            return m % d == 0;
        };
    };

    if (ranges::any_of (v, DivisibleBy (7)))
        std::cout << "At least one number is divisible by 7\n";
}
