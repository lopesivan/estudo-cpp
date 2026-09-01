#include "iterator_traits.hpp"
#include <iostream>
#include <cstdlib>

// Sua função alocadora em C
double** new_2d (const unsigned long dim1, const unsigned long dim2)
{
    size_t layer_1_size = dim1 * sizeof (double*);
    size_t data_size    = dim1 * dim2 * sizeof (double);

    unsigned char* raw_bytes = (unsigned char*)malloc (layer_1_size + data_size);
    if (raw_bytes == NULL) return NULL;

    double** array2d = (double**)raw_bytes;
    for (unsigned int i = 0; i < dim1; ++i)
    {
        array2d[i] = (double*) (array2d + dim1) + i * dim2;
    }
    return array2d;
}

// Funções auxiliares para extrair ponteiros contíguos dos dados
double* matrix_begin (double** matrix)
{
    return matrix[0]; // Ponteiro para o primeiro elemento real do buffer
}

double* matrix_end (double** matrix, size_t dim1, size_t dim2)
{
    return matrix[0] + (dim1 * dim2); // Ponteiro logo após o último elemento
}

int main()
{
    size_t rows = 3, cols = 4;
    double** matrix = new_2d (rows, cols);

    if (!matrix) return 1;

    // Preenchendo via sintaxe 2D padrão em C
    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            matrix[r][c] = (r + 1) * 10.0 + c;
        }
    }

    // --- Aplicando nossa técnica com custom::advance e custom::distance ---
    double* first = matrix_begin (matrix);
    double* last = matrix_end (matrix, rows, cols);

    std::cout << "Elementos totais (custom::distance): "
              << custom::distance (first, last) << "\n"; // Usa especialização T* (O(1))

    double* it = first;
    custom::advance (it, 6); // Avança 6 posições linearmente na matriz 2D
    std::cout << "Elemento no indice linear 6 (linha 1, col 2): " << *it << "\n";

    free (matrix);
    return 0;
}
