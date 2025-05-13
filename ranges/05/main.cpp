#include <iostream>
#include <ranges>
#include <array>

int main()
{
    std::array<std::array<int, 3>, 3> matriz {{
            {{1, 2, 3}},
            {{4, 5, 6}},
            {{7, 8, 9}}
        }};

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
