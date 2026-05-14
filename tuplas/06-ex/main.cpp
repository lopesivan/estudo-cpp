#include <iostream>
#include <tuple>

int main(int argc, char *argv[]) {
  int r{2}, g{3}, b{4};
  // std::tuple<int, int, int> rgb;

  std::cout << "r: " << r << " g: " << g << " b: " << b << std::endl;

  auto rgb = std::make_tuple(r, g, b);

  std::cout << "r: " << std::get<0>(rgb) << " g: " << std::get<1>(rgb)
            << " b: " << std::get<2>(rgb) << std::endl;
  return 0;
}
