#include <array>
#include <stdexcept>
#include <string>

struct A {};
struct B : A {};

struct MustBeInit {
  MustBeInit(int v) : v(v) {}
  int v;
};

int main(int argc, char *argv[]) {
  // Simple logic:
  int x = 1;
  int y = 2;
  int abs_diff = x < y ? y - x : x - y;
  // abs_diff = 1
  return 0;
}
