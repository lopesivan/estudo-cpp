#include <iostream>

using namespace std;

template <int n>
struct Fatorial {
    static const int valor = n * Fatorial<n-1>::valor;
};

template <>
struct Fatorial<0> {
    static const int valor = 1;
};

int main() {
    const int num = 5;
    cout << "O fatorial de " << num << " é " << Fatorial<num>::valor << endl;
    return 0;
}
