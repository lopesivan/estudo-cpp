#include "iterator_traits.hpp"
#include <iostream>
#include <numeric>   // std::iota
#include <cstdio>    // printf
//
//
#include "matrix2d.hpp"

int main()
{
    Matrix2D<double> mat (3, 4);

    // 1. Popular a matriz inteira usando algoritmo da STL (requer iterador)
    std::iota (mat.begin(), mat.end(), 12.0);

    // 2. Imprimir via ponteiro linear contíguo com printf
    std::cout << "=== Matriz Inicial (3x4) ===\n";
    double* ptr = mat.begin();
    size_t total_elements = mat.rows() * mat.cols();
    for (size_t i = 0; i < total_elements; ++i)
    {
        std::printf ("%.1f\t", *ptr++);
        if ((i + 1) % mat.cols() == 0) std::printf ("\n");
    }

    // 3. Uso do custom::distance
    std::cout << "\nTotal de elementos (custom::distance): "
              << custom::distance (mat.begin(), mat.end()) << "\n";

    // 4. Uso do custom::advance para navegar até um elemento
    auto it_first = mat.begin();
    auto it_target = mat.begin();
    custom::advance (it_target, 6); // Avança até o elemento no índice 6 (Linha 1, Col 2)

    std::cout << "Elemento na posicao linear 0: " << *it_first << "\n";
    std::cout << "Elemento na posicao linear 6: " << *it_target << "\n";

    // 5. Uso do custom::iter_swap para trocar os valores apontados
    std::cout << "\nTrocando posicao linear 0 com posicao linear 6...\n";
    custom::iter_swap (it_first, it_target);

    // 6. Reimprimir para confirmar a alteração
    std::cout << "=== Matriz Apos custom::iter_swap ===\n";
    ptr = mat.begin();
    for (size_t i = 0; i < total_elements; ++i)
    {
        std::printf ("%.1f\t", *ptr++);
        if ((i + 1) % mat.cols() == 0) std::printf ("\n");
    }

    return 0;
}
