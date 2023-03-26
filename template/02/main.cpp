#include <iostream>
#include <string>

template <typename T1, typename T2>
auto max(T1 a, T2 b) -> decltype(a + b) {
    return (a > b) ? a : b;
}

int main() {
    auto a = 5;
    auto b = 3.5;
    std::cout << "O máximo entre " 
        << a 
        << " e " 
        << b << " é " 
        << max<decltype(a), decltype(b)>(a, b) 
        << std::endl;
    return 0;
}
