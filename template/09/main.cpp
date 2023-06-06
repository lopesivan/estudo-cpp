#include <iostream>

template <unsigned int n> struct Fibonacci {
  static constexpr unsigned long long value =
      Fibonacci<n - 1>::value + Fibonacci<n - 2>::value;
};

template <> struct Fibonacci<0> {
  static constexpr unsigned long long value = 0;
};

template <> struct Fibonacci<1> {
  static constexpr unsigned long long value = 1;
};

int main() {
  std::cout << Fibonacci<10>::value; // Saída: 55
  return 0;
}
