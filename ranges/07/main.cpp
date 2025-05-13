#include <iostream>
#include <ranges>
#include <span>

int main()
{
    int matriz[3][3] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

#if true
    // -----------------------------------------------------------------------
    // exemplo 1:
    //

    std::cout << "exemplo 1" << '\n';

    std::cout << "\nMatriz original (não modificada):\n";
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << matriz[i][j] << " ";
        std::cout << '\n';
    }

    for (auto i = 0; i<3; i++)
    {
        // Cria span para uma linha
        std::span<int> linha (matriz[i], 3);

        for (int& x : linha)
            x *= 2; // modifica direto
    }

    std::cout << "\nMatriz original (modificada):\n";
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << matriz[i][j] << " ";
        std::cout << '\n';
    }
#else
    // -----------------------------------------------------------------------
    // exemplo 2:
    //

    std::cout << "exemplo 2" << '\n';

    for (auto i =0; i<3; i++)
    {
        // Cria span para uma linha
        std::span<int> linha (matriz[i], 3);

        // Agora você pode usar views
        auto dobrado = linha | std::views::transform ([] (auto x)
        {
            return x * 2;
        });

        for (auto x : dobrado)
            std::cout << x << " ";
        std::cout << '\n';
    }
#endif


}
