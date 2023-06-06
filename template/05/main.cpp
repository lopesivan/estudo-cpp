#include <algorithm>
#include <iostream>

template <int base, int exponent> struct Power {
  enum { value = base * Power<base, exponent - 1>::value };
};

template <int base> struct Power<base, 0> {
  enum { value = 1 };
};

int main() {
  int result = Power<2, 3>::value; // result é 8
  std::cout << result << '\n';
  return 0;
}
