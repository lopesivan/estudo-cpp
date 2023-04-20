#include <iostream>
#include <vector>
using namespace std;

template<typename... Ts>
class Tuple
{
public:
    Tuple (Ts... args) : data{args...} {}

    template <size_t N>
    auto get()
    {
        return std::get<N> (data);
    }

private:
    std::tuple<Ts...> data;
};

int main()
{
    Tuple<int, double, string> minhaTupla (10, 3.14, "exemplo");
    cout << minhaTupla.get<0>() 
         << " " 
         << minhaTupla.get<1>() 
         << " " <<
         minhaTupla.get<2>() 
        << endl;
    return 0;
}
