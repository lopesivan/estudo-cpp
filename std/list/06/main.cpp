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

    auto it = lista.begin();
    auto size = lista.size();
    //
    // Usando std::min com especificação explícita do template:
    std::advance (it,
                  // std::max: retorna o maior valor entre dois argumentos
                  // <size_t>: especifica o tipo dos argumentos (tipo sem sinal para tamanhos)
                  // Argumentos:
                  // - 0: valor mínimo garantido
                  // - size - 3: tamanho da lista menos 3
                  std::max<size_t> (0, size -3));

    for (; it!=lista.end(); it++)
        std::cout << *it << '\n';

    return 0;
}
