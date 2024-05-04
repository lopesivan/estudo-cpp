#include <iostream>

struct Carro {
  Carro(float v) : v(v) {}
  float v;
};

int main(int argc, char *argv[]) {
  Carro carro = 300;

  std::cout << carro.v << '\n';
  return 0;
}
