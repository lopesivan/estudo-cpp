#include <iostream>
#include <list>
#include <numeric> // std::iota
#include <ranges>  // C++20 Ranges e Views

int main()
{
    std::list<int> lista (10);
    std::iota (lista.begin(), lista.end(), 1); // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    // Pega os 3 últimos e os imprime na ordem inversa (do 10 para o 8)
    std::cout << "--- Os 3 últimos (Ordem Inversa: 10, 9, 8) ---\n";
    for (int val : lista | std::views::reverse | std::views::take (3))
    {
        std::cout << val << "\n";
    }

    std::cout << "\n";

    // Pega os 3 últimos e desfaz a inversão (Ordem Original: 8, 9, 10)
    std::cout << "--- Os 3 últimos (Ordem Original: 8, 9, 10) ---\n";
    for (int val : lista | std::views::reverse | std::views::take (3) | std::views::reverse)
    {
        std::cout << val << "\n";
    }

    return 0;
}
