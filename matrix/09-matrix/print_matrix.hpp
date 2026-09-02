#ifndef PRINT_MATRIX_HPP
#define PRINT_MATRIX_HPP

#include <iostream>
#include <vector>

template <typename T>
void print_nd_recursive (const MatrixND<T>& mat, std::vector<size_t>& indices, size_t current_dim)
{
    size_t num_dims = mat.num_dimensions();

    // CASO BASE: Chegamos na última dimensão (imprime uma linha entre colchetes)
    if (current_dim == num_dims - 1)
    {
        std::cout << "[ ";
        for (size_t i = 0; i < mat.dim (current_dim); ++i)
        {
            indices[current_dim] = i;
            size_t offset = mat.calculate_offset (indices);

            // Acessa o elemento exato no bloco contíguo de memória
            std::cout << * (mat.begin() + offset) << "\t";
        }
        std::cout << "]\n";
        return;
    }

    // PASSO RECURSIVO: Percorre a dimensão atual
    for (size_t i = 0; i < mat.dim (current_dim); ++i)
    {
        indices[current_dim] = i;

        // Imprime separadores visuais para fatias superiores
        if (current_dim == 0 && i > 0)
        {
            std::cout << "\n--- FATIA " << i << " ---\n";
        }

        print_nd_recursive (mat, indices, current_dim + 1);
    }
}

// Wrapper público para facilitar a chamada
template <typename T>
void print_matrix (const MatrixND<T>& mat)
{
    std::vector<size_t> indices (mat.num_dimensions(), 0);
    print_nd_recursive (mat, indices, 0);
    std::cout << "\n";
}

#endif      // PRINT_MATRIX_HPP
