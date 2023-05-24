/* hello.cpp: A standard "Hello, world!" program */
#include <iostream>
#include <complex>
#include <vector>

using namespace std;

//
// main
//

int main()
{
    int v[] {1, 2, 3, 4, 5, 6}; // a vector of ints

    std::cout << "antes do incremento" << std::endl;
    for (auto x: v)
    {
        std::cout << x << std::endl;
    }

    for (auto& x: v)
    {
        ++x;
    }

    std::cout << "apos o incremento" << std::endl;

    for (auto x: v)
    {
        std::cout << x << std::endl;
    }

    std::cout << "mais incremento" << std::endl;

    for (auto& x: v)
    {
        ++x;
    }
    for (auto x: v)
    {
        std::cout << x << std::endl;
    }

    return 0; // success return
}
