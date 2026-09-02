#include <iostream>
#include <cmath>

template <class N, class X, class F>
N distance(X x, X y, F f)
{
    N n = N(0);
    while (x > y)
    {
        x = f(x);
        n = n + N(1);
    }
    return n;
}

// Regra do trapezio composta com n subintervalos
double trapezoidal(double a, double b, int n, double (*fn)(double))
{
    double h = (b - a) / n;
    double sum = 0.5 * (fn(a) + fn(b));
    for (int i = 1; i < n; ++i)
        sum += fn(a + i * h);
    return sum * h;
}

double f(double x) { return std::sin(x); }

int main()
{
    double a = 0.0, b = M_PI; // integral exata de sin(x) de 0 a pi = 2.0
    double exata = 2.0;
    double eps = 1e-8;

    int n0 = 4; // ponto de partida

    // Estimativa inicial do erro via Richardson: erro(n0) ~ |T(2n0) - T(n0)| / 3
    double T_n0  = trapezoidal(a, b, n0, f);
    double T_2n0 = trapezoidal(a, b, 2 * n0, f);
    double e0 = std::fabs(T_2n0 - T_n0) / 3.0;

    std::cout << "T(n=" << n0 << ")  = " << T_n0 << '\n';
    std::cout << "T(n=" << 2*n0 << ")  = " << T_2n0 << '\n';
    std::cout << "erro estimado inicial e0 = " << e0 << '\n';

    // Erro da regra do trapezio eh O(h^2): dobrar n divide o erro por ~4
    auto reduzir_erro = [](double e) { return e / 4.0; };

    // Quantas duplicacoes de n sao necessarias ate erro cair abaixo de eps?
    int duplicacoes = distance<int>(e0, eps, reduzir_erro);

    int n_final = n0 * (1 << duplicacoes); // n0 * 2^duplicacoes

    std::cout << "duplicacoes necessarias  = " << duplicacoes << '\n';
    std::cout << "n final de subintervalos = " << n_final << '\n';

    double resultado = trapezoidal(a, b, n_final, f);
    double erro_real = std::fabs(resultado - exata);

    std::cout << "integral calculada       = " << resultado << '\n';
    std::cout << "integral exata            = " << exata << '\n';
    std::cout << "erro real                 = " << erro_real << '\n';
    std::cout << "eps alvo                  = " << eps << '\n';
}
