#include <iostream>

struct MultiplyDouble
{
    double operator() (double a, double b) const
    {
        return a * b;
    }
};

// square_op: aplica operacao binária op em (x, x)
// Precondição: op(x, x) definida.
template <class T, class Op>
T square_op (const T& x, Op op)
{
    return op (x, x);
}

int main()
{
    {
        std::cout << square_op (4.1, MultiplyDouble{}) << std::endl;
    }
}
