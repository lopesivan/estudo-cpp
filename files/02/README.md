Claro, aqui está um exemplo de código em C++ que lê um arquivo e usa um bloco de tratamento de exceções para lidar com erros, como a inexistência do arquivo:

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

int main() {
    std::string filename = "exemplo.txt";
    std::ifstream file;

    try {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir o arquivo: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }

        file.close();
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

**Explicação do código:**

1. **Inclusão de Bibliotecas**: `iostream`, `fstream` e `string` são usadas para operações de entrada/saída e manipulação de strings. `stdexcept` é usada para tratar exceções padrão em C++.

2. **Abertura do Arquivo**: O arquivo é tentado ser aberto com `file.open(filename)`. Se o arquivo não puder ser aberto (`!file.is_open()`), uma exceção do tipo `std::runtime_error` é lançada com uma mensagem apropriada.

3. **Leitura do Arquivo**: Se o arquivo for aberto com sucesso, o programa lê o arquivo linha por linha usando `std::getline(file, line)` e imprime cada linha na saída padrão.

4. **Tratamento de Exceções**: O bloco `catch` captura exceções do tipo `std::runtime_error` e imprime a mensagem de erro usando `std::cerr`.

5. **Fechamento do Arquivo**: O arquivo é fechado com `file.close()` depois de ser lido.

Este código é básico e adequado para demonstrar o uso de exceções em operações de arquivo em C++.
