#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// ---------------------------------------------------------------------
// collision_point (EoP cap.2): tartaruga e lebre.
// Detecta o ponto onde a orbita de x sob f "colide" (entra em ciclo).
// ---------------------------------------------------------------------
template <class X, class F>
X collision_point(X x, F f)
{
    X slow = x;
    X fast = x;
    do
    {
        slow = f(slow);
        fast = f(f(fast));
    } while (slow != fast);
    return slow;
}

// ---------------------------------------------------------------------
// Teste de primalidade simples (trial division) -- suficiente para
// os fatores intermediarios que aparecem apos o pollard_rho.
// ---------------------------------------------------------------------
bool eh_primo(long long n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (long long p = 3; p * p <= n; p += 2)
        if (n % p == 0) return false;
    return true;
}

// ---------------------------------------------------------------------
// Pollard's rho: acha UM fator nao trivial de n (nao a fatoracao toda).
// Usa collision_point sobre f(x) = (x^2 + c) mod n.
// Retorna n se falhar com esse c (chamador deve tentar outro c).
// ---------------------------------------------------------------------
long long pollard_rho(long long n, long long c)
{
    if (n % 2 == 0) return 2;

    auto f = [n, c](long long x) { return ((__int128)x * x + c) % n; };

    long long slow = 2, fast = 2, d = 1;
    while (d == 1)
    {
        slow = f(slow);
        fast = f(f(fast));
        d = std::gcd(std::abs(slow - fast), n);
    }
    return d; // pode ser n (falha) ou um fator nao trivial
}

// ---------------------------------------------------------------------
// Acha um fator nao trivial de n, tentando varios valores de c
// caso pollard_rho falhe (retorne n) ou n seja primo.
// ---------------------------------------------------------------------
long long achar_fator(long long n)
{
    if (n % 2 == 0) return 2;
    if (eh_primo(n)) return n;

    for (long long c = 1; c < 20; ++c)
    {
        long long d = pollard_rho(n, c);
        if (d != n && d != 1) return d;
    }
    return n; // desistiu (nao deveria acontecer para n pequenos/medios)
}

// ---------------------------------------------------------------------
// Fatoracao completa: decompoe n recursivamente em primos, usando
// achar_fator (que por baixo usa collision_point) em cada etapa.
// ---------------------------------------------------------------------
void fatorar(long long n, std::vector<long long>& fatores)
{
    if (n == 1) return;
    if (eh_primo(n)) { fatores.push_back(n); return; }

    long long d = achar_fator(n);
    fatorar(d, fatores);
    fatorar(n / d, fatores);
}

int main()
{
    std::vector<long long> numeros = { 8051, 58089, 1000003, 999999999989LL };

    for (long long n : numeros)
    {
        std::vector<long long> fatores;
        fatorar(n, fatores);
        std::sort(fatores.begin(), fatores.end());

        std::cout << n << " = ";
        long long produto = 1;
        for (size_t i = 0; i < fatores.size(); ++i)
        {
            std::cout << fatores[i];
            if (i + 1 < fatores.size()) std::cout << " x ";
            produto *= fatores[i];
        }
        std::cout << "   (verificacao: produto = " << produto
                   << (produto == n ? "  OK" : "  ERRO") << ")\n";
    }
}
