#include <iostream>
#include <vector>

#include <cmath>

using namespace std;

class Polinomio
{
private:
    vector<double> coeficientes;
public:
    Polinomio() {}
    Polinomio (const vector<double> &coef) : coeficientes (coef) {}
    double avaliar (double x) const
    {
        double resultado = 0;
        int grau = coeficientes.size() - 1;
        for (int i = grau; i >= 0; i--)
        {
            resultado += coeficientes[i] * pow (x, i);
        }
        return resultado;
    }
    friend Polinomio operator+ (const Polinomio &p1, const Polinomio &p2)
    {
        vector<double> coef (p1.coeficientes.size());
        for (int i = 0; i < p1.coeficientes.size(); i++)
        {
            coef[i] = p1.coeficientes[i];
        }
        for (int i = 0; i < p2.coeficientes.size(); i++)
        {
            coef[i] += p2.coeficientes[i];
        }
        return Polinomio (coef);
    }
    friend Polinomio operator- (const Polinomio &p1, const Polinomio &p2)
    {
        vector<double> coef (p1.coeficientes.size());
        for (int i = 0; i < p1.coeficientes.size(); i++)
        {
            coef[i] = p1.coeficientes[i];
        }
        for (int i = 0; i < p2.coeficientes.size(); i++)
        {
            coef[i] -= p2.coeficientes[i];
        }
        return Polinomio (coef);
    }
    friend Polinomio operator* (const Polinomio &p1, const Polinomio &p2)
    {
        int grau = p1.coeficientes.size() + p2.coeficientes.size() - 2;
        vector<double> coef (grau + 1);
        for (int i = 0; i < p1.coeficientes.size(); i++)
        {
            for (int j = 0; j < p2.coeficientes.size(); j++)
            {
                coef[i + j] += p1.coeficientes[i] * p2.coeficientes[j];
            }
        }
        return Polinomio (coef);
    }
    friend ostream &operator<< (ostream &os, const Polinomio &p)
    {
        int grau = p.coeficientes.size() - 1;
        os << p.coeficientes[grau] << "x^" << grau;
        for (int i = grau - 1; i >= 0; i--)
        {
            if (p.coeficientes[i] >= 0)
            {
                os << " + ";
            }
            else
            {
                os << " - ";
            }
            os << abs (p.coeficientes[i]) << "x^" << i;
        }
        return os;
    }
};

int main()
{
    Polinomio p1 ({1, 2, 3});
    Polinomio p2 ({-3, 2, 1, 4});
    cout << "p1(x) = " << p1 << endl;
    cout << "p2(x) = " << p2 << endl;
    cout << "p1(2) = " << p1.avaliar (2) << endl;
    cout << "p2(2) = " << p2.avaliar (2) << endl;
    Polinomio p3 = p1 + p2;
    cout << "p3(x) = p1(x) + p2(x) = " << p3 << endl;
    Polinomio p4 = p1 - p2;
    cout << "p4(x) = p1(x) - p2(x) = " << p4 << endl;
    Polinomio p5 = p1 * p2;
    cout << "p5(x) = p1(x) * p2(x) = " << p5 << endl;
    return 0;
}
