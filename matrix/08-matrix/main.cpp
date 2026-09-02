#include <iostream>
#include <numeric>

#include "matrixnd.hpp"

int main()
{
    // Criando uma matriz 3D (2x3x4)
    MatrixND<double> mat ({2, 3, 4});

    // 1. Populando linearmente via iteradores (compativel com STL / iterator_traits)
    std::iota (mat.begin(), mat.end(), 1.0);

    // 2. Acesso estilo C via get_root<T***>()
    auto mat3d = mat.get_root<double*** >();
    std::cout << "Acesso mat3d[1][2][3]: " << mat3d[1][2][3] << "\n"; // Deve imprimir 24.0

    // 3. Uso do custom::distance e custom::advance
    std::cout << "Total de elementos (custom::distance): "
              << custom::distance (mat.begin(), mat.end()) << "\n";

    auto it = mat.begin();
    custom::advance (it, 15); // Avanca 15 elementos na memoria contigua
    std::cout << "Elemento na posicao linear 15: " << *it << "\n\n";

    // 4. Impressao por fatias usando get_root
    for (size_t i = 0; i < mat.dim (0); ++i)
    {
        std::cout << "=== FATIA " << i << " ===\n";
        for (size_t j = 0; j < mat.dim (1); ++j)
        {
            std::cout << "[ ";
            for (size_t k = 0; k < mat.dim (2); ++k)
            {
                std::cout << mat3d[i][j][k] << "\t";
            }
            std::cout << "]\n";
        }
        std::cout << "\n";
    }

    return 0;
}
