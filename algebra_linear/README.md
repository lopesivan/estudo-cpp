```{cpp}
#include <iostream>
#include <numeric>  // inner_product
#include <vector>

int dotProduct(const std::vector<int>& vec1,
               const std::vector<int>& vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cout << "Erro: os vetores precisam ter o mesmo tamanho"
                  << std::endl;
        return 0;
    }
    return std::inner_product(
        vec1.begin(), vec1.end(), vec2.begin(), 0);
}
```

------------------------------------------------------------------------------

```{cpp}
#include <iostream>
#include <numeric>  // inner_product
#include <vector>

template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cout << "Erro: os vetores precisam ter o mesmo tamanho"
                  << std::endl;
        return T(0);
    }
    return std::inner_product(
        vec1.begin(), vec1.end(), vec2.begin(), T(0));
}
```

------------------------------------------------------------------------------

```{cpp}
#include <numeric>  // inner_product
#include <functional>
#include <vector>
#include <iostream>

template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    return inner_product(vec1.begin(),
                         vec1.end(),
                         vec2.begin(),
                         T{},
                         std::plus<>{},
                         std::multiplies<>{});
}
```

------------------------------------------------------------------------------

```{cpp}
#include <vector>
#include <iostream>
#include <execution>  // para política de execução paralela (opcional)
#include <numeric>

template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    if(vec1.size() != vec2.size())
        return T{};
    return std::transform_reduce(
        vec1.begin(), vec1.end(), vec2.begin(), T{});
    // ou paralelo: transform_reduce(std::execution::par,
    // vec1.begin(), vec1.end(), vec2.begin(), T{});
}
```


## Explicação detalhada de cada modelo apresentado

---

### **Modelo 1: Versão Específica para `int`**

```cpp
int dotProduct(const std::vector<int>& vec1, const std::vector<int>& vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cout << "Erro: os vetores precisam ter o mesmo tamanho" << std::endl;
        return 0;
    }
    return std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0);
}
```

**Características:**
- **Tipo fixo**: Funciona APENAS com vetores de `int`
- **Verificação de tamanho**: Valida se os vetores têm o mesmo comprimento
- **Tratamento de erro**: Exibe mensagem e retorna `0` em caso de incompatibilidade
- **Cálculo**: Usa `std::inner_product` da biblioteca `<numeric>`
  - `vec1.begin(), vec1.end()`: intervalo do primeiro vetor
  - `vec2.begin()`: início do segundo vetor
  - `0`: valor inicial da soma (acumulador)

**Limitações:**
- Não funciona com `double`, `float` ou outros tipos numéricos
- Código duplicado se precisar de suporte para outros tipos

---

### **Modelo 2: Versão Genérica com Template**

```cpp
template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cout << "Erro: os vetores precisam ter o mesmo tamanho" << std::endl;
        return T(0);
    }
    return std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), T(0));
}
```

**Características:**
- **Tipo genérico**: Funciona com QUALQUER tipo numérico (`int`, `double`, `float`, etc.)
- **Sobrecarga implícita**: O compilador gera versões específicas para cada tipo usado
- **Valor zero genérico**: `T(0)` cria o valor zero do tipo correto
- **Mantém validação**: A mesma verificação de tamanho e tratamento de erro

**Vantagens:**
- Código reutilizável e tipo-independente
- Fácil manutenção (uma única implementação)
- Segurança de tipos em tempo de compilação

**Desvantagens:**
- O template é instanciado para cada tipo, podendo aumentar o tamanho do código

---

### **Modelo 3: Versão com Operadores Padrão Explícitos**

```cpp
template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    return inner_product(vec1.begin(),
                         vec1.end(),
                         vec2.begin(),
                         T{},           // Valor inicial (zero do tipo T)
                         std::plus<>{}, // Operação de soma
                         std::multiplies<>{}); // Operação de multiplicação
}
```

**Características:**
- **Sem verificação explícita**: Assume que o usuário garante tamanhos iguais
- **Operadores padrão C++14/17**:
  - `std::plus<>{}`: função de soma (transparente, aceita qualquer tipo)
  - `std::multiplies<>{}`: função de multiplicação (transparente)
- **Inicialização uniforme**: `T{}` é preferível a `T(0)` em C++11+
- **Explícito**: Mostra claramente quais operações são realizadas

**Vantagens:**
- Código mais expressivo e autodocumentado
- Flexibilidade: poderia ser modificado para outras operações (ex: subtração)
- Sem dependência de cabeçalhos extras além do necessário

**Desvantagens:**
- Remove a verificação de segurança (assume que os vetores têm o mesmo tamanho)

---

### **Modelo 4: Versão com `transform_reduce` (C++17)**

```cpp
template <typename T>
T dotProduct(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    if(vec1.size() != vec2.size())
        return T{};
    return std::transform_reduce(
        vec1.begin(), vec1.end(), vec2.begin(), T{});
    // Versão paralela (comentada):
    // transform_reduce(std::execution::par, vec1.begin(), vec1.end(),
    //                  vec2.begin(), T{});
}
```

**Características:**
- **C++17 moderno**: Usa `std::transform_reduce` (mais poderoso que `inner_product`)
- **Verificação de tamanho**: Retorna `T{}` (zero) se os tamanhos forem diferentes
- **Sem mensagem de erro**: Retorno silencioso (zero) em caso de erro
- **Suporte a paralelismo**: Pode usar `std::execution::par` para computação paralela

**Vantagens:**
- **Performance**: `transform_reduce` pode ser otimizado e paralelizado
- **Flexibilidade**: Fácil adicionar paralelismo com política de execução
- **Código conciso**: Menos verboso que `inner_product` com operadores explícitos

**Desvantagens:**
- Requer C++17 ou superior
- A versão sem política de execução não é necessariamente mais rápida que `inner_product`
- O tratamento de erro silencioso pode ocultar problemas

---

## Comparação entre os modelos

| Característica | Modelo 1 | Modelo 2 | Modelo 3 | Modelo 4 |
|----------------|----------|----------|----------|----------|
| **Tipo genérico** | ❌ | ✅ | ✅ | ✅ |
| **Verificação de tamanho** | ✅ | ✅ | ❌ | ✅ |
| **Mensagem de erro** | ✅ | ✅ | ❌ | ❌ |
| **Padrão C++ mínimo** | C++98 | C++98 | C++14 | C++17 |
| **Suporte a paralelismo** | ❌ | ❌ | ❌ | ✅ |
| **Clareza do código** | Média | Boa | Ótima | Boa |
| **Performance potencial** | Boa | Boa | Boa | Melhor |

---

## Recomendações de uso

- **Modelo 2**: Melhor para compatibilidade com versões antigas do C++
- **Modelo 3**: Ideal para código moderno com máxima clareza
- **Modelo 4**: Recomendado para projetos C++17+ que precisam de performance
- **Modelo 1**: Apenas para casos muito específicos onde só se trabalha com `int`

**Sugestão final**: Use o **Modelo 4** com a política de
execução paralela para máximo desempenho em aplicações
que lidam com grandes volumes de dados.
