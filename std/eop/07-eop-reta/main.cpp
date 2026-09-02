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
    point2d<double> p{0.0, 2.0};
    point2d<double> q{4.4, 9.0};

    auto l1 = line_through (p, q);

    std::cout << "l1: a=" << l1.a << " b=" << l1.b << " c=" << l1.c << '\n';

}
