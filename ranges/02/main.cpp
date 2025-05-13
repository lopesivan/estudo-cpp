#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v (10, 2); // vetor com dez elementos, todos iguais a 2

    for (auto x : v)
    {
        std::cout << x << " ";
    }
    std::cout << '\n';
}
