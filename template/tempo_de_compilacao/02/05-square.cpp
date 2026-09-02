#include <iostream>
#include <functional>

template <class T>
struct Multiply
{
    T operator() (const T& a, const T& b) const
    {
        return a * b;
    }
};

template <class T, class Op>
T square_op (const T& x, Op op)
{
    return op (x, x);
}

int main()
{
    std::cout << square_op (3.0f, Multiply<float> {})
              << std::endl;

    std::cout << square_op (3.0f, std::multiplies<float> {})
              << std::endl;
}
