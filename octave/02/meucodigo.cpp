#include <octave/oct.h>
#include <octave/ov-struct.h> // caso queira usar dicionários no futuro

DEFUN_DLD (minha_funcao, args, nargout, "Soma dois inteiros: minha_funcao(a, b)")
{
    // Verificar número de argumentos
    if (args.length() != 2)
    {
        error ("minha_funcao: você deve passar exatamente 2 argumentos inteiros.");
        return octave_value_list();
    }

    // Tentar extrair os valores inteiros
    int arg1 = args (0).int_value();
    int arg2 = args (1).int_value();

    // Verificar se houve erro na conversão
    // if (error_state)
    // {
    //     error ("minha_funcao: os argumentos devem ser inteiros.");
    //     return octave_value_list();
    // }

    // Realiza a soma
    int resultado = arg1 + arg2;

    // Retorna como valor do Octave
    octave_value_list retorno;
    retorno (0) = resultado;
    return retorno;
}
