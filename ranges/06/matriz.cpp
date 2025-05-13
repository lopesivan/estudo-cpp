#include <iostream>

int main()
{
    int matriz[3][3] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Multiplica cada elemento por 2
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matriz[i][j] *= 2;

    // Imprime o resultado
    std::cout << "Matriz multiplicada por 2:\n";
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << matriz[i][j] << " ";
        std::cout << '\n';
    }
}
