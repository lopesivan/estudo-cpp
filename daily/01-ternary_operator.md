```{cpp}
#include <array>
#include <string>
#include <stdexcept>

struct A {};
struct B : A {};

struct MustBeInit {
    MustBeInit(int v) : v(v) {}
    int v;
};


// Simple logic:
int x = 1;
int y = 2;
int abs_diff = x < y ? y - x : x - y;
// abs_diff = 1

// Useful for initializing variables
// that do not support default initialization:
MustBeInit m = true ? 1 : 2;
// m.v == 1


// Before C++14, conditional expression was
// the only way to inject logic into constant-evaluted code
constexpr bool has_many = true;
std::array<int, has_many ? 1024 : 8> arr;


// When mixing types, standard conversions are permitted, ex:
auto r1 = true ? 2.0 : 1; // int->double
// decltype(r1) == double
auto r2 = true ? A() : B(); // class to base
// decltype(r2) == A


// Throw expressions are permitted, as they break flow:
std::string* ptr = nullptr;
try {
    auto len = ptr ? ptr->length() :
        throw std::runtime_error("Null pointer dereference.");
    // decltype(len) == size_t
} catch (...) {}

// If both sides are throw expressions, the result is void
auto r3 = []{ return true ? throw 1 : throw 2; };
// decltype(r3()) == void


// When mixing cv-qualifiers, the arguments need to overlap,
// but the result is then the most-qualified type.
int a = 1;
const int& b = 2;
int& c = a;

auto &r4 = true ? b : c;
// decltype(r4) == const int&
```
