template <unsigned int n>
struct Factorial {
    enum { resultado = n * Factorial<n - 1>::resultado };
};

template <>
struct Factorial<0> {
    enum { resultado = 1 };
};

int main() {
    int x = Factorial<5>::resultado; // x recebe o valor 120
    return 0;
}
