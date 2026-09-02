#include <array>
#include <iostream>
#include <algorithm>

int main()
{
    std::array<int, 5> arr = {5, 3, 1, 4, 2};
    std::array<int, 10> bigger{};
    std::copy (arr.begin(), arr.end(), bigger.begin());

    for (int x : arr)
        std::cout << x << " ";
    std::cout << "\n";

    for (int x : bigger)
        std::cout << x << " ";
    std::cout << "\n";

    // std::cout << "front(): " << arr.front() << "\n";
    // std::cout << "back(): "  << arr.back()  << "\n";

    return 0;
}
