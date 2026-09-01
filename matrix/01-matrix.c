#include <stddef.h>  // Para size_t
#include <stdint.h>  // SIZE_MAX
#include <stdio.h>
#include <stdlib.h>

double** new_2d(size_t dim1, size_t dim2);

int main(void)
{
    size_t   rows = 3, cols = 3;
    double** matrix = new_2d(rows, cols);

    if(matrix == NULL)
    {
        printf("Erro ao alocar memória\n");
        return 1;
    }

    // Preenchendo a matriz com valores que indicam a posição
    for(size_t i = 0; i < rows; i++)
    {
        for(size_t j = 0; j < cols; j++)
        {
            matrix[i][j] =
                i * 10.0 + j;  // a00=0.0, a01=1.0, a02=2.0
                               // a10=10.0, a11=11.0, a12=12.0
                               // a20=20.0, a21=21.0, a22=22.0
        }
    }

    // Imprimindo a matriz
    printf("%s\n", "--------------------");
    printf("%s:\n", "Imprimindo a matriz");
    printf("%s\n", "--------------------");
    for(size_t i = 0; i < rows; i++)
    {
        for(size_t j = 0; j < cols; j++)
        {
            printf("matrix[%ld][%ld] = %f\n", i, j, matrix[i][j]);
        }
    }

    // Imprimindo a matriz
    printf("%s\n", "--------------------");
    printf("%s:\n", "Imprimindo a matriz");
    printf("%s\n", "--------------------");
    for(size_t i = 0; i < rows * cols; i++)
    {
        size_t row = i / cols;  // Calcula a linha
        size_t col = i % cols;  // Calcula a coluna
        /*     i / cols dá a linha  (ex: 0,0,0,1,1,1,2,2,2) */
        /*     i % cols dá a coluna (ex: 0,1,2,0,1,2,0,1,2) */
        /*     *(matrix[0] + i) acessa o elemento na posição i do
         * array contíguo
         */

        printf("matrix[%zu][%zu] = %.1f\n",
               row,
               col,
               *(matrix[0] + i));
    }

    // Versão ainda mais rápida (acesso sequencial):
    printf("%s\n", "--------------------");
    printf("%s:\n", "Imprimindo a matriz");
    printf("%s:\n", "Versão ainda mais rápida (acesso sequencial)");
    printf("%s\n", "--------------------");
    double* ptr = matrix[0];
    for(size_t i = 0; i < rows * cols; i++)
    {
        printf("%.1f ", *ptr++);
        if((i + 1) % cols == 0)
            printf("\n");
    }


    free(matrix);
    return 0;
}

// cria uma matriz 2D
double** new_2d(size_t dim1, size_t dim2)
{
    if(dim1 == 0 || dim2 == 0)
        return NULL;

    if(dim1 > SIZE_MAX / sizeof(double*))
        return NULL;

    if(dim2 > SIZE_MAX / dim1)
        return NULL;

    size_t number_of_elements = dim1 * dim2;

    if(number_of_elements > SIZE_MAX / sizeof(double))
        return NULL;

    size_t pointers_size = dim1 * sizeof(double*);
    size_t data_size     = number_of_elements * sizeof(double);

    if(pointers_size > SIZE_MAX - data_size)
        return NULL;

    double** matrix = malloc(pointers_size + data_size);

    if(matrix == NULL)
        return NULL;

    double* data =
        (double*)((unsigned char*)matrix + pointers_size);

    for(size_t i = 0; i < dim1; ++i)
        matrix[i] = data + i * dim2;

    return matrix;
}
