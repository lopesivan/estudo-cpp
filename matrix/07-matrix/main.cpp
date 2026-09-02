#include <iostream>
#include <numeric>   // std::iota
#include <cstdio>    // printf
//
//
#include "matrix2d.hpp"

void print_matrix (const Matrix2D<double>& mat)
{
    const double* ptr = mat.begin();
    size_t total = mat.rows() * mat.cols();
    for (size_t i = 0; i < total; ++i)
    {
        std::printf ("%.1f\t", *ptr++);
        if ((i + 1) % mat.cols() == 0) std::printf ("\n");
    }
    std::cout << "\n";
}

void print_matrix2 (const Matrix2D<double>& mat)
{
    for (size_t r = 0; r < mat.rows(); ++r)
    {
        for (size_t c = 0; c < mat.cols(); ++c)
        {
            std::printf ("%.1f\t", mat[r][c]); // Chama operator[] const
        }
        std::printf ("\n");
    }
    std::cout << "\n";
}

int main()
{
    Matrix2D<double> mat (3, 4);
    std::iota (mat.begin(), mat.end(), 1.0);

    std::cout << "=== Matriz Original (3x4) ===\n";
    print_matrix (mat);

    // Troca a Linha 0 com a Linha 2
    std::cout << "=== Apos trocar Linha 0 com Linha 2 ===\n";
    mat.swap_rows ( 0, 2);
    print_matrix (mat);

    // Troca a Coluna 1 com a Coluna 3
    std::cout << "=== Apos trocar Coluna 1 com Coluna 3 ===\n";
    mat.swap_cols (1, 3);
    print_matrix (mat);

    // // Troca a Linha 1 com a Linha 2 -- fast
    // // nao usar:
    // NOTE: Importante: Se você usar swap_rows_fast, a sua matriz
    // perde a propriedade de ter linhas fisicamente ordenadas na
    // memória contígua. Isso fará com que algoritmos que dependem
    // de begin() e end() sequenciais (como custom::advance,
    // std::iota ou std::sort) produzam resultados incorretos. Para
    // manter a matriz compatível com a STL e iteradores, o correto
    // é usar a versão com custom::iter_swap.
    //
    // std::cout << "=== Apos trocar Linha 1 com Linha 2 -- fast
    // ===\n"; mat.swap_rows_fast (1,2); print_matrix (mat);

    return 0;
}
