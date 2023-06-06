template <typename T>
struct IsPointer {
    static const bool value = false;
};

template <typename T>
struct IsPointer<T*> {
    static const bool value = true;
};

int main() {
    bool check1 = IsPointer<int>::value;    // check1 é false
    bool check2 = IsPointer<int*>::value;   // check2 é true
    return 0;
}
