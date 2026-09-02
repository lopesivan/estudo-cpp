#include <array>
#include <vector>
#include <iostream>

int main()
{
    std::array<float, 3> arr = {1.0f, 2.0f, 3.0f};

    // Converter array -> vector
    std::vector<float> vec (arr.begin(), arr.end());
    // ou: std::vector<float> vec (arr.data(), arr.data() + arr.size());

    // Acessar x, y, z — vector não tem .x/.y/.z (não é struct), só índice
    float x = vec[0];
    float y = vec[1];
    float z = vec[2];

    std::cout << "x=" << x << " y=" << y << " z=" << z << std::endl;

    return 0;
}
