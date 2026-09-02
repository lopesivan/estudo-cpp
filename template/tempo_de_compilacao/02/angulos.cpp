#include <iostream>
#include <cmath>

// Constantes
constexpr double PI = 3.14159265358979323846;

// Template variable para converter graus para radianos
template<auto Graus>
constexpr auto Radianos = Graus * PI / 180.0;

// Template variable para converter radianos para graus
template<auto Rad>
constexpr auto Graus = Rad * 180.0 / PI;

int main()
{
    std::cout << "=== CONVERSÃO GRAUS ↔ RADIANOS ===\n\n";

    // Ângulos notáveis em graus
    std::cout << "0°   = " << Radianos<0> << " rad" << '\n';
    std::cout << "30°  = " << Radianos<30> << " rad" << '\n';
    std::cout << "45°  = " << Radianos<45> << " rad" << '\n';
    std::cout << "60°  = " << Radianos<60> << " rad" << '\n';
    std::cout << "90°  = " << Radianos<90> << " rad" << '\n';
    std::cout << "180° = " << Radianos<180> << " rad" << '\n';
    std::cout << "360° = " << Radianos<360> << " rad" << '\n';

    std::cout << "\nÂngulos em radianos para graus:\n";
    std::cout << "π/2  = " << Graus<PI/2> << "°" << '\n';
    std::cout << "π    = " << Graus<PI> << "°" << '\n';
    std::cout << "2π   = " << Graus<2*PI> << "°" << '\n';

    std::cout << "\nFunções trigonométricas:\n";
    std::cout << "cos(45°)  = " << cos (Radianos<45>) << '\n';
    std::cout << "sin(45°)  = " << sin (Radianos<45>) << '\n';

    return 0;
}
