#include <algorithm> // find
#include <iostream>
#include <numeric>  // para std::iota
#include <list>
#include <algorithm> // std::min, std::max


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

    auto it  = lista.begin();
    auto end = lista.begin();
    //
    // Usando std::min com especificação explícita do template:
    std::advance (end, std::min<size_t> (3, lista.size()));

    for (; it!=end; it++)
        std::cout << *it << '\n';

    return 0;
}
