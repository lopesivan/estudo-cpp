#include <iostream>

// square: devolve x * x
// Precondição: o tipo de x suporta multiplicação (x * x) bem
// definida.
template <class T>
T square (const T& x)
{
    return x * x;
}

int main()
{
    std::cout << square<float> (2.8) << std::endl;
}
