Perfeito! Você está falando de **template metaprogramming**
com valores em tempo de compilação. Vou te mostrar exemplos
simples usando `std::integral_constant` e templates com
valores.

## Exemplo 1: O Dobro (mais simples)

```cpp
#include <iostream>

// Template que calcula o dobro em tempo de compilação
template<int N>
struct Dobro {
    static constexpr int valor = N * 2;
};

int main() {
    // Usando o template com diferentes valores
    std::cout << "Dobro de 5: " << Dobro<5>::valor << std::endl;    // 10
    std::cout << "Dobro de 10: " << Dobro<10>::valor << std::endl;  // 20
    std::cout << "Dobro de 7: " << Dobro<7>::valor << std::endl;    // 14

    return 0;
}
```

## Exemplo 2: Quadrado

```cpp
#include <iostream>

template<int N>
struct Quadrado {
    static constexpr int valor = N * N;
};

int main() {
    std::cout << "Quadrado de 4: " << Quadrado<4>::valor << std::endl;   // 16
    std::cout << "Quadrado de 7: " << Quadrado<7>::valor << std::endl;   // 49
    std::cout << "Quadrado de 10: " << Quadrado<10>::valor << std::endl; // 100

    return 0;
}
```

## Exemplo 3: Potência (usando recursão em templates)

```cpp
#include <iostream>

// Caso base: potência com expoente 0 = 1
template<int Base, int Expoente>
struct Potencia {
    static constexpr int valor = Base * Potencia<Base, Expoente - 1>::valor;
};

// Especialização para expoente 0
template<int Base>
struct Potencia<Base, 0> {
    static constexpr int valor = 1;
};

int main() {
    std::cout << "2^3 = " << Potencia<2, 3>::valor << std::endl;    // 8
    std::cout << "3^4 = " << Potencia<3, 4>::valor << std::endl;    // 81
    std::cout << "5^2 = " << Potencia<5, 2>::valor << std::endl;    // 25
    std::cout << "10^0 = " << Potencia<10, 0>::valor << std::endl;  // 1

    return 0;
}
```

## Exemplo 4: Número Par ou Ímpar

```cpp
#include <iostream>

template<int N>
struct ParOuImpar {
    static constexpr const char* valor = (N % 2 == 0) ? "Par" : "Ímpar";
};

int main() {
    std::cout << "4 é: " << ParOuImpar<4>::valor << std::endl;   // Par
    std::cout << "7 é: " << ParOuImpar<7>::valor << std::endl;   // Ímpar
    std::cout << "10 é: " << ParOuImpar<10>::valor << std::endl; // Par
    std::cout << "3 é: " << ParOuImpar<3>::valor << std::endl;   // Ímpar

    return 0;
}
```

## Exemplo 5: Maior entre dois números

```cpp
#include <iostream>

template<int A, int B>
struct Maior {
    static constexpr int valor = (A > B) ? A : B;
};

int main() {
    std::cout << "Maior entre 5 e 8: " << Maior<5, 8>::valor << std::endl;   // 8
    std::cout << "Maior entre 10 e 3: " << Maior<10, 3>::valor << std::endl; // 10
    std::cout << "Maior entre 7 e 7: " << Maior<7, 7>::valor << std::endl;   // 7

    return 0;
}
```

## Exemplo 6: Soma de 1 até N (usando recursão)

```cpp
#include <iostream>

// Caso base: soma de 1 até 1 = 1
template<int N>
struct SomaAte {
    static constexpr int valor = N + SomaAte<N - 1>::valor;
};

// Especialização para N = 1
template<>
struct SomaAte<1> {
    static constexpr int valor = 1;
};

int main() {
    std::cout << "Soma de 1 a 5: " << SomaAte<5>::valor << std::endl;   // 15
    std::cout << "Soma de 1 a 10: " << SomaAte<10>::valor << std::endl; // 55
    std::cout << "Soma de 1 a 3: " << SomaAte<3>::valor << std::endl;   // 6

    return 0;
}
```

## Exemplo 7: Usando `std::integral_constant` (forma moderna)

```cpp
#include <iostream>
#include <type_traits>

// Usando std::integral_constant
template<int N>
using Dobro = std::integral_constant<int, N * 2>;

template<int N>
using Triplo = std::integral_constant<int, N * 3>;

int main() {
    // Acesso direto via ::value
    std::cout << "Dobro de 5: " << Dobro<5>::value << std::endl;   // 10
    std::cout << "Triplo de 4: " << Triplo<4>::value << std::endl; // 12

    // Pode ser usado em tempo de compilação
    constexpr int x = Dobro<7>::value;  // 14
    std::cout << "Dobro de 7: " << x << std::endl;

    return 0;
}
```

## Exemplo 8: Combinando templates (mais avançado)

```cpp
#include <iostream>

// Template para dobrar
template<int N>
struct Dobro {
    static constexpr int valor = N * 2;
};

// Template para triplicar
template<int N>
struct Triplo {
    static constexpr int valor = N * 3;
};

// Template que usa outro template
template<int N>
struct OperacaoComposta {
    // Primeiro dobra, depois triplica o resultado
    static constexpr int valor = Triplo<Dobro<N>::valor>::valor;
};

int main() {
    std::cout << "Dobro de 5: " << Dobro<5>::valor << std::endl;           // 10
    std::cout << "Triplo de 5: " << Triplo<5>::valor << std::endl;         // 15
    std::cout << "Dobro depois triplo de 5: "
              << OperacaoComposta<5>::valor << std::endl;                 // 30 (5*2*3)

    return 0;
}
```

## Exemplo 9: Usando com `constexpr` (C++11/14)

```cpp
#include <iostream>

// Função constexpr (mais simples que template)
constexpr int dobro(int n) {
    return n * 2;
}

// Template que usa função constexpr
template<int N>
struct UsandoConstexpr {
    static constexpr int valor = dobro(N);
};

int main() {
    std::cout << "Dobro de 6: " << UsandoConstexpr<6>::valor << std::endl; // 12
    std::cout << "Dobro de 8: " << UsandoConstexpr<8>::valor << std::endl; // 16

    // Também pode ser usado em tempo de execução
    int x = dobro(10);
    std::cout << "Dobro de 10: " << x << std::endl; // 20

    return 0;
}
```

## Exemplo 10: Versão mais genérica com qualquer tipo

```cpp
#include <iostream>

// Template que funciona com qualquer tipo numérico
template<typename T, T N>
struct Valor {
    static constexpr T dobro = N * 2;
    static constexpr T quadrado = N * N;
};

int main() {
    // Com inteiros
    std::cout << "Dobro de 5 (int): " << Valor<int, 5>::dobro << std::endl;     // 10
    std::cout << "Quadrado de 5 (int): " << Valor<int, 5>::quadrado << std::endl; // 25

    // Com double
    std::cout << "Dobro de 3.5 (double): " << Valor<double, 3.5>::dobro << std::endl;     // 7
    std::cout << "Quadrado de 3.5 (double): " << Valor<double, 3.5>::quadrado << std::endl; // 12.25

    return 0;
}
```

## Resumo da sintaxe

```cpp
// Estrutura básica
template<int N>
struct MeuTemplate {
    static constexpr int valor = N * 2;  // Cálculo em tempo de compilação
};

// Uso
MeuTemplate<5>::valor;  // Retorna 10 em tempo de compilação

// Com std::integral_constant (moderno)
template<int N>
using MeuTemplate = std::integral_constant<int, N * 2>;

MeuTemplate<5>::value;  // Retorna 10
```

## Quando usar isso?

1. **Cálculos em tempo de compilação** - para otimização
2. **Metaprogramação** - para gerar código baseado em valores
3. **Constantes configuráveis** - como tamanhos de arrays
4. **Condições em tempo de compilação** - com `std::enable_if`

```cpp
// Exemplo prático: array com tamanho calculado em tempo de compilação
int array[Dobro<5>::valor];  // array de 10 elementos
```

A beleza dos templates com valores é que tudo é calculado
**em tempo de compilação**, sem custo em tempo de
execução! 🚀

Perfeito! Vou criar um template que converte Fahrenheit para
Celsius em tempo de compilação.

## Exemplo 1: Conversão Fahrenheit → Celsius (básico)

```cpp
#include <iostream>

// Template para converter Fahrenheit para Celsius
// Fórmula: C = (F - 32) * 5/9
template<int Fahrenheit>
struct FahrenheitParaCelsius {
    // Usamos double para ter precisão
    static constexpr double valor = (Fahrenheit - 32) * 5.0 / 9.0;
};

int main() {
    std::cout << "32°F = " << FahrenheitParaCelsius<32>::valor << "°C" << std::endl;   // 0°C
    std::cout << "68°F = " << FahrenheitParaCelsius<68>::valor << "°C" << std::endl;   // 20°C
    std::cout << "100°F = " << FahrenheitParaCelsius<100>::valor << "°C" << std::endl; // 37.7778°C
    std::cout << "212°F = " << FahrenheitParaCelsius<212>::valor << "°C" << std::endl; // 100°C
    std::cout << "0°F = " << FahrenheitParaCelsius<0>::valor << "°C" << std::endl;     // -17.7778°C

    return 0;
}
```

## Exemplo 2: Usando `std::integral_constant` (moderno)

```cpp
#include <iostream>
#include <type_traits>

// Usando integral_constant para inteiros (resultado em décimos para evitar ponto flutuante)
template<int Fahrenheit>
using FahrenheitParaCelsius = std::integral_constant<int, (Fahrenheit - 32) * 5 / 9>;

int main() {
    std::cout << "32°F = " << FahrenheitParaCelsius<32>::value << "°C" << std::endl;   // 0°C
    std::cout << "68°F = " << FahrenheitParaCelsius<68>::value << "°C" << std::endl;   // 20°C
    std::cout << "212°F = " << FahrenheitParaCelsius<212>::value << "°C" << std::endl; // 100°C

    return 0;
}
```

## Exemplo 3: Com precisão de décimos (mais preciso)

```cpp
#include <iostream>

// Retorna o valor em décimos de grau (ex: 20.5°C = 205)
template<int Fahrenheit>
struct FahrenheitParaCelsiusDecimos {
    // Multiplicamos por 10 para ter 1 casa decimal
    static constexpr int valor = ((Fahrenheit - 32) * 50) / 9; // * 5/9 * 10 = * 50/9
};

int main() {
    std::cout << "32°F = " << FahrenheitParaCelsiusDecimos<32>::valor / 10.0 << "°C" << std::endl;     // 0°C
    std::cout << "68°F = " << FahrenheitParaCelsiusDecimos<68>::valor / 10.0 << "°C" << std::endl;     // 20°C
    std::cout << "100°F = " << FahrenheitParaCelsiusDecimos<100>::valor / 10.0 << "°C" << std::endl;   // 37.7°C
    std::cout << "98.6°F = " << FahrenheitParaCelsiusDecimos<99>::valor / 10.0 << "°C" << std::endl;   // 37.2°C (aproximado)
    std::cout << "0°F = " << FahrenheitParaCelsiusDecimos<0>::valor / 10.0 << "°C" << std::endl;       // -17.7°C

    return 0;
}
```

## Exemplo 4: Versão com `double` e `constexpr` (C++14)

```cpp
#include <iostream>

// Função constexpr para converter (mais simples)
constexpr double fahrenheitParaCelsius(double fahrenheit) {
    return (fahrenheit - 32) * 5.0 / 9.0;
}

// Template que usa a função constexpr
template<double Fahrenheit>
struct Conversor {
    static constexpr double valor = fahrenheitParaCelsius(Fahrenheit);
};

int main() {
    std::cout << "32°F = " << Conversor<32.0>::valor << "°C" << std::endl;     // 0°C
    std::cout << "68°F = " << Conversor<68.0>::valor << "°C" << std::endl;     // 20°C
    std::cout << "98.6°F = " << Conversor<98.6>::valor << "°C" << std::endl;   // 37°C

    return 0;
}
```

## Exemplo 5: Versão completa com ambas conversões

```cpp
#include <iostream>

// Template para Fahrenheit -> Celsius
template<int F>
struct FtoC {
    static constexpr double valor = (F - 32) * 5.0 / 9.0;
};

// Template para Celsius -> Fahrenheit
template<int C>
struct CtoF {
    static constexpr double valor = (C * 9.0 / 5.0) + 32;
};

// Template com precisão de décimos
template<int F>
struct FtoCDecimos {
    static constexpr int valor = ((F - 32) * 50) / 9; // * 5/9 * 10
};

int main() {
    std::cout << "=== CONVERSOR DE TEMPERATURA (Tempo de Compilação) ===\n\n";

    std::cout << "Fahrenheit → Celsius:\n";
    std::cout << "  32°F  = " << FtoC<32>::valor << "°C" << std::endl;
    std::cout << "  68°F  = " << FtoC<68>::valor << "°C" << std::endl;
    std::cout << "  100°F = " << FtoC<100>::valor << "°C" << std::endl;
    std::cout << "  212°F = " << FtoC<212>::valor << "°C" << std::endl;
    std::cout << "  0°F   = " << FtoC<0>::valor << "°C" << std::endl;
    std::cout << "  -40°F = " << FtoC<-40>::valor << "°C" << std::endl; // -40°C (ponto de igualdade)

    std::cout << "\nCelsius → Fahrenheit:\n";
    std::cout << "  0°C   = " << CtoF<0>::valor << "°F" << std::endl;
    std::cout << "  20°C  = " << CtoF<20>::valor << "°F" << std::endl;
    std::cout << "  37°C  = " << CtoF<37>::valor << "°F" << std::endl;
    std::cout << "  100°C = " << CtoF<100>::valor << "°F" << std::endl;
    std::cout << "  -40°C = " << CtoF<-40>::valor << "°F" << std::endl;

    std::cout << "\nCom precisão de 1 casa decimal (F→C):\n";
    std::cout << "  98.6°F ≈ " << FtoCDecimos<99>::valor / 10.0 << "°C" << std::endl;
    std::cout << "  72°F   ≈ " << FtoCDecimos<72>::valor / 10.0 << "°C" << std::endl;

    return 0;
}
```

## Exemplo 6: Usando `std::integral_constant` com dupla precisão

```cpp
#include <iostream>
#include <type_traits>

// Para inteiros (resultado truncado)
template<int F>
using FtoCInt = std::integral_constant<int, (F - 32) * 5 / 9>;

// Para doubles (usando struct personalizado)
template<double F>
struct FtoCDouble {
    static constexpr double value = (F - 32) * 5.0 / 9.0;
};

int main() {
    // Versão com inteiros (truncado)
    std::cout << "Versão Inteira (truncada):\n";
    std::cout << "  32°F  = " << FtoCInt<32>::value << "°C" << std::endl;
    std::cout << "  100°F = " << FtoCInt<100>::value << "°C" << std::endl;

    // Versão com double (precisa)
    std::cout << "\nVersão Double (precisa):\n";
    std::cout << "  32.0°F  = " << FtoCDouble<32.0>::value << "°C" << std::endl;
    std::cout << "  100.0°F = " << FtoCDouble<100.0>::value << "°C" << std::endl;
    std::cout << "  98.6°F  = " << FtoCDouble<98.6>::value << "°C" << std::endl;

    return 0;
}
```

## Exemplo 7: Tabela de conversão (usando recursão em templates)

```cpp
#include <iostream>

// Template para converter e imprimir uma faixa de valores
template<int Inicio, int Fim>
struct TabelaConversao {
    static void imprimir() {
        // Imprime a conversão do valor atual
        double celsius = (Inicio - 32) * 5.0 / 9.0;
        std::cout << "  " << Inicio << "°F = " << celsius << "°C" << std::endl;

        // Recursão para o próximo valor
        TabelaConversao<Inicio + 1, Fim>::imprimir();
    }
};

// Caso base: quando Inicio > Fim, para a recursão
template<int Inicio, int Fim>
struct TabelaConversao<Inicio, Fim> {
    static void imprimir() {
        // Não faz nada (condição de parada)
    }
};

// Especialização para quando Inicio == Fim (imprime o último)
template<int N>
struct TabelaConversao<N, N> {
    static void imprimir() {
        double celsius = (N - 32) * 5.0 / 9.0;
        std::cout << "  " << N << "°F = " << celsius << "°C" << std::endl;
    }
};

int main() {
    std::cout << "=== TABELA DE CONVERSÃO (32°F a 40°F) ===\n";
    TabelaConversao<32, 40>::imprimir();

    std::cout << "\n=== TABELA DE CONVERSÃO (212°F a 220°F) ===\n";
    TabelaConversao<212, 220>::imprimir();

    return 0;
}
```

## Exemplo 8: Versão mais simples e didática

```cpp
#include <iostream>

// O mais simples possível!
template<int F>
struct Celsius {
    static constexpr double valor = (F - 32) * 5.0 / 9.0;
};

int main() {
    // Entrada: temperatura em Fahrenheit (via template)
    // Saída: temperatura em Celsius (calculada em tempo de compilação)

    std::cout << "Digite um valor para testar (use constantes no código):\n\n";

    // Exemplos de uso
    constexpr int f1 = 32;
    constexpr int f2 = 68;
    constexpr int f3 = 100;
    constexpr int f4 = 212;

    std::cout << f1 << "°F = " << Celsius<f1>::valor << "°C" << std::endl;
    std::cout << f2 << "°F = " << Celsius<f2>::valor << "°C" << std::endl;
    std::cout << f3 << "°F = " << Celsius<f3>::valor << "°C" << std::endl;
    std::cout << f4 << "°F = " << Celsius<f4>::valor << "°C" << std::endl;

    // Você pode mudar o valor aqui e recompilar
    constexpr int minhaTemp = 75;
    std::cout << "\nMinha temperatura: " << minhaTemp << "°F = "
              << Celsius<minhaTemp>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 9: Com entrada do usuário (runtime) + template

```cpp
#include <iostream>

// Template para conversão (tempo de compilação)
template<int F>
struct FtoC {
    static constexpr double valor = (F - 32) * 5.0 / 9.0;
};

// Função para conversão em tempo de execução
double ftoC_runtime(double f) {
    return (f - 32) * 5.0 / 9.0;
}

int main() {
    std::cout << "=== CONVERSOR F° → C° ===\n\n";

    // Usando template (valores conhecidos em tempo de compilação)
    std::cout << "Valores em tempo de compilação:\n";
    std::cout << "  32°F  = " << FtoC<32>::valor << "°C" << std::endl;
    std::cout << "  212°F = " << FtoC<212>::valor << "°C" << std::endl;
    std::cout << "  100°F = " << FtoC<100>::valor << "°C" << std::endl;

    // Usando função runtime (entrada do usuário)
    std::cout << "\nDigite uma temperatura em °F: ";
    double f;
    std::cin >> f;

    double c = ftoC_runtime(f);
    std::cout << f << "°F = " << c << "°C" << std::endl;

    return 0;
}
```

## Saída esperada do Exemplo 8:

```
Digite um valor para testar (use constantes no código):

32°F = 0°C
68°F = 20°C
100°F = 37.7778°C
212°F = 100°C

Minha temperatura: 75°F = 23.8889°C
```

## Resumo

| Template | O que faz | Uso |
|----------|-----------|-----|
| `FahrenheitParaCelsius<int F>` | Converte F→C com double | `FahrenheitParaCelsius<32>::valor` |
| `FtoCInt<int F>` | Converte F→C com int (truncado) | `FtoCInt<32>::value` |
| `FtoCDecimos<int F>` | Converte com 1 casa decimal | `FtoCDecimos<98>::valor / 10.0` |
| `Celsius<int F>` | Versão mais simples | `Celsius<32>::valor` |

A vantagem de usar templates é que a conversão é feita **em tempo de compilação**, sem custo em tempo de execução! 🚀

Ótima observação! Você está certo, usei apenas `int`, `float` e `double`. Vou criar exemplos usando **todos os tipos numéricos** que você pode imaginar!

## Exemplo 1: Convertendo diferentes tipos numéricos

```cpp
#include <iostream>
#include <type_traits>

// Template que aceita QUALQUER tipo numérico
template<typename T, T Fahrenheit>
struct FtoC {
    static constexpr T valor = (Fahrenheit - static_cast<T>(32)) * static_cast<T>(5.0) / static_cast<T>(9.0);
};

int main() {
    std::cout << "=== CONVERSÃO COM DIFERENTES TIPOS ===\n\n";

    // 1. int (inteiro)
    std::cout << "int: 32°F = " << FtoC<int, 32>::valor << "°C" << std::endl;

    // 2. unsigned int
    std::cout << "unsigned int: 32°F = " << FtoC<unsigned int, 32>::valor << "°C" << std::endl;

    // 3. long
    std::cout << "long: 32°F = " << FtoC<long, 32>::valor << "°C" << std::endl;

    // 4. long long
    std::cout << "long long: 32°F = " << FtoC<long long, 32>::valor << "°C" << std::endl;

    // 5. float
    std::cout << "float: 98.6°F = " << FtoC<float, 98.6f>::valor << "°C" << std::endl;

    // 6. double
    std::cout << "double: 98.6°F = " << FtoC<double, 98.6>::valor << "°C" << std::endl;

    // 7. long double
    std::cout << "long double: 98.6°F = " << FtoC<long double, 98.6L>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 2: Especialização para diferentes tipos

```cpp
#include <iostream>
#include <type_traits>

// Template principal (para tipos inteiros)
template<typename T, T N>
struct FtoC {
    static constexpr double valor = (static_cast<double>(N) - 32.0) * 5.0 / 9.0;
};

// Especialização para float
template<float N>
struct FtoC<float, N> {
    static constexpr float valor = (N - 32.0f) * 5.0f / 9.0f;
};

// Especialização para double
template<double N>
struct FtoC<double, N> {
    static constexpr double valor = (N - 32.0) * 5.0 / 9.0;
};

// Especialização para long double
template<long double N>
struct FtoC<long double, N> {
    static constexpr long double valor = (N - 32.0L) * 5.0L / 9.0L;
};

int main() {
    std::cout << "=== ESPECIALIZAÇÃO POR TIPO ===\n\n";

    // Cada tipo usa sua própria especialização
    std::cout << "int: 32°F = " << FtoC<int, 32>::valor << "°C" << std::endl;
    std::cout << "float: 98.6°F = " << FtoC<float, 98.6f>::valor << "°C" << std::endl;
    std::cout << "double: 98.6°F = " << FtoC<double, 98.6>::valor << "°C" << std::endl;
    std::cout << "long double: 98.6°F = " << FtoC<long double, 98.6L>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 3: Usando `std::common_type` para flexibilidade

```cpp
#include <iostream>
#include <type_traits>

// Template que usa o tipo comum entre os operandos
template<typename T>
struct FtoC {
    // O resultado terá o tipo mais "capaz" entre T e double
    using ResultType = typename std::common_type<T, double>::type;

    static ResultType converter(T fahrenheit) {
        return (static_cast<ResultType>(fahrenheit) - 32.0) * 5.0 / 9.0;
    }
};

int main() {
    std::cout << "=== USANDO std::common_type ===\n\n";

    // Funciona com qualquer tipo
    std::cout << "int: 32°F = " << FtoC<int>::converter(32) << "°C" << std::endl;
    std::cout << "float: 98.6°F = " << FtoC<float>::converter(98.6f) << "°C" << std::endl;
    std::cout << "double: 100.0°F = " << FtoC<double>::converter(100.0) << "°C" << std::endl;
    std::cout << "long double: 212.0°F = " << FtoC<long double>::converter(212.0L) << "°C" << std::endl;

    return 0;
}
```

## Exemplo 4: Todos os tipos inteiros

```cpp
#include <iostream>
#include <type_traits>

// Template para converter QUALQUER tipo inteiro
template<typename T, T Fahrenheit>
struct FtoCInteiro {
    // Usa double para precisão na conversão
    static constexpr double valor = (static_cast<double>(Fahrenheit) - 32.0) * 5.0 / 9.0;
};

int main() {
    std::cout << "=== TODOS OS TIPOS INTEIROS ===\n\n";

    // Tipos inteiros com sinal
    std::cout << "short: 32°F = " << FtoCInteiro<short, 32>::valor << "°C" << std::endl;
    std::cout << "int: 32°F = " << FtoCInteiro<int, 32>::valor << "°C" << std::endl;
    std::cout << "long: 32°F = " << FtoCInteiro<long, 32>::valor << "°C" << std::endl;
    std::cout << "long long: 32°F = " << FtoCInteiro<long long, 32>::valor << "°C" << std::endl;

    // Tipos inteiros sem sinal
    std::cout << "unsigned short: 32°F = " << FtoCInteiro<unsigned short, 32>::valor << "°C" << std::endl;
    std::cout << "unsigned int: 32°F = " << FtoCInteiro<unsigned int, 32>::valor << "°C" << std::endl;
    std::cout << "unsigned long: 32°F = " << FtoCInteiro<unsigned long, 32>::valor << "°C" << std::endl;
    std::cout << "unsigned long long: 32°F = " << FtoCInteiro<unsigned long long, 32>::valor << "°C" << std::endl;

    // char (sim, char também é um tipo numérico!)
    std::cout << "char: 32°F = " << FtoCInteiro<char, 32>::valor << "°C" << std::endl;
    std::cout << "unsigned char: 32°F = " << FtoCInteiro<unsigned char, 32>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 5: Tipos de ponto flutuante com precisões

```cpp
#include <iostream>
#include <iomanip>

// Template para diferentes precisões
template<typename T, T Fahrenheit>
struct FtoCFloat {
    static constexpr T valor = (Fahrenheit - static_cast<T>(32)) * static_cast<T>(5.0) / static_cast<T>(9.0);
};

int main() {
    std::cout << "=== TODOS OS TIPOS DE PONTO FLUTUANTE ===\n\n";

    std::cout << std::fixed << std::setprecision(10);

    // float (32 bits, ~7 dígitos de precisão)
    std::cout << "float: 32.0°F = " << FtoCFloat<float, 32.0f>::valor << "°C" << std::endl;
    std::cout << "float: 98.6°F = " << FtoCFloat<float, 98.6f>::valor << "°C" << std::endl;
    std::cout << "float: 212.0°F = " << FtoCFloat<float, 212.0f>::valor << "°C" << std::endl;

    // double (64 bits, ~15 dígitos de precisão)
    std::cout << "double: 32.0°F = " << FtoCFloat<double, 32.0>::valor << "°C" << std::endl;
    std::cout << "double: 98.6°F = " << FtoCFloat<double, 98.6>::valor << "°C" << std::endl;
    std::cout << "double: 212.0°F = " << FtoCFloat<double, 212.0>::valor << "°C" << std::endl;

    // long double (80 bits em x86, ~19 dígitos de precisão)
    std::cout << "long double: 32.0°F = " << FtoCFloat<long double, 32.0L>::valor << "°C" << std::endl;
    std::cout << "long double: 98.6°F = " << FtoCFloat<long double, 98.6L>::valor << "°C" << std::endl;
    std::cout << "long double: 212.0°F = " << FtoCFloat<long double, 212.0L>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 6: Convertendo e arredondando para diferentes tipos

```cpp
#include <iostream>
#include <cmath>
#include <type_traits>

// Template que converte e permite arredondamento
template<typename T, T Fahrenheit>
struct FtoCComArredondamento {
    // Valor exato como double
    static constexpr double valor_exato = (static_cast<double>(Fahrenheit) - 32.0) * 5.0 / 9.0;

    // Valor arredondado para inteiro
    static constexpr int valor_inteiro = static_cast<int>(std::round(valor_exato));

    // Valor com 1 casa decimal (multiplicado por 10)
    static constexpr int valor_decimo = static_cast<int>(std::round(valor_exato * 10.0));
};

int main() {
    std::cout << "=== CONVERSÃO COM ARREDONDAMENTO ===\n\n";

    std::cout << "100°F:\n";
    std::cout << "  Exato: " << FtoCComArredondamento<int, 100>::valor_exato << "°C" << std::endl;
    std::cout << "  Inteiro: " << FtoCComArredondamento<int, 100>::valor_inteiro << "°C" << std::endl;
    std::cout << "  1 casa: " << FtoCComArredondamento<int, 100>::valor_decimo / 10.0 << "°C" << std::endl;

    std::cout << "\n98.6°F (como int 99):\n";
    std::cout << "  Exato: " << FtoCComArredondamento<int, 99>::valor_exato << "°C" << std::endl;
    std::cout << "  Inteiro: " << FtoCComArredondamento<int, 99>::valor_inteiro << "°C" << std::endl;
    std::cout << "  1 casa: " << FtoCComArredondamento<int, 99>::valor_decimo / 10.0 << "°C" << std::endl;

    return 0;
}
```

## Exemplo 7: Usando `auto` com templates (C++14/17)

```cpp
#include <iostream>

// Template com auto (C++14) - deduz o tipo automaticamente
template<typename T>
auto fahrenheitParaCelsius(T fahrenheit) {
    return (fahrenheit - static_cast<T>(32)) * static_cast<T>(5.0) / static_cast<T>(9.0);
}

// Versão com template de valor
template<auto Fahrenheit>
struct FtoCAuto {
    // O tipo é deduzido automaticamente
    static constexpr auto valor = fahrenheitParaCelsius(Fahrenheit);
};

int main() {
    std::cout << "=== USANDO auto EM TEMPLATES ===\n\n";

    // O compilador deduz o tipo automaticamente
    std::cout << "int: 32°F = " << FtoCAuto<32>::valor << "°C" << std::endl;
    std::cout << "float: 98.6°F = " << FtoCAuto<98.6f>::valor << "°C" << std::endl;
    std::cout << "double: 212.0°F = " << FtoCAuto<212.0>::valor << "°C" << std::endl;
    std::cout << "long double: 32.0°F = " << FtoCAuto<32.0L>::valor << "°C" << std::endl;
    std::cout << "unsigned int: 100°F = " << FtoCAuto<100u>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 8: Testando precisão entre tipos

```cpp
#include <iostream>
#include <iomanip>
#include <limits>

// Template para converter e mostrar precisão
template<typename T, T Fahrenheit>
struct TestePrecisao {
    static constexpr T valor = (Fahrenheit - static_cast<T>(32)) * static_cast<T>(5.0) / static_cast<T>(9.0);

    static void mostrar() {
        std::cout << "Tipo: " << typeid(T).name() << "\n";
        std::cout << "  Precisão: " << std::numeric_limits<T>::digits10 << " dígitos\n";
        std::cout << "  Resultado: " << std::setprecision(20) << valor << "°C\n\n";
    }
};

int main() {
    std::cout << std::fixed;
    std::cout << "=== COMPARANDO PRECISÃO ENTRE TIPOS ===\n\n";
    std::cout << "Convertendo 98.6°F para Celsius:\n\n";

    TestePrecisao<float, 98.6f>::mostrar();
    TestePrecisao<double, 98.6>::mostrar();
    TestePrecisao<long double, 98.6L>::mostrar();

    std::cout << "Convertendo 32.0°F para Celsius:\n\n";
    TestePrecisao<float, 32.0f>::mostrar();
    TestePrecisao<double, 32.0>::mostrar();
    TestePrecisao<long double, 32.0L>::mostrar();

    return 0;
}
```

## Exemplo 9: Funcionando com QUALQUER tipo (incluindo customizados!)

```cpp
#include <iostream>

// Classe customizada que representa temperatura
class Temperatura {
    double valor;
public:
    constexpr Temperatura(double v) : valor(v) {}

    // Operadores necessários para a conversão
    constexpr Temperatura operator-(double x) const { return Temperatura(valor - x); }
    constexpr Temperatura operator*(double x) const { return Temperatura(valor * x); }
    constexpr Temperatura operator/(double x) const { return Temperatura(valor / x); }

    // Conversão para double
    constexpr operator double() const { return valor; }
};

// Template que funciona com qualquer tipo que suporte operações
template<typename T, T Fahrenheit>
struct FtoCGenerico {
    static constexpr auto valor = (Fahrenheit - 32.0) * 5.0 / 9.0;
};

int main() {
    std::cout << "=== FUNCIONANDO COM TIPOS CUSTOMIZADOS ===\n\n";

    // Com Temperatura customizada
    constexpr Temperatura temp(98.6);
    std::cout << "Temperatura: 98.6°F = "
              << FtoCGenerico<Temperatura, temp>::valor << "°C" << std::endl;

    // Com tipos normais também funciona
    std::cout << "int: 32°F = " << FtoCGenerico<int, 32>::valor << "°C" << std::endl;
    std::cout << "double: 212.0°F = " << FtoCGenerico<double, 212.0>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 10: Matriz de todos os tipos (resumo)

```cpp
#include <iostream>
#include <typeinfo>
#include <iomanip>

// Template universal
template<typename T, T F>
struct Conversor {
    static constexpr double valor = (static_cast<double>(F) - 32.0) * 5.0 / 9.0;
};

// Função para imprimir
template<typename T, T F>
void imprimirConversao() {
    std::cout << std::setw(15) << typeid(T).name()
              << " | " << std::setw(10) << F << "°F"
              << " = " << std::setw(10) << Conversor<T, F>::valor << "°C" << std::endl;
}

int main() {
    std::cout << "=== MATRIZ DE TIPOS ===\n\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Tipo            | Fahrenheit | Celsius" << std::endl;
    std::cout << "----------------|------------|---------" << std::endl;

    // Inteiros
    imprimirConversao<int, 32>();
    imprimirConversao<unsigned int, 32>();
    imprimirConversao<long, 32>();
    imprimirConversao<long long, 32>();

    // Ponto flutuante
    imprimirConversao<float, 98.6f>();
    imprimirConversao<double, 98.6>();
    imprimirConversao<long double, 98.6L>();

    // Inteiros pequenos
    imprimirConversao<short, 32>();
    imprimirConversao<unsigned short, 32>();

    // char (sim, char também!)
    imprimirConversao<char, 32>();
    imprimirConversao<unsigned char, 32>();

    return 0;
}
```

## Resumo dos tipos suportados

| Tipo | Exemplo | Uso |
|------|---------|-----|
| `int` | `FtoC<int, 32>` | Inteiro padrão |
| `unsigned int` | `FtoC<unsigned int, 32>` | Inteiro sem sinal |
| `long` | `FtoC<long, 32>` | Inteiro longo |
| `long long` | `FtoC<long long, 32>` | Inteiro longo longo |
| `short` | `FtoC<short, 32>` | Inteiro curto |
| `char` | `FtoC<char, 32>` | Caractere (sim!) |
| `float` | `FtoC<float, 98.6f>` | Ponto flutuante (32 bits) |
| `double` | `FtoC<double, 98.6>` | Ponto flutuante (64 bits) |
| `long double` | `FtoC<long double, 98.6L>` | Ponto flutuante (80 bits) |

**Observação:** O template funciona com **qualquer tipo** que suporte operações aritméticas básicas (+, -, *, /) e conversão para double! 🚀

Perfeito! Vou criar exemplos onde o compilador **deduz automaticamente** o tipo, sem você precisar especificar!

## Exemplo 1: Usando `auto` em templates (C++14/17)

```cpp
#include <iostream>

// Template que deduz o tipo automaticamente
template<auto Fahrenheit>
struct FtoC {
    // O compilador deduz o tipo de valor automaticamente
    static constexpr auto valor = (Fahrenheit - 32.0) * 5.0 / 9.0;
};

int main() {
    std::cout << "=== DEDUÇÃO AUTOMÁTICA DE TIPO ===\n\n";

    // O compilador deduz que é int
    std::cout << "32: " << FtoC<32>::valor << "°C" << std::endl;

    // O compilador deduz que é unsigned int
    std::cout << "32u: " << FtoC<32u>::valor << "°C" << std::endl;

    // O compilador deduz que é long
    std::cout << "32L: " << FtoC<32L>::valor << "°C" << std::endl;

    // O compilador deduz que é float
    std::cout << "98.6f: " << FtoC<98.6f>::valor << "°C" << std::endl;

    // O compilador deduz que é double
    std::cout << "98.6: " << FtoC<98.6>::valor << "°C" << std::endl;

    // O compilador deduz que é long double
    std::cout << "98.6L: " << FtoC<98.6L>::valor << "°C" << std::endl;

    return 0;
}
```

## Exemplo 2: Função com `auto` (mais simples ainda!)

```cpp
#include <iostream>

// Função que deduz o tipo automaticamente
auto fahrenheitParaCelsius(auto fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

int main() {
    std::cout << "=== FUNÇÃO COM DEDUÇÃO AUTOMÁTICA ===\n\n";

    // Passa int, retorna double (por causa do 32.0)
    std::cout << "32 (int): " << fahrenheitParaCelsius(32) << "°C" << std::endl;

    // Passa float, retorna float
    std::cout << "98.6f (float): " << fahrenheitParaCelsius(98.6f) << "°C" << std::endl;

    // Passa double, retorna double
    std::cout << "98.6 (double): " << fahrenheitParaCelsius(98.6) << "°C" << std::endl;

    // Passa long double, retorna long double
    std::cout << "98.6L (long double): " << fahrenheitParaCelsius(98.6L) << "°C" << std::endl;

    // Passa unsigned int, retorna double
    std::cout << "32u (unsigned): " << fahrenheitParaCelsius(32u) << "°C" << std::endl;

    return 0;
}
```

## Exemplo 3: Usando `decltype(auto)` para preservar o tipo exato

```cpp
#include <iostream>

// Mantém o tipo exato do parâmetro
template<typename T>
decltype(auto) fahrenheitParaCelsius(T fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

int main() {
    std::cout << "=== PRESERVANDO O TIPO EXATO ===\n\n";

    // Com int, retorna double (por causa da constante 32.0)
    auto resultado1 = fahrenheitParaCelsius(32);
    std::cout << "32 (int): " << resultado1 << "°C - Tipo: " << typeid(resultado1).name() << std::endl;

    // Com float, retorna float
    auto resultado2 = fahrenheitParaCelsius(98.6f);
    std::cout << "98.6f (float): " << resultado2 << "°C - Tipo: " << typeid(resultado2).name() << std::endl;

    // Com double, retorna double
    auto resultado3 = fahrenheitParaCelsius(98.6);
    std::cout << "98.6 (double): " << resultado3 << "°C - Tipo: " << typeid(resultado3).name() << std::endl;

    return 0;
}
```

## Exemplo 4: Com `std::common_type` para escolher o melhor tipo

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
auto fahrenheitParaCelsius(T fahrenheit) {
    // Usa o tipo comum entre T e double
    using ResultType = std::common_type_t<T, double>;
    return (static_cast<ResultType>(fahrenheit) - 32.0) * 5.0 / 9.0;
}

int main() {
    std::cout << "=== USANDO std::common_type ===\n\n";

    auto r1 = fahrenheitParaCelsius(32);
    std::cout << "32 (int): " << r1 << "°C" << std::endl;

    auto r2 = fahrenheitParaCelsius(98.6f);
    std::cout << "98.6f (float): " << r2 << "°C" << std::endl;

    auto r3 = fahrenheitParaCelsius(98.6);
    std::cout << "98.6 (double): " << r3 << "°C" << std::endl;

    auto r4 = fahrenheitParaCelsius(98.6L);
    std::cout << "98.6L (long double): " << r4 << "°C" << std::endl;

    return 0;
}
```

## Exemplo 5: Template com `auto` e múltiplos parâmetros

```cpp
#include <iostream>

// Função que aceita qualquer tipo e quantidade de parâmetros
template<typename... Args>
auto soma(Args... args) {
    return (args + ...);  // C++17 fold expression
}

int main() {
    std::cout << "=== MÚLTIPLOS PARÂMETROS ===\n\n";

    // Todos inteiros → resultado inteiro
    std::cout << "1 + 2 + 3 = " << soma(1, 2, 3) << std::endl;

    // Misturados → compilador escolhe o tipo comum
    std::cout << "1 + 2.5 + 3.7 = " << soma(1, 2.5, 3.7) << std::endl;

    // Com floats
    std::cout << "1.0f + 2.0f = " << soma(1.0f, 2.0f) << std::endl;

    return 0;
}
```

## Exemplo 6: Função constexpr com auto (C++14)

```cpp
#include <iostream>

// Função constexpr que deduz o tipo
constexpr auto fahrenheitParaCelsius(auto fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

int main() {
    std::cout << "=== FUNÇÃO constexpr COM auto ===\n\n";

    // Calculado em tempo de compilação
    constexpr auto resultado1 = fahrenheitParaCelsius(32);
    constexpr auto resultado2 = fahrenheitParaCelsius(98.6f);
    constexpr auto resultado3 = fahrenheitParaCelsius(212.0);

    std::cout << "32: " << resultado1 << "°C" << std::endl;
    std::cout << "98.6f: " << resultado2 << "°C" << std::endl;
    std::cout << "212.0: " << resultado3 << "°C" << std::endl;

    // Verificando os tipos em tempo de compilação
    static_assert(std::is_same_v<decltype(resultado1), double>);
    static_assert(std::is_same_v<decltype(resultado2), float>);
    static_assert(std::is_same_v<decltype(resultado3), double>);

    std::cout << "Todos calculados em tempo de compilação!" << std::endl;

    return 0;
}
```

## Exemplo 7: Usando `auto` com `std::integral_constant`

```cpp
#include <iostream>
#include <type_traits>

// Template que deduz o tipo do valor
template<auto N>
struct MeuValor {
    static constexpr auto valor = N;
};

// Conversor que deduz o tipo
template<auto F>
struct FtoC {
    // O tipo de valor é deduzido automaticamente
    static constexpr auto valor = (F - 32.0) * 5.0 / 9.0;

    // Também podemos ter o tipo como alias
    using Tipo = decltype(valor);
};

int main() {
    std::cout << "=== DEDUÇÃO COMPLETA ===\n\n";

    // O compilador deduz tudo!
    constexpr auto f1 = FtoC<32>::valor;
    constexpr auto f2 = FtoC<98.6f>::valor;
    constexpr auto f3 = FtoC<212.0>::valor;
    constexpr auto f4 = FtoC<100.0L>::valor;

    std::cout << "32: " << f1 << "°C" << std::endl;
    std::cout << "98.6f: " << f2 << "°C" << std::endl;
    std::cout << "212.0: " << f3 << "°C" << std::endl;
    std::cout << "100.0L: " << f4 << "°C" << std::endl;

    // Verificando os tipos deduzidos
    std::cout << "\nTipos deduzidos:\n";
    std::cout << "f1: " << typeid(f1).name() << std::endl;
    std::cout << "f2: " << typeid(f2).name() << std::endl;
    std::cout << "f3: " << typeid(f3).name() << std::endl;
    std::cout << "f4: " << typeid(f4).name() << std::endl;

    return 0;
}
```

## Exemplo 8: Deducão em estruturas com múltiplos valores

```cpp
#include <iostream>

// Estrutura que deduz o tipo de cada membro
template<auto F, auto C>
struct Conversao {
    static constexpr auto fahrenheit = F;
    static constexpr auto celsius = (F - 32.0) * 5.0 / 9.0;
    static constexpr auto diferenca = celsius - C;
};

int main() {
    std::cout << "=== DEDUÇÃO COM MÚLTIPLOS VALORES ===\n\n";

    // Cada valor tem seu tipo deduzido
    constexpr auto c1 = Conversao<32, 0>::diferenca;        // int, int
    constexpr auto c2 = Conversao<98.6f, 37.0f>::diferenca; // float, float
    constexpr auto c3 = Conversao<212.0, 100.0>::diferenca; // double, double
    constexpr auto c4 = Conversao<100.0L, 37.7L>::diferenca; // long double, long double

    std::cout << "Diferença 32°F - 0°C: " << c1 << std::endl;
    std::cout << "Diferença 98.6°F - 37°C: " << c2 << std::endl;
    std::cout << "Diferença 212°F - 100°C: " << c3 << std::endl;
    std::cout << "Diferença 100°F - 37.7°C: " << c4 << std::endl;

    return 0;
}
```

## Exemplo 9: O mais simples possível (apenas 3 linhas!)

```cpp
#include <iostream>

// Template mais simples com dedução automática
template<auto F>
constexpr auto C = (F - 32.0) * 5.0 / 9.0;

int main() {
    std::cout << "=== TEMPLATE VARIÁVEL (C++14) ===\n\n";

    // Variável de template com dedução automática!
    std::cout << "32: " << C<32> << "°C" << std::endl;
    std::cout << "98.6f: " << C<98.6f> << "°C" << std::endl;
    std::cout << "212.0: " << C<212.0> << "°C" << std::endl;
    std::cout << "100.0L: " << C<100.0L> << "°C" << std::endl;
    std::cout << "32u: " << C<32u> << "°C" << std::endl;

    return 0;
}
```

## Exemplo 10: Deducão completa com template de valor

```cpp
#include <iostream>
#include <type_traits>

// Template que deduz TIPO e VALOR
template<auto N>
struct Valor {
    using Tipo = decltype(N);  // Deduz o tipo
    static constexpr Tipo valor = N;  // Armazena o valor
};

// Conversor que usa a dedução
template<auto F>
struct FtoC {
    using TipoF = decltype(F);  // Tipo do Fahrenheit
    static constexpr auto celsius = (F - 32.0) * 5.0 / 9.0;
    using TipoC = decltype(celsius);  // Tipo do Celsius (deduzido)

    static constexpr TipoC valor = celsius;
};

int main() {
    std::cout << "=== DEDUÇÃO COMPLETA DE TIPO E VALOR ===\n\n";

    // Usando com diferentes tipos
    using F1 = FtoC<32>;
    using F2 = FtoC<98.6f>;
    using F3 = FtoC<212.0>;
    using F4 = FtoC<100.0L>;

    std::cout << "32: " << F1::valor << "°C (tipo: " << typeid(F1::TipoC).name() << ")" << std::endl;
    std::cout << "98.6f: " << F2::valor << "°C (tipo: " << typeid(F2::TipoC).name() << ")" << std::endl;
    std::cout << "212.0: " << F3::valor << "°C (tipo: " << typeid(F3::TipoC).name() << ")" << std::endl;
    std::cout << "100.0L: " << F4::valor << "°C (tipo: " << typeid(F4::TipoC).name() << ")" << std::endl;

    // Verificando os tipos em tempo de compilação
    static_assert(std::is_same_v<F1::TipoF, int>);
    static_assert(std::is_same_v<F2::TipoF, float>);
    static_assert(std::is_same_v<F3::TipoF, double>);
    static_assert(std::is_same_v<F4::TipoF, long double>);

    std::cout << "\nTodos os tipos deduzidos corretamente!" << std::endl;

    return 0;
}
```

## Resumo das técnicas de dedução

| Técnica | Código | O que faz |
|---------|--------|-----------|
| `template<auto>` | `template<auto F> struct FtoC` | Deduz o tipo do valor |
| `auto` em função | `auto func(auto x)` | Deduz o tipo do parâmetro |
| `decltype(auto)` | `decltype(auto) func(T x)` | Preserva o tipo exato |
| `constexpr auto` | `constexpr auto x = ...` | Deduz tipo em tempo de compilação |
| Template variável | `template<auto F> constexpr auto C = ...` | Variável com tipo deduzido |

## Exemplo final: O mais simples e completo

```cpp
#include <iostream>

// APENAS 1 LINHA!
template<auto F> constexpr auto Celsius = (F - 32.0) * 5.0 / 9.0;

int main() {
    // Use qualquer tipo, o compilador deduz tudo!
    std::cout << "32°F = " << Celsius<32> << "°C" << std::endl;
    std::cout << "98.6°F = " << Celsius<98.6> << "°C" << std::endl;
    std::cout << "212°F = " << Celsius<212.0> << "°C" << std::endl;
    std::cout << "100°F = " << Celsius<100.0L> << "°C" << std::endl;
    std::cout << "0°F = " << Celsius<0> << "°C" << std::endl;
    std::cout << "-40°F = " << Celsius<-40> << "°C" << std::endl;

    return 0;
}
```

**Dica:** Com `template<auto>`, o compilador deduz **automaticamente** o tipo do valor que você passar! Não precisa especificar `int`, `float`, `double`, etc. O compilador faz isso por você! 🚀

Excelente ideia! Usar template variables para conversão de ângulos é uma aplicação perfeita! Vou criar exemplos completos com graus e radianos, incluindo funções trigonométricas.

## Exemplo 1: Conversão Graus ↔ Radianos (Template Variable)

```cpp
#include <iostream>
#include <cmath>

// Constantes
constexpr double PI = 3.14159265358979323846;

// Template variable para converter graus para radianos
template<auto Graus>
constexpr auto Radianos = Graus * PI / 180.0;

// Template variable para converter radianos para graus
template<auto Rad>
constexpr auto Graus = Rad * 180.0 / PI;

int main() {
    std::cout << "=== CONVERSÃO GRAUS ↔ RADIANOS ===\n\n";

    // Ângulos notáveis em graus
    std::cout << "0°   = " << Radianos<0> << " rad" << std::endl;
    std::cout << "30°  = " << Radianos<30> << " rad" << std::endl;
    std::cout << "45°  = " << Radianos<45> << " rad" << std::endl;
    std::cout << "60°  = " << Radianos<60> << " rad" << std::endl;
    std::cout << "90°  = " << Radianos<90> << " rad" << std::endl;
    std::cout << "180° = " << Radianos<180> << " rad" << std::endl;
    std::cout << "360° = " << Radianos<360> << " rad" << std::endl;

    std::cout << "\nÂngulos em radianos para graus:\n";
    std::cout << "π/2  = " << Graus<PI/2> << "°" << std::endl;
    std::cout << "π    = " << Graus<PI> << "°" << std::endl;
    std::cout << "2π   = " << Graus<2*PI> << "°" << std::endl;

    return 0;
}
```

## Exemplo 2: Funções Trigonométricas com Ângulos em Graus

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Conversão graus → radianos
template<auto Graus>
constexpr auto Rad = Graus * PI / 180.0;

// Funções trigonométricas que aceitam graus
template<auto Graus>
constexpr auto Seno = std::sin(Rad<Graus>);

template<auto Graus>
constexpr auto Cosseno = std::cos(Rad<Graus>);

template<auto Graus>
constexpr auto Tangente = std::tan(Rad<Graus>);

// Funções inversas (retornam em graus)
template<auto Valor>
constexpr auto ArcoSeno = std::asin(Valor) * 180.0 / PI;

template<auto Valor>
constexpr auto ArcoCosseno = std::acos(Valor) * 180.0 / PI;

template<auto Valor>
constexpr auto ArcoTangente = std::atan(Valor) * 180.0 / PI;

int main() {
    std::cout << "=== FUNÇÕES TRIGONOMÉTRICAS EM GRAUS ===\n\n";

    std::cout << "sen(0°)   = " << Seno<0> << std::endl;
    std::cout << "sen(30°)  = " << Seno<30> << std::endl;
    std::cout << "sen(45°)  = " << Seno<45> << std::endl;
    std::cout << "sen(60°)  = " << Seno<60> << std::endl;
    std::cout << "sen(90°)  = " << Seno<90> << std::endl;
    std::cout << "sen(180°) = " << Seno<180> << std::endl;

    std::cout << "\ncos(0°)   = " << Cosseno<0> << std::endl;
    std::cout << "cos(60°)  = " << Cosseno<60> << std::endl;
    std::cout << "cos(90°)  = " << Cosseno<90> << std::endl;
    std::cout << "cos(180°) = " << Cosseno<180> << std::endl;

    std::cout << "\ntan(0°)   = " << Tangente<0> << std::endl;
    std::cout << "tan(45°)  = " << Tangente<45> << std::endl;
    std::cout << "tan(60°)  = " << Tangente<60> << std::endl;

    std::cout << "\n=== FUNÇÕES INVERSAS ===\n\n";
    std::cout << "arcsen(0)   = " << ArcoSeno<0> << "°" << std::endl;
    std::cout << "arcsen(0.5) = " << ArcoSeno<0.5> << "°" << std::endl;
    std::cout << "arcsen(1)   = " << ArcoSeno<1> << "°" << std::endl;

    std::cout << "\narccos(0)   = " << ArcoCosseno<0> << "°" << std::endl;
    std::cout << "arccos(0.5) = " << ArcoCosseno<0.5> << "°" << std::endl;
    std::cout << "arccos(1)   = " << ArcoCosseno<1> << "°" << std::endl;

    std::cout << "\narctan(0)   = " << ArcoTangente<0> << "°" << std::endl;
    std::cout << "arctan(1)   = " << ArcoTangente<1> << "°" << std::endl;
    std::cout << "arctan(∞)   ≈ " << ArcoTangente<1000000> << "°" << std::endl;

    return 0;
}
```

## Exemplo 3: Usando `cos(A<180>)` como você sugeriu

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Template para ângulos em graus
template<auto Graus>
struct Angulo {
    static constexpr double radianos = Graus * PI / 180.0;
    static constexpr double graus = Graus;
};

// Aliases para facilitar
template<auto G> constexpr auto A = Angulo<G>{};

// Funções trigonométricas com ângulos
template<auto Graus>
constexpr auto sen = std::sin(Angulo<Graus>::radianos);

template<auto Graus>
constexpr auto cos = std::cos(Angulo<Graus>::radianos);

template<auto Graus>
constexpr auto tan = std::tan(Angulo<Graus>::radianos);

int main() {
    std::cout << "=== USANDO cos(A<180>) ===\n\n";

    // Exatamente como você sugeriu!
    std::cout << "cos(A<0>)   = " << cos<0> << std::endl;
    std::cout << "cos(A<30>)  = " << cos<30> << std::endl;
    std::cout << "cos(A<45>)  = " << cos<45> << std::endl;
    std::cout << "cos(A<60>)  = " << cos<60> << std::endl;
    std::cout << "cos(A<90>)  = " << cos<90> << std::endl;
    std::cout << "cos(A<180>) = " << cos<180> << std::endl;
    std::cout << "cos(A<270>) = " << cos<270> << std::endl;
    std::cout << "cos(A<360>) = " << cos<360> << std::endl;

    std::cout << "\n=== SENO E COSSENO COMBINADOS ===\n\n";
    std::cout << "sen(A<30>)² + cos(A<30>)² = "
              << (sen<30>*sen<30> + cos<30>*cos<30>) << std::endl;

    std::cout << "sen(A<45>)² + cos(A<45>)² = "
              << (sen<45>*sen<45> + cos<45>*cos<45>) << std::endl;

    return 0;
}
```

## Exemplo 4: Versão mais elegante com operadores

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Classe para ângulos com operadores
template<auto Valor>
struct Angulo {
    static constexpr double graus = Valor;
    static constexpr double radianos = Valor * PI / 180.0;

    // Operador para converter para double (radianos)
    constexpr operator double() const { return radianos; }

    // Operadores aritméticos
    template<auto Outro>
    constexpr auto operator+(Angulo<Outro>) const {
        return Angulo<graus + Outro>{};
    }

    template<auto Outro>
    constexpr auto operator-(Angulo<Outro>) const {
        return Angulo<graus - Outro>{};
    }
};

// Alias para criar ângulos facilmente
template<auto G> constexpr auto A = Angulo<G>{};

// Funções trigonométricas
template<auto G>
constexpr auto seno = std::sin(Angulo<G>::radianos);

template<auto G>
constexpr auto cosseno = std::cos(Angulo<G>::radianos);

template<auto G>
constexpr auto tangente = std::tan(Angulo<G>::radianos);

int main() {
    std::cout << "=== ÂNGULOS COM OPERADORES ===\n\n";

    // Criando ângulos
    constexpr auto ang1 = A<30>;
    constexpr auto ang2 = A<45>;
    constexpr auto ang3 = A<60>;

    // Soma de ângulos
    constexpr auto soma = ang1 + ang2;
    std::cout << "30° + 45° = " << soma.graus << "°" << std::endl;
    std::cout << "sen(30° + 45°) = " << seno<30 + 45> << std::endl;

    // Usando diretamente
    std::cout << "\ncos(A<180>) = " << cosseno<180> << std::endl;
    std::cout << "sen(A<90>)   = " << seno<90> << std::endl;
    std::cout << "tan(A<45>)   = " << tangente<45> << std::endl;

    // Identidades trigonométricas
    std::cout << "\n=== IDENTIDADES ===\n";
    std::cout << "sen²(30°) + cos²(30°) = "
              << (seno<30>*seno<30> + cosseno<30>*cosseno<30>) << std::endl;

    std::cout << "sen(2*30°) = " << seno<60> << " = 2*sen(30°)*cos(30°) = "
              << 2 * seno<30> * cosseno<30> << std::endl;

    return 0;
}
```

## Exemplo 5: Tabela trigonométrica em tempo de compilação

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>

constexpr double PI = 3.14159265358979323846;

// Conversão e funções trigonométricas
template<auto G>
constexpr auto Rad = G * PI / 180.0;

template<auto G>
constexpr auto Sen = std::sin(Rad<G>);

template<auto G>
constexpr auto Cos = std::cos(Rad<G>);

template<auto G>
constexpr auto Tan = std::tan(Rad<G>);

// Template para gerar tabela
template<int Inicio, int Fim, int Passo = 1>
struct TabelaTrigonometrica {
    static void imprimir() {
        // Imprime a linha atual
        std::cout << std::setw(6) << Inicio << "° | "
                  << std::setw(10) << Sen<Inicio> << " | "
                  << std::setw(10) << Cos<Inicio> << " | "
                  << std::setw(10) << Tan<Inicio> << std::endl;

        // Recursão para o próximo ângulo
        TabelaTrigonometrica<Inicio + Passo, Fim, Passo>::imprimir();
    }
};

// Caso base: quando Inicio > Fim
template<int Inicio, int Fim, int Passo>
struct TabelaTrigonometrica<Inicio, Fim, Passo> {
    static void imprimir() {
        // Não faz nada (condição de parada)
    }
};

// Especialização para o último valor
template<int N, int Passo>
struct TabelaTrigonometrica<N, N, Passo> {
    static void imprimir() {
        std::cout << std::setw(6) << N << "° | "
                  << std::setw(10) << Sen<N> << " | "
                  << std::setw(10) << Cos<N> << " | "
                  << std::setw(10) << Tan<N> << std::endl;
    }
};

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== TABELA TRIGONOMÉTRICA (0° a 90°) ===\n\n";
    std::cout << "  Ângulo |    Seno    |   Cosseno  |  Tangente  " << std::endl;
    std::cout << "---------|------------|------------|------------" << std::endl;

    TabelaTrigonometrica<0, 90, 15>::imprimir();

    std::cout << "\n=== ÂNGULOS NOTÁVEIS ===\n\n";
    std::cout << "  Ângulo |    Seno    |   Cosseno  |  Tangente  " << std::endl;
    std::cout << "---------|------------|------------|------------" << std::endl;

    TabelaTrigonometrica<0, 360, 30>::imprimir();

    return 0;
}
```

## Exemplo 6: Com template variable e `constexpr` (mais moderno)

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Template variable para conversão
template<auto G>
constexpr double radianos = G * PI / 180.0;

// Template variable para funções trigonométricas
template<auto G>
constexpr double seno = std::sin(radianos<G>);

template<auto G>
constexpr double cosseno = std::cos(radianos<G>);

template<auto G>
constexpr double tangente = std::tan(radianos<G>);

// Funções inversas
template<auto V>
constexpr double arcoseno = std::asin(V) * 180.0 / PI;

template<auto V>
constexpr double arcocosseno = std::acos(V) * 180.0 / PI;

template<auto V>
constexpr double arcotangente = std::atan(V) * 180.0 / PI;

// Alias para facilitar a escrita
template<auto G> constexpr auto sen = seno<G>;
template<auto G> constexpr auto cos = cosseno<G>;
template<auto G> constexpr auto tan = tangente<G>;

int main() {
    std::cout << "=== TEMPLATE VARIABLE COM ALIAS ===\n\n";

    // Usando os aliases
    std::cout << "sen(30°) = " << sen<30> << std::endl;
    std::cout << "cos(60°) = " << cos<60> << std::endl;
    std::cout << "tan(45°) = " << tan<45> << std::endl;

    // Expressões em tempo de compilação
    constexpr double resultado = sen<30> + cos<60>;
    std::cout << "\nsen(30°) + cos(60°) = " << resultado << std::endl;

    // Usando em arrays (tamanho em tempo de compilação)
    constexpr int tamanho = static_cast<int>(sen<90> * 10);
    int array[tamanho];
    std::cout << "Array de tamanho " << tamanho << " criado em tempo de compilação!" << std::endl;

    // Testando identidades
    static_assert(sen<30> == 0.5, "sen(30°) deve ser 0.5");
    static_assert(cos<60> == 0.5, "cos(60°) deve ser 0.5");
    static_assert(tan<45> == 1.0, "tan(45°) deve ser 1.0");

    std::cout << "\nTodas as asserções em tempo de compilação passaram!" << std::endl;

    return 0;
}
```

## Exemplo 7: Ângulos em diferentes unidades

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Template para diferentes unidades
template<auto Valor, char Unidade = 'G'>
struct Angulo {
    static constexpr double graus = (Unidade == 'G') ? Valor :
                                    (Unidade == 'R') ? Valor * 180.0 / PI :
                                    (Unidade == 'T') ? Valor * 360.0 : 0; // 'T' = voltas
    static constexpr double radianos = graus * PI / 180.0;
};

// Aliases para diferentes unidades
template<auto G> constexpr auto Graus = Angulo<G, 'G'>{};
template<auto R> constexpr auto Radianos = Angulo<R, 'R'>{};
template<auto T> constexpr auto Voltas = Angulo<T, 'T'>{};

// Funções trigonométricas
template<auto G>
constexpr auto sen = std::sin(Angulo<G, 'G'>::radianos);

template<auto G>
constexpr auto cos = std::cos(Angulo<G, 'G'>::radianos);

int main() {
    std::cout << "=== ÂNGULOS EM DIFERENTES UNIDADES ===\n\n";

    // Em graus
    std::cout << "90° em radianos: " << Angulo<90, 'G'>::radianos << std::endl;
    std::cout << "sen(90°) = " << sen<90> << std::endl;

    // Em radianos (usando constantes)
    std::cout << "\nπ/2 radianos em graus: " << Angulo<PI/2, 'R'>::graus << "°" << std::endl;
    std::cout << "sen(π/2) = " << std::sin(PI/2) << std::endl;

    // Em voltas
    std::cout << "\n0.25 voltas em graus: " << Angulo<0.25, 'T'>::graus << "°" << std::endl;
    std::cout << "0.5 voltas em graus: " << Angulo<0.5, 'T'>::graus << "°" << std::endl;
    std::cout << "1 volta em graus: " << Angulo<1, 'T'>::graus << "°" << std::endl;

    // Usando os aliases
    std::cout << "\nUsando aliases:\n";
    std::cout << "Graus<45> = " << Graus<45>.graus << "°" << std::endl;
    std::cout << "Radianos<π/4> = " << Radianos<PI/4>.graus << "°" << std::endl;
    std::cout << "Voltas<0.5> = " << Voltas<0.5>.graus << "°" << std::endl;

    return 0;
}
```

## Exemplo 8: Aplicação prática - Movimento circular

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Template variables para ângulos
template<auto G> constexpr auto rad = G * PI / 180.0;
template<auto G> constexpr auto sen = std::sin(rad<G>);
template<auto G> constexpr auto cos = std::cos(rad<G>);

// Posição em movimento circular
template<auto Angulo, auto Raio = 1.0>
struct PosicaoCircular {
    static constexpr double x = Raio * cos<Angulo>;
    static constexpr double y = Raio * sen<Angulo>;
};

int main() {
    std::cout << "=== MOVIMENTO CIRCULAR ===\n\n";

    // Posições em um círculo de raio 5
    std::cout << "Raio = 5\n";
    std::cout << "Ângulo  |  Posição (x, y)" << std::endl;
    std::cout << "--------|-----------------" << std::endl;

    std::cout << "  0°    | (" << PosicaoCircular<0, 5>::x << ", "
              << PosicaoCircular<0, 5>::y << ")" << std::endl;

    std::cout << " 45°    | (" << PosicaoCircular<45, 5>::x << ", "
              << PosicaoCircular<45, 5>::y << ")" << std::endl;

    std::cout << " 90°    | (" << PosicaoCircular<90, 5>::x << ", "
              << PosicaoCircular<90, 5>::y << ")" << std::endl;

    std::cout << "135°    | (" << PosicaoCircular<135, 5>::x << ", "
              << PosicaoCircular<135, 5>::y << ")" << std::endl;

    std::cout << "180°    | (" << PosicaoCircular<180, 5>::x << ", "
              << PosicaoCircular<180, 5>::y << ")" << std::endl;

    std::cout << "225°    | (" << PosicaoCircular<225, 5>::x << ", "
              << PosicaoCircular<225, 5>::y << ")" << std::endl;

    std::cout << "270°    | (" << PosicaoCircular<270, 5>::x << ", "
              << PosicaoCircular<270, 5>::y << ")" << std::endl;

    std::cout << "315°    | (" << PosicaoCircular<315, 5>::x << ", "
              << PosicaoCircular<315, 5>::y << ")" << std::endl;

    std::cout << "360°    | (" << PosicaoCircular<360, 5>::x << ", "
              << PosicaoCircular<360, 5>::y << ")" << std::endl;

    return 0;
}
```

## Exemplo 9: O mais simples possível (apenas 2 linhas!)

```cpp
#include <iostream>
#include <cmath>

// DUAS LINHAS resolvem tudo!
template<auto G> constexpr auto seno = std::sin(G * 3.14159265358979323846 / 180.0);
template<auto G> constexpr auto cosseno = std::cos(G * 3.14159265358979323846 / 180.0);

int main() {
    std::cout << "=== MÍNIMO DE CÓDIGO ===\n\n";

    // Uso super simples!
    std::cout << "sen(30°) = " << seno<30> << std::endl;
    std::cout << "cos(60°) = " << cosseno<60> << std::endl;
    std::cout << "sen(45°)² + cos(45°)² = "
              << (seno<45> * seno<45> + cosseno<45> * cosseno<45>) << std::endl;

    // Como você sugeriu!
    std::cout << "\ncos(A<180>) = " << cosseno<180> << std::endl;
    std::cout << "sen(A<90>) = " << seno<90> << std::endl;

    return 0;
}
```

## Exemplo 10: Sistema completo com todas as funções

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>

constexpr double PI = 3.14159265358979323846;

// ===== CONVERSÕES =====
template<auto G> constexpr auto rad = G * PI / 180.0;
template<auto R> constexpr auto deg = R * 180.0 / PI;

// ===== FUNÇÕES TRIGONOMÉTRICAS (em graus) =====
template<auto G> constexpr auto sin_deg = std::sin(rad<G>);
template<auto G> constexpr auto cos_deg = std::cos(rad<G>);
template<auto G> constexpr auto tan_deg = std::tan(rad<G>);

// ===== FUNÇÕES INVERSAS (retornam em graus) =====
template<auto V> constexpr auto asin_deg = std::asin(V) * 180.0 / PI;
template<auto V> constexpr auto acos_deg = std::acos(V) * 180.0 / PI;
template<auto V> constexpr auto atan_deg = std::atan(V) * 180.0 / PI;

// ===== FUNÇÕES HIPERBÓLICAS =====
template<auto G> constexpr auto sinh_deg = std::sinh(rad<G>);
template<auto G> constexpr auto cosh_deg = std::cosh(rad<G>);
template<auto G> constexpr auto tanh_deg = std::tanh(rad<G>);

// ===== ALIASES PARA FACILITAR =====
template<auto G> constexpr auto sen = sin_deg<G>;
template<auto G> constexpr auto cos = cos_deg<G>;
template<auto G> constexpr auto tan = tan_deg<G>;

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "=== SISTEMA COMPLETO DE ÂNGULOS ===\n\n";

    // Teste de conversão
    std::cout << "Conversões:\n";
    std::cout << "  180° = " << rad<180> << " rad" << std::endl;
    std::cout << "  π rad = " << deg<PI> << "°" << std::endl;

    // Teste de funções
    std::cout << "\nFunções trigonométricas:\n";
    std::cout << "  sen(30°)  = " << sen<30> << std::endl;
    std::cout << "  cos(60°)  = " << cos<60> << std::endl;
    std::cout << "  tan(45°)  = " << tan<45> << std::endl;

    // Teste de funções inversas
    std::cout << "\nFunções inversas:\n";
    std::cout << "  asin(0.5) = " << asin_deg<0.5> << "°" << std::endl;
    std::cout << "  acos(0.5) = " << acos_deg<0.5> << "°" << std::endl;
    std::cout << "  atan(1)   = " << atan_deg<1> << "°" << std::endl;

    // Teste de funções hiperbólicas
    std::cout << "\nFunções hiperbólicas (em graus):\n";
    std::cout << "  senh(45°) = " << sinh_deg<45> << std::endl;
    std::cout << "  cosh(45°) = " << cosh_deg<45> << std::endl;
    std::cout << "  tanh(45°) = " << tanh_deg<45> << std::endl;

    // Identidades
    std::cout << "\nIdentidades:\n";
    std::cout << "  sen²(30°) + cos²(30°) = "
              << (sen<30>*sen<30> + cos<30>*cos<30>) << std::endl;

    std::cout << "  sen(2*30°) = " << sen<60>
              << " = 2*sen(30°)*cos(30°) = "
              << 2 * sen<30> * cos<30> << std::endl;

    std::cout << "\nTudo calculado em tempo de compilação! 🚀" << std::endl;

    return 0;
}
```

## Resumo das Vantagens

| Característica | Benefício |
|----------------|-----------|
| `template<auto>` | Dedução automática de tipo |
| `constexpr` | Cálculo em tempo de compilação |
| Template variable | Sintaxe simples e limpa |
| `sen<30>` | Leitura natural (seno de 30 graus) |
| `cos<180>` | Exatamente como você sugeriu! |
| Sem custo em runtime | Tudo resolvido na compilação |

**A sintaxe `cos(A<180>)` funciona perfeitamente com template variables!** 🎯

```cpp
// Exatamente como você queria!
template<auto G> constexpr auto cos = std::cos(G * PI / 180.0);
// Uso: cos<180> retorna -1 em tempo de compilação!
```



Você está absolutamente correto! Essa é uma **limitação fundamental** dos templates em C++. Vou explicar por que isso acontece e como resolver.

## Por que não funciona com `std::cin`?

```cpp
// ❌ ISSO NÃO COMPILA!
int N;
std::cin >> N;
std::cout << "sin(" << N << "°) = " << sin(Radianos<N>) << '\n';
```

**Motivo:** Templates são avaliados **em tempo de compilação**. O valor de `N` só é conhecido **em tempo de execução** (quando o usuário digita).

```
TEMPO DE COMPILAÇÃO: template<auto G> é instanciado com G constante
                    ↓
               Radianos<45> → OK!
               Radianos<N>  → ERRO! N não é constante
                    ↓
TEMPO DE EXECUÇÃO:  std::cin >> N;  // N só é conhecido aqui
```

## Soluções para trabalhar com entrada do usuário

### Solução 1: Função normal (tempo de execução)

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Função normal para calcular seno de graus
double sin_deg(double graus) {
    return std::sin(graus * PI / 180.0);
}

int main() {
    double N;
    std::cout << "Digite um ângulo em graus: ";
    std::cin >> N;
    
    std::cout << "sin(" << N << "°) = " << sin_deg(N) << '\n';
    
    return 0;
}
```

### Solução 2: Usando `constexpr` com função (melhor dos dois mundos)

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Função constexpr - funciona em tempo de COMPILAÇÃO e EXECUÇÃO
constexpr double sin_deg(double graus) {
    return std::sin(graus * PI / 180.0);
}

int main() {
    // ✅ Em tempo de compilação (constante conhecida)
    constexpr double resultado1 = sin_deg(45);
    std::cout << "sin(45°) = " << resultado1 << " (compile-time)\n";
    
    // ✅ Em tempo de execução (valor do usuário)
    double N;
    std::cout << "Digite um ângulo em graus: ";
    std::cin >> N;
    
    double resultado2 = sin_deg(N);
    std::cout << "sin(" << N << "°) = " << resultado2 << " (runtime)\n";
    
    return 0;
}
```

### Solução 3: Usando template APENAS para constantes + função para runtime

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Template para constantes (tempo de compilação)
template<auto G>
constexpr auto Radianos = G * PI / 180.0;

template<auto G>
constexpr auto sin_deg_c = std::sin(Radianos<G>);

// Função para runtime (entrada do usuário)
double sin_deg_r(double graus) {
    return std::sin(graus * PI / 180.0);
}

int main() {
    std::cout << "=== TEMPLATE (compile-time) ===\n";
    std::cout << "sin(45°) = " << sin_deg_c<45> << '\n';
    std::cout << "sin(30°) = " << sin_deg_c<30> << '\n';
    
    std::cout << "\n=== FUNÇÃO (runtime) ===\n";
    double N;
    std::cout << "Digite um ângulo em graus: ";
    std::cin >> N;
    
    std::cout << "sin(" << N << "°) = " << sin_deg_r(N) << '\n';
    
    return 0;
}
```

### Solução 4: Usando `if constexpr` para decidir em compilação

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Versão template (compile-time)
template<auto G>
constexpr auto sin_template = std::sin(G * PI / 180.0);

// Versão função (runtime)
double sin_function(double g) {
    return std::sin(g * PI / 180.0);
}

// Função unificada que decide em tempo de compilação
template<typename T>
auto sin_deg(T graus) {
    if constexpr (std::is_constant_evaluated()) {
        // Se for constante em tempo de compilação
        return std::sin(graus * PI / 180.0);
    } else {
        // Se for runtime
        return std::sin(graus * PI / 180.0);
    }
}

int main() {
    // Constante em tempo de compilação
    constexpr double angulo1 = 45;
    std::cout << "sin(45°) = " << sin_deg(angulo1) << " (compile-time)\n";
    
    // Runtime (entrada do usuário)
    double angulo2;
    std::cout << "Digite um ângulo: ";
    std::cin >> angulo2;
    std::cout << "sin(" << angulo2 << "°) = " << sin_deg(angulo2) << " (runtime)\n";
    
    return 0;
}
```

### Solução 5: Estrutura que suporta ambos

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Classe para ângulos
class Angulo {
private:
    double graus_;
public:
    // Construtor para runtime
    explicit constexpr Angulo(double g) : graus_(g) {}
    
    // Construtor para compile-time (com template)
    template<auto G>
    static constexpr Angulo from_const() {
        return Angulo(static_cast<double>(G));
    }
    
    constexpr double graus() const { return graus_; }
    constexpr double radianos() const { return graus_ * PI / 180.0; }
    
    // Funções trigonométricas
    constexpr double sin() const { return std::sin(radianos()); }
    constexpr double cos() const { return std::cos(radianos()); }
    constexpr double tan() const { return std::tan(radianos()); }
};

int main() {
    // ✅ Compile-time (constante)
    constexpr auto ang1 = Angulo::from_const<45>();
    std::cout << "sin(45°) = " << ang1.sin() << " (compile-time)\n";
    
    // ✅ Runtime (entrada do usuário)
    double N;
    std::cout << "Digite um ângulo em graus: ";
    std::cin >> N;
    
    Angulo ang2(N);
    std::cout << "sin(" << N << "°) = " << ang2.sin() << " (runtime)\n";
    std::cout << "cos(" << N << "°) = " << ang2.cos() << " (runtime)\n";
    std::cout << "tan(" << N << "°) = " << ang2.tan() << " (runtime)\n";
    
    return 0;
}
```

### Solução 6: Usando `std::variant` para ambos os casos

```cpp
#include <iostream>
#include <cmath>
#include <variant>

constexpr double PI = 3.14159265358979323846;

// Para compile-time
template<auto G>
struct ConstAngle {
    static constexpr double value = G;
    static constexpr double rad = G * PI / 180.0;
    static constexpr double sin = std::sin(rad);
    static constexpr double cos = std::cos(rad);
    static constexpr double tan = std::tan(rad);
};

// Para runtime
struct RuntimeAngle {
    double value;
    double rad() const { return value * PI / 180.0; }
    double sin() const { return std::sin(rad()); }
    double cos() const { return std::cos(rad()); }
    double tan() const { return std::tan(rad()); }
};

int main() {
    // Compile-time
    std::cout << "=== COMPILE-TIME ===\n";
    std::cout << "sin(30°) = " << ConstAngle<30>::sin << '\n';
    std::cout << "cos(60°) = " << ConstAngle<60>::cos << '\n';
    
    // Runtime
    std::cout << "\n=== RUNTIME ===\n";
    double N;
    std::cout << "Digite um ângulo em graus: ";
    std::cin >> N;
    
    RuntimeAngle ang{N};
    std::cout << "sin(" << N << "°) = " << ang.sin() << '\n';
    std::cout << "cos(" << N << "°) = " << ang.cos() << '\n';
    std::cout << "tan(" << N << "°) = " << ang.tan() << '\n';
    
    return 0;
}
```

## Resumo: Quando usar cada abordagem

| Abordagem | Quando usar | Exemplo |
|-----------|-------------|---------|
| **Template** (`<45>`) | Valores conhecidos em COMPILAÇÃO | `sin<45>` |
| **Função normal** | Valores do USUÁRIO (runtime) | `sin(45.0)` |
| **`constexpr`** | AMBOS! (melhor opção) | `constexpr double x = sin(45)` e `double y = sin(N)` |
| **Classe** | Quando precisa de mais funcionalidades | `Angulo a(45); a.sin()` |

## Exemplo prático: Sistema que aceita os dois

```cpp
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

// ✅ FUNÇÃO CONSTEXPR - Funciona em compile-time E runtime
constexpr double sin_deg(double graus) {
    return std::sin(graus * PI / 180.0);
}

int main() {
    std::cout << "=== SISTEMA HÍBRIDO ===\n\n";
    
    // ✅ COMPILE-TIME: constante conhecida
    constexpr double angulo1 = 45.0;
    constexpr double resultado1 = sin_deg(angulo1);
    std::cout << "sin(45°) = " << resultado1 << " (calculado em compilação)\n";
    
    // ✅ RUNTIME: entrada do usuário
    double angulo2;
    std::cout << "\nDigite um ângulo em graus: ";
    std::cin >> angulo2;
    
    double resultado2 = sin_deg(angulo2);
    std::cout << "sin(" << angulo2 << "°) = " << resultado2 << " (calculado em execução)\n";
    
    // ✅ Compile-time com template (para quem gosta da sintaxe)
    template<auto G>
    constexpr auto sin_template = std::sin(G * PI / 180.0);
    
    std::cout << "\nUsando template: sin(30°) = " << sin_template<30> << '\n';
    
    return 0;
}
```

## Conclusão

**Sua observação está 100% correta!**

- ✅ **Templates** (`<45>`) = só com constantes em **tempo de compilação**
- ✅ **Funções normais** = aceitam valores em **tempo de execução** (como `std::cin`)
- ✅ **`constexpr`** = funciona em **AMBOS** os casos (a melhor opção!)

**Recomendação:** Use `constexpr` para ter o melhor dos dois mundos:

```cpp
constexpr double sin_deg(double g) {
    return std::sin(g * PI / 180.0);
}

// Funciona com constante (compile-time)
constexpr double x = sin_deg(45);

// E também com input do usuário (runtime)
double y;
std::cin >> y;
double z = sin_deg(y);
```

Assim você tem **flexibilidade** e **performance** quando possível! 🚀
