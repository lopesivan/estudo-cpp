#include <numeric>  // inner_product
#include <functional>
#include <vector>
#include <iostream>

using namespace std;
template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    return inner_product(vec1.begin(),
                         vec1.end(),
                         vec2.begin(),
                         T{},
                         std::plus<>{},
                         std::multiplies<>{});
}

int main()
{
    // Teste com inteiros
    std::vector<int> vec1_int   = {1, 2, 3};
    std::vector<int> vec2_int   = {4, 5, 6};
    int              result_int = dotProduct(vec1_int, vec2_int);
    std::cout << "Produto escalar (int): " << result_int
              << std::endl;

    // Teste com doubles
    std::vector<double> vec1_double = {1.5, 2.5, 3.5};
    std::vector<double> vec2_double = {4.5, 5.5, 6.5};
    double result_double = dotProduct(vec1_double, vec2_double);
    std::cout << "Produto escalar (double): " << result_double
              << std::endl;

    // Teste com floats
    std::vector<float> vec1_float = {1.1f, 2.2f, 3.3f};
    std::vector<float> vec2_float = {4.4f, 5.5f, 6.6f};
    float result_float = dotProduct(vec1_float, vec2_float);
    std::cout << "Produto escalar (float): " << result_float
              << std::endl;

    return 0;
}
