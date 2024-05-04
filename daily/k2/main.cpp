#include <iostream>
#include <string>

struct HelloWorld {
    std::string mensagem;

    // Construtor que aceita um `std::string`
    HelloWorld(std::string mensagem) : mensagem(mensagem) {}

    // Construtor que aceita um `const char*` (string literal)
    HelloWorld(const char* mensagem) : mensagem(mensagem) {}

    // Método show
    void show() const {
        std::cout << "mensagem: " << this->mensagem << std::endl;
    }
};

int main() {
    // Exemplo de uso com string literal
    HelloWorld hw = "Olá, Mundo!";
    hw.show();

    return 0;
}
