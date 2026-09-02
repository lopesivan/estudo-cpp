#include <iostream>
#include <cmath>

template <class N, class X, class F>
N distance (X x, X y, F f)
{
    N n = N (0);
    while (x > y)
    {
        x = f (x);
        n = n + N (1);
    }
    return n;
}

int main()
{
    double theta = 1.2;     // angulo em radianos (~68.7 graus, nao eh pequeno)
    double eps   = 1e-4;    // tolerancia: abaixo disso, sin(x) ~= x

    auto bissectar = [] (double t)
    {
        return t / 2.0;
    };

    // 1) Conta quantas bisseccoes levam theta abaixo de eps
    int n = distance<int> (theta, eps, bissectar);
    double theta_pequeno = theta / std::pow (2.0, n);

    std::cout << "n = " << n << " biseccoes\n";
    std::cout << "theta_pequeno = " << theta_pequeno << " rad\n";

    // 2) Aproximacao de angulo pequeno no fundo
    double s = theta_pequeno;               // sin(x) ~= x
    double c = std::sqrt (1 - s * s);       // cos(x) = sqrt(1 - sin^2 x)

    // 3) Reconstroi subindo n vezes com a formula do angulo duplo
    for (int i = 0; i < n; ++i)
    {
        double s2 = 2 * s * c;
        double c2 = 1 - 2 * s * s;
        s = s2;
        c = c2;
    }

    std::cout << "sin(theta) calculado = " << s << '\n';
    std::cout << "sin(theta) std::sin  = " << std::sin (theta) << '\n';
    std::cout << "erro absoluto        = " << std::fabs (s - std::sin (theta)) << '\n';
}
