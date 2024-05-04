
#include <iostream> // Para std::cout e std::endl
#include <string>   // Para std::string

struct HelloWorld {
  std::string mensagem;

  // Construtor
  HelloWorld(std::string mensagem) : mensagem(mensagem) {}

  // Método show
  void show() const {
    std::cout << "mensagem: " << this->mensagem << std::endl;
  }
};

int main() {
  // Exemplo de uso da estrutura
  HelloWorld hw("Olá, Mundo!");
  hw.show();

  return 0;
}
