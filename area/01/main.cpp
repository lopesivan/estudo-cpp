#include <iostream>
#include <concepts>
using namespace std;

// Definindo um conceito para tipos que têm função área
template <typename T>
concept HasArea = requires(T t) {
    { t.area() } -> std::same_as<double>;
};

// Definindo uma classe template para retângulos
template <typename T>
class Rectangle {
public:
    Rectangle(T width, T height) : width(width), height(height) {}

    double area() const {
        return static_cast<double>(width) * static_cast<double>(height);
    }

private:
    T width, height;
};

// Definindo uma classe template para círculos
template <typename T>
class Circle {
public:
    Circle(T radius) : radius(radius) {}

    double area() const {
        return 3.14159265359 * static_cast<double>(radius) * static_cast<double>(radius);
    }

private:
    T radius;
};

// Definindo a classe Area
class Area {
public:
    // Sobrecarga do operador () para calcular a área de um objeto que tem função área
    template <HasArea T>
    double operator()(T const& t) const {
        return t.area();
    }

};

// Sobrecarga do operador + para somar áreas
// double operator+(double const& a, double const& b) {
//     return a + b;
// }

// Sobrecarga do operador + para somar áreas de objetos que têm função área
// template <HasArea T, HasArea U>
// double operator+(T const& t, U const& u) {
//     return t.area() + u.area();
// }

int main() {
    // Criando objetos de retângulo e círculo
    Rectangle<int> retangulo(5, 3);
    Circle<double> circulo(2.5);

    // Criando um objeto da classe Area
    Area area;

    // Calculando a área do retângulo e do círculo
    double areaRetangulo = area(retangulo);
    double areaCirculo = area(circulo);

    // Somando as áreas do retângulo e do círculo
    double areaTotal = areaRetangulo + areaCirculo;

    double aareaTotal = area(retangulo) + area(circulo);
    // Imprimindo a área total na tela
    cout << "A area total e: " << areaTotal << endl;

    return 0;
}
