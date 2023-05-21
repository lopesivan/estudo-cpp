/* hello.cpp: A standard "Hello, world!" program */
#include <iostream>
#include <cmath>

using namespace std;

//
// main
//

int main()
{
    auto b = true;     // a bool
    auto ch = 'x' ;    // a char
    auto i = 123;      // an int
    auto d = 1.2;      // a double
    auto y = 9.9;      // z has the type of whatever sqrt(y) returns
    auto z = sqrt (y); // z has the type of whatever sqrt(y) returns
    auto bb {true};    // bb is a bool

    return 0; // success return
}
