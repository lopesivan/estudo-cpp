#include <iostream>
#include <numeric>
#include <cmath>
#include <vector>

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

// Regra do trapezio composta, agora via std::accumulate:
// soma_interna = f(a+h) + f(a+2h) + ... + f(a+(n-1)h)
double trapezoidal_accumulate (double a, double b, int n, double (*fn) (double))
{
    double h = (b - a) / n;

    // indices 1..n-1 (pontos internos)
    std::vector<int> indices (n - 1);
    std::iota (indices.begin(), indices.end(), 1); // 1,2,...,n-1

    double soma_interna = std::accumulate (
                              indices.begin(), indices.end(), 0.0,
                              [&] (double acc, int i)
    {
        return acc + fn (a + i * h);
    });

    return h * (0.5 * fn (a) + soma_interna + 0.5 * fn (b));
}

double f (double x)
{
    return std::sin (x);
}

int main()
{
    double a = 0.0, b = M_PI;
    double exata = 2.0;
    double eps = 1e-8;
    int n0 = 4;

    // distance decide QUANTOS refinamentos (contagem, nao soma)
    double T_n0  = trapezoidal_accumulate (a, b, n0, f);
    double T_2n0 = trapezoidal_accumulate (a, b, 2 * n0, f);
    double e0 = std::fabs (T_2n0 - T_n0) / 3.0;

    auto reduzir_erro = [] (double e)
    {
        return e / 4.0;
    };
    int duplicacoes = distance<int> (e0, eps, reduzir_erro);
    int n_final = n0 * (1 << duplicacoes);

    // accumulate faz a SOMA de verdade (a integral em si)
    double resultado = trapezoidal_accumulate (a, b, n_final, f);
    double erro_real = std::fabs (resultado - exata);

    std::cout << "n final de subintervalos = " << n_final << '\n';
    std::cout << "integral (via accumulate) = " << resultado << '\n';
    std::cout << "integral exata             = " << exata << '\n';
    std::cout << "erro real                  = " << erro_real << '\n';
}
