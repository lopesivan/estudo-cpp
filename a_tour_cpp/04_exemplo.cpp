#include <iostream>
#include <vector>

//
// main
//

void inc (std::vector<int>& values)
{
    // for (const int& value : values)
    for (auto& value : values)
    {
        value = value + 10;
    }
}

int sum (const std::vector<int>& values)
{
    int result = 0;
    // for (const int& value : values)
    for (auto& value : values)
    {
        result += value;
    }
    return result;
}
int main()
{

    std::vector<int> v {1, 2, 3, 4, 5, 6}; // a vector of ints

    for (auto x: v)
    {
        std::cout << x << std::endl;
    }

    std::cout << sum (v) << std::endl;

    for (auto x: v)
    {
        std::cout << x << std::endl;
    }

    inc (v);

    for (auto x: v)
    {
        std::cout << x << std::endl;
    }
    return 0; // success return
}
