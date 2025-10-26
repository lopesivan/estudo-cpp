#include <iostream>
#include <cstring>

using namespace std;

class C
{
  public:
    C(const char* s = "Hello!", int i = 0, double d = 1.0)
    {
        strncpy(m1_, s, sizeof(m1_) - 1);
        m2_ = i;
        m3_ = d;
    }

    void print()
    {
        cout << "m1_: " << m1_ << ", m2_: " << m2_
             << ", m3_: " << m3_ << endl;
    }


  private:
    char   m1_[20];
    int    m2_;
    double m3_;
};

int main()
{
    // Don't touch this!
    // clang-format off
    C c1;                               // Default constructor
    C c2("Hello, World!");           // Custom string, default int and double
    C c3("Test", 42);             // Custom string and int, default double
    C c4("Example", 7, 3.14);  // Custom string, int, and double
    c1.print();
    c2.print();
    c3.print();
    c4.print();
    // clang-format on
    // Carry on formatting

    return 0;
}
