#include <iostream>
#include <numeric>

// collision_point (EoP cap.2): tartaruga e lebre.
// Detecta o ponto onde a orbita de x sob f "colide" (entra em ciclo),
// sem precisar conhecer o tamanho do ciclo de antemao.
template <class X, class F>
X collision_point (X x, F f)
{
    X slow = x;
    X fast = x;
    do
    {
        slow = f (slow);         // 1 passo
        fast = f (f (fast));     // 2 passos
    }
    while (slow != fast);
    return slow; // ponto de colisao (dentro do ciclo)
}

// Pollard's rho: usa collision_point sobre f(x) = (x^2 + c) mod n
// para achar um fator nao trivial de n.
long long pollard_rho (long long n, long long c = 1)
{
    auto f = [n, c] (long long x)
    {
        return (x * x + c) % n;
    };

    long long x = collision_point (2LL, f);

    // A partir do ponto de colisao, usamos gcd para extrair o fator
    long long slow = 2, fast = 2, d = 1;
    while (d == 1)
    {
        slow = f (slow);
        fast = f (f (fast));
        d = std::gcd (std::abs (slow - fast), n);
    }
    return d; // fator nao trivial (ou n, se falhar -- tenta outro c)
}

int main()
{
    long long n = 8051; // 8051 = 83 * 97 (numero real a fatorar)

    long long fator = pollard_rho (n);

    std::cout << "n = " << n << '\n';
    std::cout << "fator encontrado = " << fator << '\n';
    std::cout << "n / fator = " << n / fator << '\n';
    std::cout << "verificacao: " << fator << " * " << (n / fator)
              << " = " << fator * (n / fator) << '\n';
}
