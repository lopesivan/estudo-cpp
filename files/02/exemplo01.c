#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    char telefone[15];
} Pessoa;

int main() {
    Pessoa p = {"João Silva", "1234-5678"};
    
    FILE *file = fopen("dados.bin", "wb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    
    if (fwrite(&p, sizeof(Pessoa), 1, file) != 1) {
        perror("Erro ao escrever no arquivo");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    return 0;
}
