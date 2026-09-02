#include <iostream>
#include <vector>
#include <algorithm>

namespace eop {
template <class I, class F>
F for_each(I first, I last, F f)
{
    while (first != last)
    {
        f(*first);
        ++first;
    }
    return f; // devolve f (pode carregar estado acumulado)
}
}

int main()
{
    std::vector<int> v{1, 2, 3, 4, 5};

    // 1) efeito colateral (imprimir)
    eop::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ' '; });
    std::cout << '\n';

    // 2) std::for_each da biblioteca padrao -- mesma ideia
    std::for_each(v.begin(), v.end(), [](int& x) { x *= 10; });
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // 3) funtor com estado: for_each devolve o funtor final
    struct Somador {
        int total = 0;
        void operator()(int x) { total += x; }
    };
    Somador s = eop::for_each(v.begin(), v.end(), Somador{});
    std::cout << "soma = " << s.total << '\n';
}
