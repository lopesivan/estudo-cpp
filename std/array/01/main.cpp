#include <array>
#include <iostream>
#include <algorithm>

int main()
{
    std::array<int, 5> arr = {5, 3, 1, 4, 2};

    // Tamanho fixo, conhecido em compile-time
    std::cout << "Tamanho: " << arr.size() << "\n";

    // Acesso: igual array C, mas com checagem de limites via .at()
    std::cout << "arr[2] = " << arr[2] << "\n";
    std::cout << "arr.at(2) = " << arr.at (2) << "\n"; // lança exceção se fora do range

    // Suporta algoritmos da STL (array C puro também suporta, mas array é mais idiomático)
    std::sort (arr.begin(), arr.end());

    std::cout << "Ordenado: ";
    for (int x : arr)
        std::cout << x << " ";
    std::cout << "\n";

    // Métodos extras que T[N] não tem
    std::cout << "front(): " << arr.front() << "\n";
    std::cout << "back(): "  << arr.back()  << "\n";

    return 0;
}
