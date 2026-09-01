#include <iostream>
#include <numeric>  // inner_product
#include <vector>

int dotProduct(const std::vector<int>& vec1,
               const std::vector<int>& vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cout << "Erro: os vetores precisam ter o mesmo tamanho"
                  << std::endl;
        return 0;
    }
    return std::inner_product(
        vec1.begin(), vec1.end(), vec2.begin(), 0);
}

int main()
{
    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2 = {4, 5, 6};

    int result = dotProduct(vec1, vec2);
    std::cout << "Produto escalar dos vetores: " << result
              << std::endl;
    return 0;
}
