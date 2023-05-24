#include <iostream>

//
// main
//

void print()
{
    std::cout << "print" << '\n';

    int v[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (auto i=0; i!=10; ++i)   // print elements
        std::cout << v[i] << '\n';
}

void print2()
{
    std::cout << "print2" << '\n';

    int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (auto& x: v)   // print elements
        std::cout << x << '\n';
}

int main()
{

    print();
    print2();

    return 0; // success return
}
