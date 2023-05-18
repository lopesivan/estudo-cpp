#include <iostream>

template <typename T, typename U>
class Pair {
public:
    T first;
    U second;

    Pair(T f, U s) : first(f), second(s) {}
};

template <typename T>
class Pair<T, T> {
public:
    T both;

    Pair(T value) : both(value) {}
};

int main() {
    Pair<int, float> pair1(10, 3.14f);
    std::cout << pair1.first << ", " << pair1.second << std::endl;

    Pair<int, int> pair2(5);
    std::cout << pair2.both << std::endl;

    return 0;
}

