
```c
double **new_2d(const unsigned long dim1, const unsigned long dim2)
{
    size_t layer_1_size = dim1 * sizeof(double *);
    size_t layers_size  = layer_1_size;

    size_t data_size = dim1 * dim2 * sizeof(double);

    unsigned char *raw_bytes =
        (unsigned char *)malloc(layers_size + data_size);

    if (raw_bytes == NULL)
        return NULL;

    double **array2d = (double **)raw_bytes;

    for (unsigned int i = 0; i < dim1; ++i)
    {
        array2d[i] =
            (double *)(array2d + dim1) + i * dim2;
    }

    return array2d;
}
```


Eu acho uma boa forma de alocar uma matriz 2D quando você quer
simultaneamente:

* usar a sintaxe natural `matriz[i][j]`;
* manter todos os `double` contíguos na memória;
* fazer apenas uma chamada a `malloc`;
* liberar tudo com apenas `free(matriz)`.

A organização do bloco é esta:

```text
┌────────────────────────────┬──────────────────────────────────┐
│ ponteiros para cada linha  │ valores double contíguos         │
│ array2d[0], array2d[1]...  │ linha 0, linha 1, linha 2...     │
└────────────────────────────┴──────────────────────────────────┘
```

Cada ponteiro é configurado para o começo de uma linha:

```c
array2d[i] = (double *)(array2d + dim1) + i * dim2;
```

Por exemplo, para uma matriz `3 × 4`:

```text
array2d[0] → dados[0]
array2d[1] → dados[4]
array2d[2] → dados[8]
```

Então:

```c
array2d[2][1]
```

corresponde ao elemento linear:

```c
dados[2 * 4 + 1]
```

### Pontos positivos

Essa implementação tem boa localidade de memória. Percorrer a
matriz linha por linha tende a aproveitar bem o cache:

```c
for (size_t i = 0; i < linhas; ++i)
    for (size_t j = 0; j < colunas; ++j)
        matriz[i][j] = 0.0;
```

Também não existe o problema de alocar cada linha separadamente e precisar liberar uma por uma.

### Cuidados importantes

O principal problema da implementação atual é o risco de
overflow na multiplicação:

```c
dim1 * dim2 * sizeof(double)
```

Se as dimensões forem muito grandes, o resultado pode
transbordar antes de chegar ao `malloc`, fazendo com que ele
aloque menos memória do que o necessário.

Também é melhor usar `size_t` para dimensões e índices:

```c
double **new_2d(size_t dim1, size_t dim2)
```

e:

```c
for (size_t i = 0; i < dim1; ++i)
```

Em C, não é necessário converter o retorno de `malloc`. Eu
escreveria sua ideia desta maneira:

```c
double **new_2d(size_t dim1, size_t dim2)
{
    if (dim1 == 0 || dim2 == 0)
        return NULL;

    if (dim1 > SIZE_MAX / sizeof(double *))
        return NULL;

    if (dim2 > SIZE_MAX / dim1)
        return NULL;

    size_t number_of_elements = dim1 * dim2;

    if (number_of_elements > SIZE_MAX / sizeof(double))
        return NULL;

    size_t pointers_size = dim1 * sizeof(double *);
    size_t data_size     = number_of_elements * sizeof(double);

    if (pointers_size > SIZE_MAX - data_size)
        return NULL;

    double **matrix = malloc(pointers_size + data_size);

    if (matrix == NULL)
        return NULL;

    double *data = (double *)((unsigned char *)matrix + pointers_size);

    for (size_t i = 0; i < dim1; ++i)
        matrix[i] = data + i * dim2;

    return matrix;
}
```

Uso:

```c
double **matrix = new_2d(5, 6);

if (matrix != NULL)
{
    matrix[2][3] = 10.5;

    /* Uma única liberação. */
    free(matrix);
}
```

Portanto, a ideia é eficiente e elegante. Para uma matriz
especificamente de `double`, ela é melhor do que fazer um
`malloc` separado para cada linha. A principal melhoria
necessária é proteger os cálculos contra overflow.




Para um vetor de `double`, não é necessária uma tabela de
ponteiros. Basta alocar um bloco contínuo:

```c
double *new_1d(size_t size)
{
    double *vector = malloc(size * sizeof(double));

    if (vector == NULL)
        return NULL;

    return vector;
}
```

Manipulação com índices:

```c
double *vector = new_1d(5);

if (vector == NULL)
    return EXIT_FAILURE;

vector[0] = 10.0;
vector[1] = 20.0;
vector[2] = 30.0;

printf("%.2f\n", vector[1]);

free(vector);
```

A mesma manipulação usando apenas aritmética de ponteiros:

```c
*(vector + 0) = 10.0;
*(vector + 1) = 20.0;
*(vector + 2) = 30.0;

printf("%.2f\n", *(vector + 1));
```

Estas expressões são equivalentes:

```c
vector[i]
*(vector + i)
```

Uma função para preencher o vetor usando ponteiros poderia ser:

```c
void populate_vector(double *vector, size_t size)
{
    double *end = vector + size;

    for (double *current = vector; current != end; ++current)
        *current = (double)(current - vector);
}
```

Uso completo:

```c
#include <stdio.h>
#include <stdlib.h>

double *new_1d(size_t size)
{
    return malloc(size * sizeof(double));
}

void populate_vector(double *vector, size_t size)
{
    double *end = vector + size;

    for (double *current = vector; current != end; ++current)
        *current = (double)(current - vector);
}

int main(void)
{
    size_t size = 5;

    double *vector = new_1d(size);

    if (vector == NULL)
        return EXIT_FAILURE;

    populate_vector(vector, size);

    for (double *current = vector;
         current != vector + size;
         ++current)
    {
        printf("%.2f\n", *current);
    }

    free(vector);

    return EXIT_SUCCESS;
}
```

Na matriz, `double **` é necessário para permitir
`matrix[i][j]`. No vetor, um único `double *` já permite tanto
`vector[i]` quanto `*(vector + i)`.



No C++, `std::vector` já administra a alocação e a liberação da
memória. Para um vetor de `double`:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<double> vector(5);

    vector[0] = 10.0;
    vector[1] = 20.0;
    vector[2] = 30.0;

    std::cout << vector[1] << '\n';
}
```

Internamente, os elementos estão em uma região contínua. Você
pode obter o ponteiro com `data()`:

```cpp
double *pointer = vector.data();

*(pointer + 0) = 10.0;
*(pointer + 1) = 20.0;
*(pointer + 2) = 30.0;

std::cout << *(pointer + 1) << '\n';
```

Estas expressões são equivalentes:

```cpp
vector[i]
vector.data()[i]
*(vector.data() + i)
```

Para representar uma matriz 2D contígua, eu usaria um único
`std::vector<double>`:

```cpp
#include <cstddef>
#include <iostream>
#include <vector>

int main()
{
    std::size_t linhas  = 5;
    std::size_t colunas = 6;

    std::vector<double> matriz(linhas * colunas);

    matriz[2 * colunas + 3] = 10.5;

    std::cout << matriz[2 * colunas + 3] << '\n';
}
```

Com ponteiros:

```cpp
double *dados = matriz.data();

*(dados + 2 * colunas + 3) = 10.5;
```

Para manter uma sintaxe parecida com `matriz[i][j]`, você pode
obter o começo de uma linha:

```cpp
double *linha = matriz.data() + 2 * colunas;

linha[3] = 10.5;
```

Ou diretamente:

```cpp
(matriz.data() + 2 * colunas)[3] = 10.5;
```

A relação é:

```cpp
matriz[i][j]  // ideia lógica
```

equivale, no vetor linear, a:

```cpp
matriz[i * colunas + j]
```

Eu evitaria:

```cpp
std::vector<std::vector<double>> matriz;
```

quando você precisa de memória totalmente contígua, porque cada
linha é um `std::vector` independente. Um único
`std::vector<double>` é o equivalente mais próximo da sua
alocação 2D contínua em C.


Sim. A parte dos dados da sua `new_2d` é bastante parecida com
um `std::vector<double>`: ambos armazenam os `double` em memória
linear e contígua.

Na sua função, o bloco possui duas regiões:

```text
┌─────────────────────────┬─────────────────────────────────┐
│ ponteiros das linhas    │ dados double contíguos          │
│ double* double* ...     │ d0 d1 d2 d3 d4 d5 ...           │
└─────────────────────────┴─────────────────────────────────┘
```

Já um `std::vector<double>` contém conceitualmente:

```text
objeto vector              memória dinâmica
┌──────────────────┐       ┌─────────────────────────────┐
│ ponteiro         │ ────► │ d0 d1 d2 d3 d4 d5 ...       │
│ tamanho          │       └─────────────────────────────┘
│ capacidade       │
└──────────────────┘
```

A semelhança principal é:

```c
/* Sua matriz em C */
array2d[i][j]
```

```cpp
/* vector linear em C++ */
matrix[i * columns + j]
```

Nos dois casos, o índice linear é:

```text
índice = i × número_de_colunas + j
```

Na sua implementação, cada ponteiro de linha é preparado assim:

```c
array2d[i] =
    (double *)(array2d + dim1) + i * dim2;
```

Portanto:

```c
array2d[i]
```

aponta para:

```c
dados + i * dim2
```

Isso permite escrever:

```c
array2d[i][j]
```

O `std::vector<double>` não cria esses ponteiros de linha. Por
isso normalmente usamos:

```cpp
std::vector<double> matrix(rows * columns);

matrix[i * columns + j] = 5.0;
```

Também podemos criar temporariamente um ponteiro para uma linha:

```cpp
double *row = matrix.data() + i * columns;

row[j] = 5.0;
```

Isso é praticamente a mesma lógica da sua função:

```c
array2d[i] = data + i * dim2;
```

As diferenças importantes são:

| Sua `new_2d` em C               | `std::vector<double>`                                 |
| ------------------------------- | ----------------------------------------------------- |
| Memória linear para os dados    | Memória linear para os dados                          |
| Possui ponteiros para as linhas | Não possui ponteiros para linhas                      |
| Permite `matrix[i][j]`          | Usa `matrix[i * columns + j]`                         |
| Uma chamada a `malloc`          | Alocação administrada pelo próprio vector             |
| Precisa chamar `free`           | Liberação automática                                  |
| Tamanho fixo após a criação     | Pode crescer com `resize` e `push_back`               |
| Não executa inicialização       | Inicializa os elementos quando construído com tamanho |

Portanto, sim: a região de `double` da sua `new_2d` funciona
como um `std::vector<double>` linear. A diferença é que você
adicionou antes dela uma tabela de ponteiros para obter a
sintaxe confortável `matrix[i][j]`.


No seu algoritmo, `layers_size` e `data_size` correspondem a
duas regiões diferentes dentro do mesmo bloco de memória.

## `layer_1_size`

```c
size_t layer_1_size = dim1 * sizeof(double *);
```

É a quantidade de memória necessária para armazenar os ponteiros das linhas.

Para uma matriz com `dim1 = 3`:

```text
array2d[0]
array2d[1]
array2d[2]
```

Cada elemento é um `double *`, não um `double`.

Supondo um computador de 64 bits, cada ponteiro normalmente
ocupa 8 bytes:

```text
layer_1_size = 3 × 8 = 24 bytes
```

Essa região não armazena os números da matriz. Ela armazena os
endereços onde cada linha começa.

## `layers_size`

Na matriz 2D existe apenas uma camada de ponteiros:

```c
size_t layers_size = layer_1_size;
```

Portanto, nesse caso:

```text
layers_size == layer_1_size
```

O nome plural é útil porque, nas matrizes 3D, 4D etc., existem
várias camadas de ponteiros.

Por exemplo, na matriz 3D:

```c
size_t layers_size = layer_1_size + layer_2_size;
```

## `data_size`

```c
size_t data_size = dim1 * dim2 * sizeof(double);
```

É a quantidade de memória usada pelos valores numéricos da
matriz.

Para uma matriz `3 × 4`, existem:

```text
3 × 4 = 12 valores double
```

Como um `double` normalmente ocupa 8 bytes:

```text
data_size = 3 × 4 × 8 = 96 bytes
```

Essa região contém efetivamente:

```text
matriz[0][0]
matriz[0][1]
matriz[0][2]
matriz[0][3]

matriz[1][0]
matriz[1][1]
matriz[1][2]
matriz[1][3]

matriz[2][0]
matriz[2][1]
matriz[2][2]
matriz[2][3]
```

## Bloco completo

A chamada:

```c
malloc(layers_size + data_size);
```

cria um único bloco contendo as duas regiões:

```text
raw_bytes
   │
   ▼
┌──────────────────────────────┬───────────────────────────────────┐
│ layers_size                  │ data_size                         │
│ ponteiros para as linhas     │ valores double                    │
├──────────────────────────────┼───────────────────────────────────┤
│ array2d[0]                   │ matriz[0][0] matriz[0][1] ...     │
│ array2d[1]                   │ matriz[1][0] matriz[1][1] ...     │
│ array2d[2]                   │ matriz[2][0] matriz[2][1] ...     │
└──────────────────────────────┴───────────────────────────────────┘
```

Para a matriz `3 × 4`, supondo ponteiros e `double` com 8 bytes:

```text
layers_size = 24 bytes
data_size   = 96 bytes
total       = 120 bytes
```

Depois:

```c
double **array2d = (double **)raw_bytes;
```

faz `array2d` apontar para o começo da região dos ponteiros.

A expressão:

```c
(double *)(array2d + dim1)
```

encontra o começo dos dados.

Isso acontece porque:

```c
array2d + dim1
```

avança `dim1` elementos do tipo `double *`:

```text
dim1 × sizeof(double *)
```

Ou seja, avança exatamente `layers_size` bytes.

Por fim:

```c
array2d[i] =
    (double *)(array2d + dim1) + i * dim2;
```

faz cada ponteiro apontar para o começo de sua respectiva linha.

Resumindo:

```text
layers_size = memória de organização
data_size   = memória dos valores
```

`layers_size` guarda os ponteiros que permitem usar
`array2d[i][j]`; `data_size` guarda os números `double`
propriamente ditos.


Em C++, esta declaração:

```cpp
std::vector<std::vector<double>> matriz;
```

representa uma matriz como um vetor de linhas. Cada linha é
outro `std::vector<double>` independente.

Por exemplo, uma matriz com três linhas e quatro colunas:

```cpp
std::vector<std::vector<double>> matriz(
    3,
    std::vector<double>(4)
);
```

Pode ser lida como:

```text
Crie 3 vetores.
Cada vetor contém 4 elementos double.
```

A manipulação usa a sintaxe natural:

```cpp
matriz[linha][coluna] = valor;
```

Exemplo:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::size_t linhas  = 3;
    std::size_t colunas = 4;

    std::vector<std::vector<double>> matriz(
        linhas,
        std::vector<double>(colunas)
    );

    matriz[0][0] = 10.0;
    matriz[1][2] = 20.0;
    matriz[2][3] = 30.0;

    for (const auto& linha : matriz)
    {
        for (double valor : linha)
            std::cout << valor << ' ';

        std::cout << '\n';
    }
}
```

## Organização na memória

Ela não fica organizada como um único bloco de `double`.

O vetor externo contém objetos `std::vector<double>`:

```text
matriz
┌───────────────┐
│ vector linha 0│ ───► [d00][d01][d02][d03]
├───────────────┤
│ vector linha 1│ ───► [d10][d11][d12][d13]
├───────────────┤
│ vector linha 2│ ───► [d20][d21][d22][d23]
└───────────────┘
```

Cada linha possui sua própria alocação de memória.

Portanto:

```cpp
matriz[0].data()
matriz[1].data()
matriz[2].data()
```

apontam para blocos diferentes, que não precisam estar próximos.

Os elementos de cada linha são contíguos:

```cpp
&matriz[i][j + 1] == &matriz[i][j] + 1
```

Mas a última posição de uma linha não precisa estar ao
lado da primeira posição da linha seguinte:

```cpp
&matriz[i + 1][0] != &matriz[i][colunas - 1] + 1
```

## Comparação com sua `new_2d`

| Sua matriz em C                     | `vector<vector<double>>`                                |
| ----------------------------------- | ------------------------------------------------------- |
| Uma única chamada a `malloc`        | Uma alocação para o vetor externo e uma para cada linha |
| Todos os `double` são contíguos     | Somente cada linha é contígua                           |
| Possui uma camada de `double *`     | Possui uma camada de objetos `vector<double>`           |
| Usa `matriz[i][j]`                  | Usa `matriz[i][j]`                                      |
| Todas as linhas têm o mesmo tamanho | Cada linha pode ter tamanho diferente                   |
| Liberação manual com `free`         | Liberação automática                                    |
| Tamanho definido na criação         | Linhas e colunas podem mudar                            |

## Linhas com tamanhos diferentes

Como cada linha é um vetor independente, é possível criar
uma matriz irregular:

```cpp
std::vector<std::vector<double>> matriz = {
    {1.0, 2.0},
    {3.0, 4.0, 5.0},
    {6.0}
};
```

Sua estrutura é:

```text
linha 0: 1.0 2.0
linha 1: 3.0 4.0 5.0
linha 2: 6.0
```

Isso não é propriamente uma matriz retangular; costuma ser
chamado de *jagged array* ou matriz irregular.

## Relação com `layers_size` e `data_size`

Na sua implementação em C, você calcula explicitamente:

```c
layers_size = linhas * sizeof(double *);
data_size   = linhas * colunas * sizeof(double);
```

No `vector<vector<double>>`, a biblioteca faz algo
conceitualmente semelhante, mas com alocações separadas:

```text
camada externa:
linhas × sizeof(vector<double>)

dados:
linha 0 → colunas × sizeof(double)
linha 1 → colunas × sizeof(double)
linha 2 → colunas × sizeof(double)
```

Note que a camada externa não contém somente ponteiros. Ela
contém objetos completos do tipo:

```cpp
std::vector<double>
```

Cada um normalmente mantém informações semelhantes a:

```text
ponteiro para os dados
tamanho atual
capacidade disponível
```

A implementação exata depende da biblioteca padrão.

## `size()` em cada nível

O tamanho externo corresponde ao número de linhas:

```cpp
matriz.size()
```

O tamanho de uma linha corresponde ao número de colunas daquela linha:

```cpp
matriz[i].size()
```

Exemplo:

```cpp
std::cout << "Linhas: " << matriz.size() << '\n';

if (!matriz.empty())
    std::cout << "Colunas: " << matriz[0].size() << '\n';
```

## Verificação de limites

O operador `[]` não verifica se o índice é válido:

```cpp
matriz[i][j]
```

Para verificar os dois índices:

```cpp
matriz.at(i).at(j)
```

Se algum índice estiver fora dos limites, será lançada uma
exceção `std::out_of_range`.

## Conclusão

`std::vector<std::vector<double>>` é simples, seguro e
conveniente para usar `matriz[i][j]`. Entretanto, ele não é
equivalente à sua `new_2d` em relação à organização da memória.

Sua `new_2d` possui todos os números em um único bloco contínuo.
O `vector<vector<double>>` possui um bloco independente para
cada linha. Para cálculos numéricos, interoperabilidade com
bibliotecas C ou melhor previsibilidade de cache, um único
`std::vector<double>` linear geralmente é mais adequado.


Usando `std::vector<std::vector<double>>`, podemos calcular o
determinante pela regra de Sarrus:

```cpp
#include <iostream>
#include <vector>

double determinante3x3(
    const std::vector<std::vector<double>>& matriz)
{
    return
          matriz[0][0] * matriz[1][1] * matriz[2][2]
        + matriz[0][1] * matriz[1][2] * matriz[2][0]
        + matriz[0][2] * matriz[1][0] * matriz[2][1]
        - matriz[0][2] * matriz[1][1] * matriz[2][0]
        - matriz[0][1] * matriz[1][0] * matriz[2][2]
        - matriz[0][0] * matriz[1][2] * matriz[2][1];
}

int main()
{
    std::vector<std::vector<double>> matriz = {
        {1.0, 2.0, 3.0},
        {0.0, 1.0, 4.0},
        {5.0, 6.0, 0.0}
    };

    double determinante = determinante3x3(matriz);

    std::cout << "Matriz:\n";

    for (const auto& linha : matriz)
    {
        for (double valor : linha)
            std::cout << valor << '\t';

        std::cout << '\n';
    }

    std::cout << "\nDeterminante = "
              << determinante << '\n';

    return 0;
}
```

Saída:

```text
Matriz:
1    2    3
0    1    4
5    6    0

Determinante = 1
```

O cálculo realizado é:

$$
\det(A)=
1(1)(0)+2(4)(5)+3(0)(6)
-3(1)(5)-2(0)(0)-1(4)(6)
$$

$$
\det(A)=0+40+0-15-0-24=1
$$


Para uma matriz de ordem genérica, uma solução eficiente é usar
eliminação de Gauss com pivotamento parcial.

Para reduzir alocações:

* armazenamos a matriz em um único `std::vector<double>`;
* a função trabalha diretamente sobre a matriz;
* nenhuma matriz auxiliar é criada;
* usamos apenas algumas variáveis locais;
* complexidade: \(O(n^3)\);
* memória adicional: \(O(1)\).

```cpp
#include <cmath>
#include <cstddef>
#include <iostream>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

double determinante(std::span<double> matriz, std::size_t ordem)
{
    if (ordem == 0 || matriz.size() != ordem * ordem)
        throw std::invalid_argument("Dimensoes invalidas");

    double resultado = 1.0;
    int sinal = 1;

    for (std::size_t coluna = 0; coluna < ordem; ++coluna)
    {
        /*
         * Procura o maior pivô, em módulo, na coluna.
         */
        std::size_t linha_pivo = coluna;
        double maior = std::abs(matriz[coluna * ordem + coluna]);

        for (std::size_t linha = coluna + 1; linha < ordem; ++linha)
        {
            double valor =
                std::abs(matriz[linha * ordem + coluna]);

            if (valor > maior)
            {
                maior      = valor;
                linha_pivo = linha;
            }
        }

        /*
         * Coluna nula: determinante igual a zero.
         */
        if (maior == 0.0)
            return 0.0;

        /*
         * Troca a linha atual pela linha do pivô.
         */
        if (linha_pivo != coluna)
        {
            for (std::size_t j = 0; j < ordem; ++j)
            {
                std::swap(
                    matriz[coluna * ordem + j],
                    matriz[linha_pivo * ordem + j]
                );
            }

            sinal = -sinal;
        }

        double pivo = matriz[coluna * ordem + coluna];

        resultado *= pivo;

        /*
         * Elimina os elementos abaixo do pivô.
         */
        for (std::size_t linha = coluna + 1;
             linha < ordem;
             ++linha)
        {
            double fator =
                matriz[linha * ordem + coluna] / pivo;

            matriz[linha * ordem + coluna] = 0.0;

            for (std::size_t j = coluna + 1; j < ordem; ++j)
            {
                matriz[linha * ordem + j] -=
                    fator * matriz[coluna * ordem + j];
            }
        }
    }

    return sinal * resultado;
}

int main()
{
    constexpr std::size_t ordem = 3;

    std::vector<double> matriz = {
        1.0, 2.0, 3.0,
        0.0, 1.0, 4.0,
        5.0, 6.0, 0.0
    };

    std::cout << "Determinante = "
              << determinante(matriz, ordem)
              << '\n';
}
```

Resultado:

```text
Determinante = 1
```

O acesso lógico `matriz[i][j]` foi substituído por:

```cpp
matriz[i * ordem + j]
```

A função recebe um `std::span<double>`, que é apenas uma visão
sobre a memória do `vector`. Ele não copia nem aloca os
elementos.

Atenção: para evitar uma cópia, a função transforma a matriz
original em uma matriz triangular. Se quiser preservar os
valores originais, será inevitável criar uma cópia:

```cpp
std::vector<double> copia = matriz;

double det = determinante(copia, ordem);
```

A principal otimização em relação à expansão por cofatores é a
complexidade:
| Método                 |              Complexidade |
| ---------------------- | ------------------------: |
| Expansão por cofatores | aproximadamente \(O(n!)\) |
| Eliminação de Gauss    |                \(O(n^3)\) |

Para matrizes maiores, a diferença é enorme.


`std::span<double>` representa uma visão sobre uma sequência
contínua de objetos `double`.

Ele não aloca memória, não copia elementos e não é dono dos
dados. Apenas guarda, conceitualmente:

```cpp
double*     ponteiro;
std::size_t tamanho;
```

Está disponível a partir do C++20:

```cpp
#include <span>
```

## Exemplo básico

```cpp
#include <iostream>
#include <span>
#include <vector>

void imprimir(std::span<double> valores)
{
    for (double valor : valores)
        std::cout << valor << ' ';

    std::cout << '\n';
}

int main()
{
    std::vector<double> vector = {
        10.0, 20.0, 30.0, 40.0
    };

    imprimir(vector);
}
```

Embora a função receba:

```cpp
std::span<double>
```

podemos passar diretamente:

```cpp
std::vector<double>
```

O `span` passa a apontar para os elementos do `vector`:

```text
vector
┌───────────────────────────────────┐
│ 10.0 │ 20.0 │ 30.0 │ 40.0         │
└───────────────────────────────────┘
    ▲
    │
span: ponteiro + tamanho
```

Nenhum `double` é copiado.

## Relação com ponteiro e tamanho

Em C, provavelmente escreveríamos:

```c
void imprimir(double *valores, size_t tamanho);
```

Em C++20, podemos escrever:

```cpp
void imprimir(std::span<double> valores);
```

O `span` reúne o ponteiro e o tamanho no mesmo objeto:

```cpp
valores.data()  // double*
valores.size()  // quantidade de elementos
```

Exemplo:

```cpp
void imprimir(std::span<double> valores)
{
    double* ponteiro = valores.data();
    std::size_t tamanho = valores.size();

    for (std::size_t i = 0; i < tamanho; ++i)
        std::cout << ponteiro[i] << '\n';
}
```

## O `span` pode modificar os dados

Como usamos:

```cpp
std::span<double>
```

os elementos podem ser alterados:

```cpp
void zerar(std::span<double> valores)
{
    for (double& valor : valores)
        valor = 0.0;
}
```

Uso:

```cpp
std::vector<double> vector = {
    1.0, 2.0, 3.0
};

zerar(vector);
```

Depois da chamada, o próprio `vector` contém:

```text
0.0 0.0 0.0
```

Isso acontece porque o `span` aponta para a memória original.

## `span<const double>`

Se a função deve apenas ler os elementos, usamos:

```cpp
std::span<const double>
```

Exemplo:

```cpp
double soma(std::span<const double> valores)
{
    double resultado = 0.0;

    for (double valor : valores)
        resultado += valor;

    return resultado;
}
```

Agora isto não é permitido:

```cpp
valores[0] = 10.0; // erro
```

O `span` continua sem ser dono da memória, mas oferece uma visão
somente para leitura.

## Fontes de dados aceitas

Um `span` pode visualizar diferentes contêineres contíguos.

### `std::vector`

```cpp
std::vector<double> valores = {1.0, 2.0, 3.0};

std::span<double> view = valores;
```

### `std::array`

```cpp
#include <array>

std::array<double, 3> valores = {
    1.0, 2.0, 3.0
};

std::span<double> view = valores;
```

### Array comum

```cpp
double valores[] = {
    1.0, 2.0, 3.0
};

std::span<double> view = valores;
```

### Ponteiro e tamanho

```cpp
double *dados = /* memória alocada */;
std::size_t tamanho = 10;

std::span<double> view(dados, tamanho);
```

Isso é especialmente útil para sua matriz criada com `malloc`:

```cpp
double *dados = /* início dos valores */;
std::span<double> view(dados, linhas * colunas);
```

## Acesso aos elementos

O acesso é parecido com `std::vector`:

```cpp
span[0]
span[1]
span[i]
```

Também podemos percorrê-lo:

```cpp
for (double valor : span)
{
    std::cout << valor << '\n';
}
```

E acessar seus dados:

```cpp
span.front();
span.back();
span.data();
span.size();
span.empty();
```

Diferentemente de `std::vector`, o `std::span` não possui:

```cpp
push_back()
resize()
reserve()
```

Isso ocorre porque ele não administra a memória.

## Sub-regiões

Um `span` pode criar uma visão de apenas uma parte dos dados
sem copiar nada:

```cpp
std::vector<double> valores = {
    10.0, 20.0, 30.0, 40.0, 50.0
};

std::span<double> view = valores;

std::span<double> parte = view.subspan(1, 3);
```

`parte` representa:

```text
20.0 30.0 40.0
```

Alterar `parte` altera o `vector` original:

```cpp
parte[0] = 99.0;
```

Agora:

```text
valores = 10.0 99.0 30.0 40.0 50.0
```

## Uso na matriz

Na função do determinante:

```cpp
double determinante(
    std::span<double> matriz,
    std::size_t ordem
)
```

quando fazemos:

```cpp
std::vector<double> matriz = {
    1.0, 2.0, 3.0,
    0.0, 1.0, 4.0,
    5.0, 6.0, 0.0
};

determinante(matriz, 3);
```

o `span` visualiza diretamente os nove elementos:

```text
span
  │
  ▼
[1][2][3][0][1][4][5][6][0]
```

O índice da posição `[i][j]` é:

```cpp
matriz[i * ordem + j]
```

## Tempo de vida

O cuidado mais importante é que o `span` não mantém os dados vivos.

Isto é perigoso:

```cpp
std::span<double> view;

{
    std::vector<double> valores = {
        1.0, 2.0, 3.0
    };

    view = valores;
} // valores é destruído aqui

// view agora aponta para memória que não existe mais
```

Também é necessário cuidado quando o `vector` realoca sua memória:

```cpp
std::vector<double> valores = {1.0, 2.0};

std::span<double> view = valores;

valores.push_back(3.0); // pode realocar

// view pode ter se tornado inválido
```

## Comparação direta

| Recurso                   | `std::vector<double>` | `std::span<double>` |
| ------------------------- | --------------------- | ------------------- |
| É dono dos dados          | Sim                   | Não                 |
| Aloca memória             | Sim                   | Não                 |
| Libera memória            | Sim                   | Não                 |
| Conhece o tamanho         | Sim                   | Sim                 |
| Pode alterar elementos    | Sim                   | Sim                 |
| Pode mudar de tamanho     | Sim                   | Não                 |
| Cópia copia os elementos  | Sim                   | Não                 |
| Serve como parâmetro leve | Sim, por referência   | Sim                 |

Em resumo:

```cpp
std::vector<double>
```

é o proprietário da memória, enquanto:

```cpp
std::span<double>
```

é apenas uma janela temporária para observar ou alterar
essa memória.

Você tem toda razão no seu ponto: **os dados dos elementos
(`double`) são 100% contíguos na memória** porque você
fez um único `malloc` para o bloco inteiro.

A minha afirmação foi imprecisa. O que não é contíguo
**do ponto de vista do tipo de dado em C/C++** não é a
região de memória em si, mas sim **a forma como a linguagem
interpreta a caminhada do ponteiro `double****`.

Para deixar isso perfeitamente claro, vale a pena separar a
memória física do tipo de dado:

---

### 1. O Layout Físico na Memória (Tudo Contíguo)

A sua estratégia de alocação coloca o vetor de ponteiros
e a matriz de dados em uma **única tripa contígua de bytes**:

```text
[  Ponteiros (Linhas)  ] [                 Dados Contíguos (double)                 ]
┌───────┬───────┬───────┼───────┬───────┬───────┬───────┬───────┬───────┬───┬───────┐
│ptr[0] │ptr[1] │ptr[2] │  0.0  │  1.5  │  2.3  │  4.1  │  5.0  │  6.2  │...│  N.N  │
└───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───┴───────┘
^                       ^
raw_bytes               array2d[0] (início real dos dados)

```

Fisicamente, a memória é 100% contígua do início ao fim do bloco.

---

### 2. Por que o tipo `double**` impede o avanço direto de um Iterador?

Embora a memória esteja colada, a aritmética de ponteiros
depende exclusivamente do **tipo da variável** no C++:

* Se você tem um ponteiro `double* p = matrix[0]`, ao fazer
`p++`, o compilador avança exatamente `sizeof(double)`
(8 bytes). **Isso caminha perfeitamente pela matriz.**
* Se você tenta usar a variável `double** p = matrix`
diretamente como iterador e faz `p++`, o compilador avança
`sizeof(double*)` (8 bytes de um ponteiro), navegando apenas no
**vetor de cabeçalhos de linha**, e não pelos dados `double`.

---

### O Ajuste no Nosso Raciocínio

Portanto, a sua estrutura de alocação é excelente e
**perfeita para iteradores**, desde que a gente passe para o
`iterator_traits` o ponteiro para o primeiro elemento real
dos dados (`double*`), e não a cabeça da matriz (`double**`).

Se pegarmos o ponteiro dos dados `matrix[0]`, o seu `malloc`
único garante que podemos percorrer a matriz inteira de
0 até $dim1 \times dim2 - 1$ como uma única sequência
contígua $O(1)$:

```cpp
// Isso funciona perfeitamente DEVIDO ao seu malloc contíguo:
double* begin = matrix[0];
double* end   = matrix[0] + (dim1 * dim2);

// custom::distance e custom::advance funcionam em O(1)
auto total_elementos = custom::distance(begin, end);

```

O seu truque de calcular `layers_size + data_size` em um único
`malloc` é exatamente a forma otimizada usada em bibliotecas
de alto desempenho em C para evitar a fragmentação de
memória e garantir *cache locality*.
