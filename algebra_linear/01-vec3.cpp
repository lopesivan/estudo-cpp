#include <iostream>

template <class T> struct Vec3 {
  T x, y, z;

  Vec3 operator+(const Vec3 &o) const { return {x + o.x, y + o.y, z + o.z}; }
  Vec3 operator-(const Vec3 &o) const { return {x - o.x, y - o.y, z - o.z}; }
  Vec3 operator*(T s) const { return {x * s, y * s, z * s}; }

  T dot(const Vec3 &o) const { return x * o.x + y * o.y + z * o.z; }

  Vec3 cross(const Vec3 &o) const {
    return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
  }
};

template <class T>
std::ostream &operator<<(std::ostream &os, const Vec3<T> &v) {
  return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

int main() {
  Vec3<float> a{1.0f, 2.0f, 3.0f};
  Vec3<float> b{4.0f, 5.0f, 6.0f};

  std::cout << "a + b = " << (a + b) << std::endl;
  std::cout << "a - b = " << (a - b) << std::endl;
  std::cout << "a * 2 = " << (a * 2.0f) << std::endl;
  std::cout << "dot   = " << a.dot(b) << std::endl;
  std::cout << "cross = " << a.cross(b) << std::endl;

  return 0;
}
