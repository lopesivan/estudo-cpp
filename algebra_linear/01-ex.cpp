#include <iostream>
#include <numeric>   // inner_product
#include <vector>
using namespace std;

int dotProduct(const vector<int>& vec1, const vector<int>& vec2) {
    if (vec1.size() != vec2.size()) {
        cout << "Erro: os vetores precisam ter o mesmo tamanho" << endl;
        return 0;
    }
    return inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0);
}

int main() {
    vector<int> vec1 = {1, 2, 3};
    vector<int> vec2 = {4, 5, 6};

    int result = dotProduct(vec1, vec2);
    cout << "Produto escalar dos vetores: " << result << endl;
    return 0;
}
