# Diferença entre `consteval` e `constexpr`

`consteval` e `constexpr` são palavras-chave introduzidas no
padrão C++20 para especificar funções que podem ser avaliadas em
tempo de compilação. Embora eles sejam semelhantes em alguns
aspectos, existem diferenças importantes entre eles.

## As principais diferenças

Aqui estão as diferenças principais entre `consteval` e
`constexpr`:

1. Tempo de avaliação: 
   - `consteval`: A função `consteval` deve ser avaliada em
   tempo de compilação, garantindo que seu resultado seja
   conhecido em tempo de compilação. Se uma chamada a uma função
   `consteval` não puder ser avaliada em tempo de compilação,
   ocorrerá um erro de compilação.
   - `constexpr`: A função `constexpr` pode ser avaliada tanto
   em tempo de compilação quanto em tempo de execução,
   dependendo do contexto em que é usada. Se uma chamada a uma
   função `constexpr` não puder ser avaliada em tempo de
   compilação, ela será avaliada em tempo de execução.

2. Restrições de uso:
   - `consteval`: Uma função `consteval` só pode conter um
   conjunto limitado de instruções, conhecidas como instruções
   "core constant expressions" (expressões constantes
           principais). Isso significa que o corpo da função
   `consteval` deve ser composto por instruções que podem ser
   avaliadas em tempo de compilação, como atribuições, loops e
   chamadas a outras funções `consteval`.
   - `constexpr`: Uma função `constexpr` tem menos restrições em
   relação ao seu corpo. Ela pode conter uma variedade mais
   ampla de instruções, incluindo instruções condicionais e
   loops mais complexos. No entanto, é importante lembrar que o
   corpo da função `constexpr` deve ser avaliável em tempo de
   compilação sempre que for necessário.

Em resumo, `consteval` é mais restritivo e garante que a função
seja avaliada em tempo de compilação, enquanto `constexpr` é
mais flexível e permite avaliação em tempo de compilação ou em
tempo de execução, dependendo do contexto. A escolha entre
`consteval` e `constexpr` depende das necessidades específicas e
das restrições do problema em questão.

## Pointeiros, Arrays, e Referencias
```{cpp}
char v[6];                // array of 6 characters
```

Similarly, a pointer can be declared like this:

```{cpp}
char* p;                  // pointer to character
```

In declarations, `[ ]` means `array of` and * means `pointer to.`
All arrays have 0 as their lower bound, so v has six elements,
v[0] to v[5]. 

```{cpp}
char* p = &v[3];       // p points to v's fourth element
char x = *p;           // *p is the object that p points to
```

Aqui está um exemplo de função em C++ que imprime os valores do
vetor v usando referência e, em seguida, ponteiros:

```{cpp}
#include <iostream>

void printValuesByReference(const int (&v)[10]) {
    for (const int& value : v) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

void printValuesByPointer(const int* v) {
    for (int i = 0; i < 10; i++) {
        std::cout << *(v + i) << " ";
    }
    std::cout << std::endl;
}

int main() {
    int v[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Imprimir valores usando referência
    std::cout << "Valores do vetor (referência): ";
    printValuesByReference(v);

    // Imprimir valores usando ponteiros
    std::cout << "Valores do vetor (ponteiros): ";
    printValuesByPointer(v);

    return 0;
}
```

## cdecl
No Linux, você pode usar o comando `cdecl` para transcrever a
definição de ponteiros em C. O `cdecl` é uma ferramenta de linha
de comando que ajuda a traduzir expressões em C ou C++ em inglês
para declarações em C.

Para instalar o `cdecl` no Ubuntu ou em outras distribuições
baseadas em Debian, você pode usar o seguinte comando:

```bash
sudo apt-get install cdecl
```

Após a instalação do `cdecl`, você pode usá-lo da seguinte
maneira para transcrever a definição de ponteiros em C:

```bash
cdecl explain int*
```

Nesse exemplo, estamos usando o `cdecl` para explicar a
definição de um ponteiro para `int`, representado por `int*`. O
comando retornará a tradução em inglês da declaração, explicando
o significado da definição do ponteiro.

Você pode substituir `int*` pelo tipo de ponteiro desejado para
obter a transcrição correspondente.

Por exemplo, ao executar `cdecl explain char**`, o comando
retornará:

```
declare char as pointer to pointer to char
```

Isso indica que `char**` é declarado como um ponteiro para um
ponteiro de `char`.

O `cdecl` é uma ferramenta útil para auxiliar na interpretação e
compreensão de definições de ponteiros em C e pode ser uma opção
conveniente para verificar a sintaxe e a semântica dessas
definições.



```
 I  cdecl> explain int (*p)[10]
declare p as pointer to array 10 of int
```


Sim, em parâmetros de função, `int vetor[]` e `int *vetor` são equivalentes. 

Em C, quando um vetor é passado como argumento para uma função, ele é tratado como um ponteiro para o primeiro elemento do vetor. Portanto, `int vetor[]` e `int *vetor` são maneiras diferentes de declarar o mesmo parâmetro, indicando que a função espera receber o endereço de memória do primeiro elemento do vetor.

A declaração `int vetor[]` é uma sintaxe especial que pode ser usada para indicar que o parâmetro representa um vetor. No entanto, essa sintaxe é interpretada como um ponteiro para o tipo do elemento do vetor, ou seja, `int *vetor`.

Portanto, tanto `int vetor[]` quanto `int *vetor` permitem que você acesse os elementos do vetor dentro da função utilizando a notação `vetor[i]`. Ambas as formas são comumente usadas e são equivalentes na passagem de vetores como argumentos de função em C.


Na declaração `int *vetor[10]`, a sintaxe está um pouco equivocada. Essa declaração cria um vetor de ponteiros para inteiros com capacidade para 10 elementos. Cada elemento desse vetor é um ponteiro para um inteiro.

No entanto, é importante mencionar que essa declaração não cria um vetor de tamanho fixo, mas sim um vetor de ponteiros. Cada elemento do vetor de ponteiros pode apontar para um inteiro em uma posição de memória diferente.

Aqui está um exemplo para ilustrar a declaração e uso desse vetor de ponteiros:

```c
#include <stdio.h>

int main() {
    int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8, i = 9, j = 10;
    int *vetor[10] = {&a, &b, &c, &d, &e, &f, &g, &h, &i, &j};

    for (int i = 0; i < 10; i++) {
        printf("%d ", *(vetor[i]));
    }
    printf("\n");

    return 0;
}
```

Nesse exemplo, temos um vetor de ponteiros `vetor` com capacidade para 10 elementos. Cada elemento do vetor é inicializado com o endereço de uma variável inteira diferente (`a`, `b`, `c`, etc.). O operador `&` é usado para obter o endereço das variáveis.

Dentro do loop `for`, percorremos o vetor de ponteiros e utilizamos o operador `*` para acessar o valor apontado por cada ponteiro. Em seguida, imprimimos esse valor.

A saída será: `1 2 3 4 5 6 7 8 9 10`, que são os valores das variáveis `a`, `b`, `c`, etc.

Portanto, a declaração `int *vetor[10]` cria um vetor de ponteiros para inteiros, onde cada elemento pode apontar para um inteiro em uma posição de memória diferente.

