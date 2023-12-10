#include <iostream>
#include <string>

template <typename T> class Box {
private:
  T _contents;

public:
  void set(T contents) { _contents = contents; }

  T get() { return _contents; }
};

int main() {
  Box<int> intBox;
  intBox.set(123);
  std::cout << intBox.get() << '\n';

  Box<std::string> stringBox;
  stringBox.set("Hello World");
  std::cout << stringBox.get() << '\n';
  return 0;
}
