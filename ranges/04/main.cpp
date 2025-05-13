#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges> // <- necessário para std::views
#include <vector>

int main()
{
    std::vector<int> v(10, 2); // vetor com dez elementos, todos iguais a 2

    std::cout << "Valores: ";
    for (auto x : v | std::views::transform([](auto x) { return x * 2; }))
    {
        std::cout << x << " ";
    }

    std::cout << '\n';
}
