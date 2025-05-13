#include <octave/oct.h>
#include <octave/Matrix.h>

DEFUN_DLD (det3x3, args, nargout, "Calcula o determinante de uma matriz 3x3")
{
    // Verifica se foi passado 1 argumento
    if (args.length() != 1)
    {
        error ("det3x3: número incorreto de argumentos. Use: det3x3(matrix)");
        return octave_value_list();
    }

    // Tenta converter o argumento para matriz
    Matrix A = args (0).matrix_value();

    // Verifica se é 3x3
    if (A.rows() != 3 || A.columns() != 3)
    {
        error ("det3x3: a matriz deve ser 3x3.");
        return octave_value_list();
    }

    // Calcula o determinante manualmente (regra de Sarrus)
    double det =
        A (0,0) * (A (1,1) * A (2,2) - A (1,2) * A (2,1)) -
        A (0,1) * (A (1,0) * A (2,2) - A (1,2) * A (2,0)) +
        A (0,2) * (A (1,0) * A (2,1) - A (1,1) * A (2,0));

    return octave_value (det);
}
