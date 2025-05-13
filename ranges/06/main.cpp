#include <iostream>
#include <ranges>
#include <array>

int main()
{
    int matriz[3][3] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // std::array<std::array<int, 3>, 3> matriz_std;

    // for (int i = 0; i < 3; ++i)
    //     for (int j = 0; j < 3; ++j)
    //         matriz_std[i][j] = matriz[i][j];

    for (auto& linha : matriz)
    {
        auto dobrado = linha | std::views::transform ([] (auto x)
        {
            return x * 2;
        });
        for (auto x : dobrado)
            std::cout << x << " ";
        std::cout << '\n';
    }
}
