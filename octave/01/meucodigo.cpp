#include <octave/oct.h>

DEFUN_DLD(minha_funcao, args, nargout, "Descrição da minha função")
{
  // Pegue os argumentos da função
  int arg1 = args(0).int_value();
  int arg2 = args(1).int_value();

  // Faça alguma coisa com os argumentos (por exemplo, some-os)
  int resultado = arg1 + arg2;

  // Retorne o resultado como uma matriz do Octave
  octave_value ret = octave_value(resultado);
  return ret;
}

