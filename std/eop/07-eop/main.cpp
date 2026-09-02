#include <iostream>
#include <cmath>
#include <limits>

// ---------------------------------------------------------------------
// point2d<F>: ponto em 2D sobre um campo escalar F.
// Regular: suporta == com o significado usual (mesmas coordenadas).
// ---------------------------------------------------------------------
template <class F>
struct point2d
{
    F x;
    F y;
};

template <class F>
bool operator== (const point2d<F>& p, const point2d<F>& q)
{
    return p.x == q.x && p.y == q.y;
}

template <class F>
bool operator!= (const point2d<F>& p, const point2d<F>& q)
{
    return ! (p == q);
}

// ---------------------------------------------------------------------
// line2d<F>: reta na forma geral a*x + b*y + c = 0.
//
// IMPORTANTE: a representacao (a,b,c) nao e unica -- (k*a,k*b,k*c)
// para qualquer k != 0 descreve a MESMA reta geometrica. Para que
// line2d seja Regular (igualdade == reflete o valor conceitual, nao
// a representacao em bits), toda reta e mantida em FORMA CANONICA:
//   - (a,b) normalizado como vetor unitario: a^2 + b^2 = 1
//   - sinal escolhido de forma unica: a > 0, ou (a == 0 e b > 0)
// ---------------------------------------------------------------------
template <class F>
struct line2d
{
    F a;
    F b;
    F c;
};

// normalizar: leva uma reta qualquer para sua forma canonica.
// Pre-condicao: (a,b) != (0,0), isto e, a reta e de fato uma reta.
template <class F>
line2d<F> normalizar (line2d<F> l)
{
    F norm = std::sqrt (l.a * l.a + l.b * l.b);
    l.a /= norm;
    l.b /= norm;
    l.c /= norm;

    // Escolhe o sinal canonico: garante que a mesma reta, vinda de
    // qualquer par de pontos em qualquer ordem, produza sempre os
    // mesmos coeficientes (a > 0, ou a == 0 e b > 0).
    if (l.a < F (0) || (l.a == F (0) && l.b < F (0)))
    {
        l.a = -l.a;
        l.b = -l.b;
        l.c = -l.c;
    }
    return l;
}

template <class F>
bool aproximadamente_igual (F x, F y, F eps = std::numeric_limits<F>::epsilon() * 100)
{
    return std::fabs (x - y) <= eps;
}

// operator== para line2d: compara a forma canonica com tolerancia
// (necessaria porque F costuma ser ponto flutuante).
template <class F>
bool operator== (const line2d<F>& l, const line2d<F>& m)
{
    return aproximadamente_igual (l.a, m.a)
           && aproximadamente_igual (l.b, m.b)
           && aproximadamente_igual (l.c, m.c);
}

template <class F>
bool operator!= (const line2d<F>& l, const line2d<F>& m)
{
    return ! (l == m);
}

// ---------------------------------------------------------------------
// line_through(p, q): constroi a reta que passa por p e q, ja em
// forma canonica.
//
// Pre-condicao:  p != q
// Pos-condicao:  line_through(p, q) == line_through(q, p)
//                (mesma reta, independente da ordem dos pontos)
// ---------------------------------------------------------------------
template <class F>
line2d<F> line_through (const point2d<F>& p, const point2d<F>& q)
{
    line2d<F> l;
    l.a = p.y - q.y;
    l.b = q.x - p.x;
    l.c = p.x * q.y - q.x * p.y;
    return normalizar (l);
}

// avaliar: distancia (assinada, a menos de escala) de p a reta l.
// Como l esta normalizada (a^2+b^2=1), isto e EXATAMENTE a distancia
// com sinal do ponto a reta -- um bonus da forma canonica.
template <class F>
F avaliar (const line2d<F>& l, const point2d<F>& p)
{
    return l.a * p.x + l.b * p.y + l.c;
}

int main()
{
    point2d<double> p{0.0, 0.0};
    point2d<double> q{1.0, 1.0};

    auto l1 = line_through (p, q);
    auto l2 = line_through (q, p); // mesma reta, ordem invertida

    std::cout << "l1: a=" << l1.a << " b=" << l1.b << " c=" << l1.c << '\n';
    std::cout << "l2: a=" << l2.a << " b=" << l2.b << " c=" << l2.c << '\n';
    std::cout << "l1 == l2 ? " << (l1 == l2 ? "true" : "false")
              << "  (agora deve ser true)\n\n";

    // Mesma reta y=x, construida a partir de OUTRO par de pontos
    point2d<double> p2{2.0, 2.0};
    point2d<double> q2{5.0, 5.0};
    auto l3 = line_through (p2, q2);
    std::cout << "l3 (outros pontos, mesma reta): a=" << l3.a
              << " b=" << l3.b << " c=" << l3.c << '\n';
    std::cout << "l1 == l3 ? " << (l1 == l3 ? "true" : "false")
              << "  (mesma reta geometrica, pontos diferentes)\n\n";

    // Reta diferente, para garantir que == nao da falso positivo
    point2d<double> r{0.0, 1.0};
    point2d<double> s{1.0, 0.0};
    auto l4 = line_through (r, s); // reta x+y=1, diferente de y=x
    std::cout << "l4 (reta diferente): a=" << l4.a
              << " b=" << l4.b << " c=" << l4.c << '\n';
    std::cout << "l1 == l4 ? " << (l1 == l4 ? "true" : "false")
              << "  (deve ser false)\n\n";

    // Verificacao geometrica: p e q devem estar sobre l1
    std::cout << "avaliar(l1, p) = " << avaliar (l1, p) << " (esperado ~0)\n";
    std::cout << "avaliar(l1, q) = " << avaliar (l1, q) << " (esperado ~0)\n";

    // ponto fora da reta: avaliar da a distancia com sinal (forma normalizada)
    point2d<double> fora{0.0, 2.0};
    std::cout << "avaliar(l1, (0,2)) = " << avaliar (l1, fora)
              << " (deve ser +-sqrt(2), a distancia real ate y=x)\n";
}
