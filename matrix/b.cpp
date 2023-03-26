#include <iostream>
#include <vector>

using namespace std;

// Função que executa o produto de duas matrizes
vector<vector<int>> matrixProduct(vector<vector<int>> mat1, vector<vector<int>> mat2) {
    // Verifica se as dimensões das matrizes são compatíveis para multiplicação
    int n = mat1.size();
    int m = mat2.size();
    int p = mat2[0].size();
    if (mat1[0].size() != m) {
        cout << "Erro: as matrizes não têm dimensões compatíveis para multiplicação" << endl;
        return {};
    }

    // Cria a matriz resultante com dimensões n x p
    vector<vector<int>> result(n, vector<int>(p, 0));

    // Calcula o produto da matriz
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < m; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}

int main() {
    // Cria duas matrizes
    vector<vector<int>> mat1 = {{1, 2}, {3, 4}, {5, 6}};
    vector<vector<int>> mat2 = {{7, 8}, {9, 10}};

    // Executa o produto das matrizes e imprime o resultado
    vector<vector<int>> result = matrixProduct(mat1, mat2);
    cout << "Produto das matrizes:" << endl;
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[0].size(); j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
