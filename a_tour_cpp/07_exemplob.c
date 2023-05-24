#include <stdio.h>

void imprimirVetor(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

int main() {
    int v[10] = {77, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int tamanho = sizeof(v) / sizeof(v[0]);

    imprimirVetor(v, tamanho);

    return 0;
}
