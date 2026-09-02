#include <algorithm> // find
#include <iostream>
#include <numeric>  // para std::iota
#include <list>
#include <ranges>  // C++20 Ranges e Views

int main()
{
    // std::list<int> lista (10);
    // int n = 1;
    // std::generate (lista.begin(), lista.end(), [&n]()
    // {
    //     return n++;
    // });

    std::list<int> lista (10);
    std::iota (lista.begin(), lista.end(), 1);

    // Imprime os 3 primeiros elementos diretamente de forma declarativa:
    for (int val : lista | std::views::take (3))
    {
        std::cout << val << "\n";
    }

    return 0;
}
