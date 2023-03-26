#include <iostream>
#include <vector>

using namespace std;

// Função auxiliar para calcular o determinante de uma matriz
int determinant(vector<vector<int>> mat) {
    // Verifica se a matriz é quadrada
    int n = mat.size();
    if (n != mat[0].size()) {
        cout << "Erro: a matriz não é quadrada" << endl;
        return 0;
    }

    // Caso base: matriz 1x1
    if (n == 1) {
        return mat[0][0];
    }

    // Caso base: matriz 2x2
    if (n == 2) {
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    }

    // Cálculo do determinante por cofatores
    int det = 0;
    for (int i = 0; i < n; i++) {
        // Cria a matriz cofatora
        vector<vector<int>> cofactor(n-1, vector<int>(n-1));
        for (int j = 1; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (k < i) {
                    cofactor[j-1][k] = mat[j][k];
                }
                else if (k > i) {
                    cofactor[j-1][k-1] = mat[j][k];
                }
            }
        }
        // Calcula o cofator e o adiciona ao determinante
        int cof = ((i % 2 == 0) ? 1 : -1) * mat[0][i] * determinant(cofactor);
        det += cof;
    }

    return det;
}

int main() {
    // Cria a matriz
    vector<vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    // Calcula o determinante e imprime o resultado
    int det = determinant(mat);
    cout << "Determinante da matriz: " << det << endl;

    return 0;
}
