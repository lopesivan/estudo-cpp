#include <iostream>
#include <queue>

using namespace std;

int main() {
    // cria a fila de prioridade
    priority_queue<int> pq;

    // insere elementos na fila de prioridade
    pq.push(10);
    pq.push(30);
    pq.push(20);

    // acessa o elemento de maior prioridade (maior valor)
    cout << "Maior elemento na fila de prioridade: " << pq.top() << endl;

    // remove o elemento de maior prioridade
    pq.pop();

    // acessa o novo elemento de maior prioridade
    cout << "Novo maior elemento na fila de prioridade: " << pq.top() << endl;

    return 0;
}
