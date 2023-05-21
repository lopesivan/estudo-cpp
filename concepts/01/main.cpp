#include <iostream>
#include <concepts>

// Definindo um concept para tipos numéricos
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Função template que usa o concept Numeric
template <Numeric T>
T square(T value) {
    return value * value;
}

int main() {
    int numInt = 5;
    double numDouble = 3.14;

    // Chamando a função template com tipos numéricos
    std::cout << "Quadrado de " << numInt << ": " << square(numInt) << std::endl;
    std::cout << "Quadrado de " << numDouble << ": " << square(numDouble) << std::endl;

    // Tentando chamar a função template com um tipo não numérico
    // Isso resultará em um erro de compilação
    // std::string str = "Hello";
    // std::cout << "Quadrado de " << str << ": " << square(str) << std::endl;

    return 0;
}

