#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>


int main()
{
    std::vector<int> v (10, 2); // vetor com dez elementos, todos iguais a 2

    std::cout << "Valores: ";
    std::ranges::copy (v, std::ostream_iterator<int> (std::cout, " "));
    std::cout << '\n';
}
