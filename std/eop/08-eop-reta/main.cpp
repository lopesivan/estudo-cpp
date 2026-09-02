#include <iostream>

// Modelamos um ponto em 2D sobre um tipo escalar F.
// Requisitos sobre F (informais):
//   - F modela um campo numerico (como double, float, etc.).
template <class F>
struct point2d
{
    F x;
    F y;
};

// Reta na forma geral: a*x + b*y + c = 0
template <class F>
struct line2d
{
    F a;
    F b;
    F c;
};

// line_through
// Constroi a reta que passa por p e q.
//
// Pre-condicao:
//   - p.x != q.x ou p.y != q.y  (isto e, p != q).
// Pos-condicao:
//   - line_through(p, q) representa o conjunto de pontos colineares
//   com p e q.
template <class F>
line2d<F> line_through (const point2d<F>& p, const point2d<F>& q)
{
    line2d<F> l;
    l.a = p.y - q.y;
    l.b = q.x - p.x;
    l.c = p.x * q.y - q.x * p.y;
    return l;
}

// avaliar: substitui p na equacao da reta (a*x + b*y + c).
// Resultado 0 significa que p esta sobre a reta.
template <class F>
F avaliar (const line2d<F>& l, const point2d<F>& p)
{
    return l.a * p.x + l.b * p.y + l.c;
}

int main()
{
    point2d<double> p{0.0, 2.0};
    point2d<double> q{1.0, 6.0};

    auto l = line_through (p, q);
    std::cout << "reta: a=" << l.a << " b=" << l.b << " c=" << l.c << '\n';

    std::cout << "avaliar(l, p) = " << avaliar (l, p) << " (esperado 0)\n";
    std::cout << "avaliar(l, q) = " << avaliar (l, q) << " (esperado 0)\n";

    point2d<double> fora{0.0, 5.0};
    std::cout << "avaliar(l, (0,5)) = " << avaliar (l, fora)
              << " (esperado != 0)\n";
}
