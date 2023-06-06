#include <iostream>

template <int p, int i> struct IsPrime {
  static const bool primality =
      (p == 2) || (p % 2 != 0 && (p % i != 0) &&
                   IsPrime<(i > 2 ? p : 0), i - 1>::primality);
};

template <> struct IsPrime<0, 0> {
  static const bool primality = true;
};

template <> struct IsPrime<0, 1> {
  static const bool primality = true;
};

template <> struct IsPrime<2, 2> {
  static const bool primality = true;
};

template <int i> struct Prime {
  static const int value =
      Prime<i - 1>::value +
      IsPrime<Prime<i - 1>::value + 1, Prime<i - 1>::value / 2>::primality;
};

template <> struct Prime<0> {
  static const int value = 2;
};

int main() {
  std::cout << Prime<10>::value; // Saída: 29
  return 0;
}
