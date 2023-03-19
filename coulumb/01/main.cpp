#include <iostream>
#include <cmath>

using namespace std;

int main() {
    double k = 8.9875517923e9; // Constante eletrostática
    double q1, q2, r; // Carga das partículas e distância entre elas
    double f; // Força de atração entre as partículas

    cout << "Digite a carga da primeira partícula (em Coulombs): ";
    cin >> q1;
    cout << "Digite a carga da segunda partícula (em Coulombs): ";
    cin >> q2;
    cout << "Digite a distância entre as partículas (em metros): ";
    cin >> r;

    f = k * q1 * q2 / pow(r, 2);

    cout << "A força de atração entre as partículas é de " << f << " Newtons." << endl;

    return 0;
}
