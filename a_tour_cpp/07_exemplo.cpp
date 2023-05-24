#include <iostream>

void printValuesByReference (const int (&v)[10])
{
    // for (const int& value : v)
    for (auto& value : v)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

void printValuesByPointer (const int* v)
{
    for (auto i = 0; i < 10; i++)
    {
        std::cout << * (v + i) << " ";
    }
    std::cout << std::endl;
}

int main()
{
    int v[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Imprimir valores usando referência
    std::cout << "Valores do vetor (referência): ";
    printValuesByReference (v);

    // Imprimir valores usando ponteiros
    std::cout << "Valores do vetor (ponteiros): ";
    printValuesByPointer (v);

    return 0;
}
