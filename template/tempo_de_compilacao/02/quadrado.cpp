#include <iostream>

template<int N>
struct Quadrado
{
    static constexpr int valor = N * N;
};

int main()
{
    std::cout << "Quadrado de 4: " << Quadrado<4>::valor << std::endl;   // 16
    std::cout << "Quadrado de 7: " << Quadrado<7>::valor << std::endl;   // 49
    std::cout << "Quadrado de 10: " << Quadrado<10>::valor << std::endl; // 100

    return 0;
}
