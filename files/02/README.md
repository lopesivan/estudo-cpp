Salvar informações em formato binário permite que você mantenha os dados em um formato mais compacto e usualmente mais rápido para leitura e escrita, comparado com o formato texto. Vou mostrar como você pode fazer isso tanto em C quanto em C++.

### Salvando Dados em Binário em C

Em C, você usará as funções `fwrite` e `fopen` para gravar dados em formato binário. Aqui está um exemplo que salva o nome e o telefone:

```c
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
```

**Explicação:**

- **Estrutura `Pessoa`**: Define uma estrutura com `nome` e `telefone`.
- **`fopen`**: Abre um arquivo em modo binário de escrita (`wb`).
- **`fwrite`**: Escreve a estrutura no arquivo. Se não conseguir escrever exatamente um elemento, indica erro.
- **`fclose`**: Fecha o arquivo.

### Salvando Dados em Binário em C++

Em C++, você usará o fluxo de arquivo do `<fstream>` com o modo binário. Aqui está um exemplo:

```cpp
#include <iostream>
#include <fstream>
#include <string>

struct Pessoa {
    std::string nome;
    std::string telefone;
};

int main() {
    Pessoa p = {"João Silva", "1234-5678"};
    
    std::ofstream file("dados.bin", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo" << std::endl;
        return 1;
    }
    
    size_t nomeSize = p.nome.size();
    file.write(reinterpret_cast<char*>(&nomeSize), sizeof(nomeSize));
    file.write(p.nome.c_str(), nomeSize);
    
    size_t telefoneSize = p.telefone.size();
    file.write(reinterpret_cast<char*>(&telefoneSize), sizeof(telefoneSize));
    file.write(p.telefone.c_str(), telefoneSize);
    
    file.close();
    return 0;
}
```

**Explicação:**

- **Estrutura `Pessoa`**: Usa `std::string` para `nome` e `telefone`.
- **`std::ofstream`**: Abre o arquivo em modo binário (`std::ios::binary`).
- **`write`**: Escreve o tamanho e os dados das strings no arquivo. Isso é necessário porque `std::string` não tem tamanho fixo e você precisa saber quantos bytes ler ao carregar os dados.
- **`close`**: Fecha o arquivo.

Nesses exemplos, os dados são gravados de forma binária, o que significa que você precisará de um processo correspondente para ler esses dados de volta de maneira adequada.
