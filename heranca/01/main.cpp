#include <iostream>
using namespace std;

class Mamifero {
public:
    virtual void falar() = 0; // Método abstrato
};

class Gato : public Mamifero {
public:
    void falar() {
        cout << "Miau" << endl;
    }
};

class Cachorro : public Mamifero {
public:
    void falar() {
        cout << "Au Au" << endl;
    }
};

int main() {
    Gato gato;
    Cachorro cachorro;

    gato.falar();     // Saída: Miau
    cachorro.falar(); // Saída: Au Au

    return 0;
}

