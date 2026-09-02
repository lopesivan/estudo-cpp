#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define MAX_DIM 5

typedef double Scalar;

static void print_array(const double* array, size_t size)
{
    for(size_t i = 0; i < size; i++)
        printf("array[%zu] = %8.1f\n", i, array[i]);
}

void* criar_matriz(size_t num_dims, const size_t dims[])
{
    if(num_dims < 2 || num_dims > MAX_DIM)
        return NULL;

    size_t total_pointers     = 0;
    size_t current_layer_ptrs = 1;

    // 1. Calcula o total de ponteiros intermediarios ate a
    // penultima dimensao
    for(size_t i = 0; i < num_dims - 1; ++i)
    {
        current_layer_ptrs *= dims[i];
        total_pointers += current_layer_ptrs;
    }

    // 2. Calcula o total de elementos da matriz
    size_t total_elements = current_layer_ptrs * dims[num_dims - 1];

    // 3. Tamanhos e alinhamento de memoria
    size_t pointers_bytes = total_pointers * sizeof(void*);

    // Garante que o offset dos dados esteja alinhado para o tipo
    // Scalar
    size_t alignment = _Alignof(Scalar);
    size_t padding =
        (alignment - (pointers_bytes % alignment)) % alignment;
    size_t data_offset = pointers_bytes + padding;

    size_t data_bytes = total_elements * sizeof(Scalar);

    // 4. Alocacao unica
    void** layers = (void**)malloc(data_offset + data_bytes);
    if(!layers)
        return NULL;

    Scalar* data = (Scalar*)((char*)layers + data_offset);

    // 5. Encadeamento dos ponteiros das camadas
    void** current_layer = layers;
    void** next_layer    = layers + dims[0];
    size_t count         = 1;

    for(size_t d = 0; d < num_dims - 1; ++d)
    {
        count *=
            dims[d];  // Quantidade de ponteiros na camada atual

        if(d == num_dims - 2)
        {
            // Ultima camada de ponteiros -> aponta para os dados
            // (Scalar*)
            for(size_t i = 0; i < count; ++i)
            {
                current_layer[i] = data + (i * dims[d + 1]);
            }
        }
        else
        {
            // Camada intermediaria de ponteiros -> aponta para o
            // proximo nivel de ponteiros (void**)
            size_t step = dims[d + 1];
            for(size_t i = 0; i < count; ++i)
            {
                current_layer[i] = next_layer + (i * step);
            }
            current_layer = next_layer;
            next_layer += count * step;
        }
    }

    return layers;
}

void imprimir_matriz_3d(Scalar*** mat, const size_t dims[3])
{
    size_t fatias  = dims[0];
    size_t linhas  = dims[1];
    size_t colunas = dims[2];

    for(size_t i = 0; i < fatias; ++i)
    {
        printf("=== FATIA (PÁGINA) %zu ===\n", i);

        for(size_t j = 0; j < linhas; ++j)
        {
            printf("[ ");
            for(size_t k = 0; k < colunas; ++k)
            {
                // Acesso natural via sintaxe de colchetes 3D
                printf("%6.1f ", mat[i][j][k]);
            }
            printf("]\n");  // Quebra de linha ao fim de cada linha
                            // da tabela
        }
        printf("\n");  // Espaço em branco entre as fatias
    }
}


int main(void)
{
    size_t dims3D[3] = {
        2, 3, 4};  // Dimensao assimetrica 2x3x4 para teste rigoroso

    Scalar*** matriz3d = (Scalar***)criar_matriz(3, dims3D);
    if(!matriz3d)
    {
        printf("Erro ao alocar matriz3d\n");
        return 1;
    }

    // Populando a matriz
    for(size_t i = 0; i < dims3D[0]; ++i)
        for(size_t j = 0; j < dims3D[1]; ++j)
            for(size_t k = 0; k < dims3D[2]; ++k)
                matriz3d[i][j][k] = (Scalar)(100 * i + 10 * j + k);

    printf("===========> matriz3d[1][2][3] = %.1f\n\n",
           matriz3d[1][2][3]);

    for(size_t i = 0; i < dims3D[0]; ++i)
        for(size_t j = 0; j < dims3D[1]; ++j)
            for(size_t k = 0; k < dims3D[2]; ++k)
                printf("matriz3d[%ld][%ld][%ld] = %.1f\n\n",
                       i,
                       j,
                       k,
                       matriz3d[i][j][k]);


    // Acessando os dados de forma linear a partir do primeiro bloco
    Scalar* linear_data = **matriz3d;
    print_array(linear_data, dims3D[0] * dims3D[1] * dims3D[2]);

    // Chamada da função de impressão
    imprimir_matriz_3d(matriz3d, dims3D);

    free(matriz3d);
    return 0;
}
