#include <iostream>
#include <tuple>
#include <type_traits>
using namespace std;

template<typename... Ts>
class Tuple {
public:
    Tuple(Ts... args) : data{args...} {}

    template <size_t N>
    auto get() {
        return std::get<N>(data);
    }

    static constexpr size_t size() {
        return std::tuple_size_v<decltype(data)>;
    }

private:
    std::tuple<Ts...> data;
};

int main() {
    Tuple<int, double, string> minhaTupla(10, 3.14, "exemplo");
    cout << "Numero de elementos da tupla: " 
         << minhaTupla.size() 
         << endl;
    return 0;
}
