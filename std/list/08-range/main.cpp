#include <iostream>
#include <list>
#include <numeric> // std::iota
#include <ranges>  // C++20 Ranges e Views

int main()
{
    std::list<int> lista (10);
    std::iota (lista.begin(), lista.end(), 1); // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    // Composição de Views usando o operador Pipe (|)
    auto resultado = lista
                     | std::views::filter ([] (int n)
    {
        return n % 2 == 0;
    }) // Filtra pares
    | std::views::transform ([] (int n)
    {
        return n * 10;
    })  // Multiplica por 10
    | std::views::take (2);                                // Pega só os 2 primeiros

    std::cout << "--- Resultado do View Preguiçoso ---\n";
    for (int num : resultado)
    {
        std::cout << num << "\n";
    }

    return 0;
}
