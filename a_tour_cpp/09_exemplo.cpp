#include <iostream>

void f()
{
    float x, &r = x;

    [=]
    {
        decltype (x) y1;        // y1 tem o tipo float
        decltype ((x)) y2 = y1; // y2 tem o tipo const float& porque esta lambda
        // não é mutable e x é um lvalue

        decltype (r) r1 = y1;   // r1 tem o tipo float&

        // Aqui precisamos garantir que r2 seja const float&
        const decltype ((x))& r2 = y2; // r2 tem o tipo const float&
    };
}

int main (int argc, char* argv[])
{
    f();

    return 0;
}
