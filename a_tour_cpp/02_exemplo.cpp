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
    double d1 = 2.3;                  // initialize d1 to 2.3
    double d2 {2.3};                  // initialize d2 to 2.3
    double d3 = {2.3};                // initialize d3 to 2.3 (the = is optional with { ... })

    complex<double> z = 1;            // a complex number with double-precision floating-point scalars
    complex<double> z2 {d1, d2};
    complex<double> z3 = {d1, d2};    // the = is optional with { ... }

    vector<int> v {1, 2, 3, 4, 5, 6}; // a vector of ints

    return 0; // success return
}
