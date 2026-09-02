#include <iostream>

// Template que calcula o dobro em tempo de compilação
template<int N>
struct Dobro
{
    static constexpr int valor = N * 2;
};

int main()
{
    // Usando o template com diferentes valores
    std::cout << "Dobro de 5: " << Dobro<5>::valor << std::endl;    // 10
    std::cout << "Dobro de 10: " << Dobro<10>::valor << std::endl;  // 20
    std::cout << "Dobro de 7: " << Dobro<7>::valor << std::endl;    // 14

    return 0;
}
