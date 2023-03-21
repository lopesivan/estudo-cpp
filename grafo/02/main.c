#include <stdio.h>

int main()
{
    // Cria a matriz de adjacências do grafo
    int grafo[3][3] = { {0, 1, 0}, {1, 0, 1}, {0, 1, 0} }; // O grafo tem 3 vértices

    // Imprime o grafo
    for (int i = 0; i < 3; i++) {
        printf("Vértice %d é adjacente a: ", i);
        for (int j = 0; j < 3; j++) {
            if (grafo[i][j] == 1) {
                printf("%d ", j);
            }
        }
        printf("\n");
    }

    return 0;
}

