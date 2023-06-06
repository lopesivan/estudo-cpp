template <typename T>
struct IsConst {
    static const bool value = false;
};

template <typename T>
struct IsConst<const T> {
    static const bool value = true;
};

int main() {
    bool check1 = IsConst<int>::value;        // check1 é false
    bool check2 = IsConst<const int>::value;  // check2 é true
    return 0;
}

