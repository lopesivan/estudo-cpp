#include <iostream>
#include <string>

using namespace std;

class C
{
  public:
    C(std::string s = "Hello!", int i = 0, double d = 1.0)
        : str(s), num(i), val(d)
    {
    }

    void print()
    {
        cout << "m1_: " << str << ", m2_: " << num
             << ", m3_: " << val << endl;
    }

  private:
    std::string str;
    int         num;
    double      val;
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
