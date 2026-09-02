#include <iostream>

// Multiply
// Um modelo de operação binária multiplicativa em T.
//
// Requisitos sobre T:
//   - T é um tipo Regular (copiável, atribuível, destruível).
//   - O operador * está definido em T:
//         operator* : T × T → T
//   - A operação * é fechada em T.
//
// Observação:
//   Esta estrutura não guarda estado; modela apenas a operação.
template <class T>
struct Multiply
{
    T operator() (const T& a, const T& b) const
    {
        return a * b;
    }
};

// square_op
// Aplica uma operação binária op ao par (x, x).
//
// Parâmetros:
//   x  — elemento de um conjunto T.
//   op — operação binária em T.
//
// Requisitos:
//   - Op modela uma operação binária em T:
//         op : T × T → T
//   - op(x, x) está definida.
//
// Pós-condição:
//   - O resultado é op(x, x).
//
// Complexidade:
//   - 1 aplicação de op.
template <class T, class Op>
T square_op (const T& x, Op op)
{
    return op (x, x);
}

int main()
{
    {
        // Aqui, Multiply<double> modela uma operação multiplicativa
        // em double. Pré-condição:
        //   - 3.0 é representável como double.
        // Pós-condição:
        //   - O valor impresso é 3.0 * 3.0.
        std::cout << square_op (3.0, Multiply<double> {})
                  << std::endl;
    }
}
