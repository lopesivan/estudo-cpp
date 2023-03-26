#include <iostream>
#include <vector>

using namespace std;

// Função que executa o produto escalar de dois vetores
int dotProduct(vector<int> vec1, vector<int> vec2) {
    // Verifica se os vetores têm o mesmo tamanho
    if (vec1.size() != vec2.size()) {
        cout << "Erro: os vetores precisam ter o mesmo tamanho" << endl;
        return 0;
    }

    // Calcula o produto escalar
    int result = 0;
    for (int i = 0; i < vec1.size(); i++) {
        result += vec1[i] * vec2[i];
    }

    return result;
}

int main() {
    // Cria dois vetores
    vector<int> vec1 = {1, 2, 3};
    vector<int> vec2 = {4, 5, 6};

    // Executa o produto escalar dos vetores e imprime o resultado
    int result = dotProduct(vec1, vec2);
    cout << "Produto escalar dos vetores: " << result << endl;

    return 0;
}
