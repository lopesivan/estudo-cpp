#include <iostream>
#include <tuple>
using namespace std;

int main()
{
    // Criando uma tupla com três elementos
    tuple<int, double, string> minhaTupla (10, 3.14, "exemplo");

    // Acessando cada elemento da tupla usando a função get<>
    cout << get<0>(minhaTupla) 
         << " " 
         << get<1>(minhaTupla) 
         << " " 
         << get<2>(minhaTupla) 
         << endl;

    return 0;
}
