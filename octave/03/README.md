Um exemplo completo em C++ que define uma função Octave
chamada `det3x3`, que calcula o **determinante de uma matriz
3×3**.

---

## ✅ Passo 1: Código C++ (`det3x3.cpp`)

```cpp
#include <octave/oct.h>
#include <octave/Matrix.h>

DEFUN_DLD(det3x3, args, nargout, "Calcula o determinante de uma matriz 3x3")
{
    // Verifica se foi passado 1 argumento
    if (args.length() != 1) {
        error("det3x3: número incorreto de argumentos. Use: det3x3(matrix)");
        return octave_value_list();
    }

    // Tenta converter o argumento para matriz
    Matrix A = args (0).matrix_value();

    // Verifica se é 3x3
    if (A.rows() != 3 || A.columns() != 3) {
        error("det3x3: a matriz deve ser 3x3.");
        return octave_value_list();
    }

    // Calcula o determinante manualmente (regra de Sarrus)
    double det =
        A(0,0) * (A(1,1) * A(2,2) - A(1,2) * A(2,1)) -
        A(0,1) * (A(1,0) * A(2,2) - A(1,2) * A(2,0)) +
        A(0,2) * (A(1,0) * A(2,1) - A(1,1) * A(2,0));

    return octave_value(det);
}
```

---

## ✅ Passo 2: Compilar com `mkoctfile`

No terminal:

```bash
mkoctfile det3x3.cpp
```

Isso criará um arquivo `det3x3.oct`.

---

## ✅ Passo 3: Usar no Octave

Abra o Octave no mesmo diretório:

```octave
A = [1 2 3; 0 1 4; 5 6 0];
det3x3(A)
```

### Resultado esperado:

```octave
ans = 1
```

---

## ✅ Verificações implementadas:

* Verifica se a matriz passada tem tamanho 3×3.
* Usa a **fórmula direta do determinante** para melhor desempenho.
* Mostra mensagens de erro se os argumentos forem inválidos.

---

## 🧪 Testes extras no Octave:

```octave
det3x3(rand(3))        % válido
det3x3(rand(4))        % erro: não é 3x3
det3x3("teste")        % erro: não é matriz
det3x3()               % erro: sem argumentos
```

---

