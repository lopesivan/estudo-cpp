#include <iostream>
#include <ranges>
#include <span>

template <size_t N, size_t M>
void imprimir_dobrada (const int (&matriz)[N][M])
{
    std::cout << "Matriz com valores dobrados (view apenas):\n";

    for (size_t i = 0; i < N; ++i)
    {
        std::span<const int> linha (matriz[i], M);

        auto dobrado = linha | std::views::transform ([] (auto x)
        {
            return x * 2;
        });

        for (auto x : dobrado)
            std::cout << x << " ";
        std::cout << '\n';
    }
}


template <size_t N, size_t M>
void dobrar_matriz (auto (&matriz)[N][M])
{
    for (size_t i = 0; i < N; ++i)
    {
        std::span<int> linha (matriz[i], M);

        for (auto x : linha)
            x *= 2;
    }
}


int main()
{
    int matriz[3][3] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

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

    imprimir_dobrada (matriz); // mostra versão dobrada, sem alterar

    std::cout << "\nAgora dobrando a matriz original...\n";
    dobrar_matriz (matriz);   // modifica a matriz original
    dobrar_matriz (matriz);   // modifica a matriz original
    dobrar_matriz (matriz);   // modifica a matriz original
    dobrar_matriz (matriz);   // modifica a matriz original

    std::cout << "\nMatriz original (modificada):\n";
    for (const auto& linha : matriz)
    {
        for (int x : linha)
            std::cout << x << " ";
        std::cout << '\n';
    }

}
