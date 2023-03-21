#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Cria a lista de adjacências do grafo
    vector<vector<int>> grafo(3); // O grafo tem 3 vértices

    // Adiciona as arestas do grafo
    grafo[0].push_back(1); // Adiciona uma aresta entre os vértices 0 e 1
    grafo[1].push_back(0);
    grafo[1].push_back(2); // Adiciona uma aresta entre os vértices 1 e 2
    grafo[2].push_back(1);

    // Imprime o grafo
    for (int i = 0; i < grafo.size(); i++) {
        cout << "Vértice " << i << " é adjacente a: ";
        for (int j = 0; j < grafo[i].size(); j++) {
            cout << grafo[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
