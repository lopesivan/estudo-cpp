O que estes exemplos demonstram:
* Exemplo 1 (words.begin() vs words.rbegin()): Mostra a
utilidade da overload com dois tipos de templates diferentes
(ForwardIt1 e ForwardIt2), permitindo combinar tipos de
iteradores incompatíveis.

* Exemplo 2 (Ponteiros brutos): Prova que a especialização
iterator_traits<T*> funciona corretamente ao extrair o
value_type de tipos de ponteiros nativos como int*.

* Exemplo 3 (std::vector com std::list): Ilustra a flexibilidade
do design em C++: podemos trocar valores apontados mesmo que o
layout de memória de cada estrutura seja totalmente distinto.

* Exemplo 4 (swap_iterators_themselves): Deixa evidente a
diferença conceitual entre trocar o dado dentro da coleção
e trocar a posição para onde a variável iterador aponta.


A intuição de querer colocar `&` em tudo faz todo sentido à
primeira vista, mas no design do C++ existe uma distinção
fundamental entre **o iterador (o ponteiro/visão)** e **o
valor para o qual ele aponta**.

---

### 1. Por que `swap_pointed_values` passa por valor (SEM `&`)?

Iteradores em C++ (e ponteiros brutos) foram projetados para
imitar ponteiros da linguagem C: **eles são leves e baratos
de copiar** (geralmente apenas um endereço de memória de
8 bytes).

Quando você passa por valor:

```cpp
template<typename ForwardIt1, typename ForwardIt2>
void swap_pointed_values(ForwardIt1 it1, ForwardIt2 it2) {
    typename iterator_traits<ForwardIt1>::value_type temp = *it1;
    *it1 = *it2;
    *it2 = temp;
}

```

* **O que é copiado:** Apenas as variáveis `it1` e `it2`
(o "endereço" ou "posição").
* **O que é modificado via `*it1`:** O objeto original que
reside dentro da coleção. Ao desreferenciar (`*it1 = *it2`),
a atribuição ocorre no contêiner original, e não na cópia
do iterador.

#### E se colocássemos `&` em `swap_pointed_values`?

Se a assinatura fosse `(ForwardIt1& it1, ForwardIt2& it2)`, o
código **quebraria ao passar rvalues (valores temporários)**.

No seu próprio exemplo:

```cpp
custom::swap_pointed_values(words.begin(), words.rbegin());

```

`words.begin()` e `words.rbegin()` retornam **objetos
temporários (rvalues)**. Em C++, uma referência
não-constante (`T&`) não pode se conectar a um
temporário. Passando por valor (`ForwardIt it1`), o
temporário é copiado diretamente para a função sem erros
de compilação.

---

### 2. Por que `swap_iterators_themselves` OBRIGA o uso do `&`?

Esta função **não quer alterar o conteúdo apontado**, mas
sim alterar as próprias variáveis que guardam os iteradores
no escopo de quem chamou (`main`):

```cpp
template<typename ForwardIt>
void swap_iterators_themselves(ForwardIt& it1, ForwardIt& it2) {
    std::swap(it1, it2);
}

```

Se tirássemos o `&`:

* `std::swap` trocaria apenas as cópias locais de `it1` e
`it2` dentro da função.
* Ao sair da função, as variáveis do `main` continuariam
apontando para os mesmos lugares originais. O efeito seria
nulo.

---

### 3. Por que a primeira assinatura é redundante?

Na verdade, existe outro detalhe: **a primeira overload nem precisava existir!**

| Função | Comportamento com tipos iguais | Comportamento com tipos diferentes |
| --- | --- | --- |
| **Apenas 1 tipo (`ForwardIt`)** | Funciona (`vector::iterator` e `vector::iterator`) | **Falha ao compilar** (`vector::iterator` e `vector::reverse_iterator`) |
| **2 tipos (`ForwardIt1`, `ForwardIt2`)** | **Funciona perfeitamente** (o compilador deduce `ForwardIt1 = ForwardIt2`) | **Funciona perfeitamente** |

A versão com dois parâmetros de template cobre 100% dos
casos. A versão de 1 tipo é um resíduo de código que pode
ser totalmente removido sem perda de funcionalidade.

---

### Resumo Visual

* **Sem `&` (`ForwardIt it`):** Copia o "apontador". Usado
quando você quer acessar e modificar o conteúdo remoto
(`*it = val`). Permite temporários como `vec.begin()`.
* **Com `&` (`ForwardIt& it`):** Requer uma variável existente
na memória. Usado exclusivamente quando você quer reatribuir
a própria variável iterador (`it = novo_iterador`).


Este header implementa uma versão simplificada do
`std::iterator_traits` da biblioteca padrão do C++.

Ele resolve um problema clássico de **metaprogramação**:
como descobrir o tipo do elemento apontado por algo, garantindo
que o código funcione **tanto para iteradores de classes
quanto para ponteiros brutos da linguagem C**.

---

### Por que esse header é necessário?

1. **Iteradores de containers (`std::vector<int>::iterator`):**
São classes/structs. Elas guardam internamente uma definição
de tipo chamada `value_type` (que no caso é `int`). Para
obter esse tipo, acessamos `Iterator::value_type`.
2. **Ponteiros nativos (`int*`):** Não são classes! Se você
tentar fazer `(int*)::value_type`, o compilador gera erro
porque um ponteiro bruto não possui membros nem `typedef`
internalizado.

O `iterator_traits` atua como uma **interface unificada (traits)**:

```
                  ┌───────────────────────────────┐
                  │iterator_traits<T>::value_type │
                  └───────────────┬───────────────┘
                                  │
         ┌────────────────────────┴────────────────────────┐
         ▼                                                 ▼
[Container Iterator]                               [Ponteiro Bruto]
(ex: vector<int>::iterator)                         (ex: int*)
   ↓ Usa versão Primária                               ↓ Usa Especialização
Acessa Iterator::value_type                        Extrai direto o tipo base (int)

```

---

### O que o código faz linha por linha:

* **Include Guards (`#ifndef ...`):** Evitam erros de
redefinição se o arquivo for incluído mais de uma vez.
* **Template Primário (`struct iterator_traits`):** Usado para
qualquer classe de iterador. Ele assume que o tipo `Iterator`
possui um alias interno `value_type`. A palavra-chave
`typename` é obrigatória aqui para avisar o compilador de
que `Iterator::value_type` é um tipo, não uma variável.
* **Especialização de Template (`struct
iterator_traits<ElementType*>`):** Acionada quando passamos
um ponteiro bruto (`T*`). Ela ignora o `::value_type` e pega
diretamente o tipo apontado `ElementType`.

---

### Simplificando o Header

A partir do **C++11**, podemos substituir o antigo `typedef`
pelo operador `using`, que é a sintaxe moderna, mais limpa
e legível:

```cpp
#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

// 1. Caso Geral: Classes de Iteradores (ex: std::vector<T>::iterator)
template<typename Iterator>
struct iterator_traits {
    using value_type = typename Iterator::value_type;
};

// 2. Especialização: Ponteiros brutos (ex: int*, std::string*)
template<typename T>
struct iterator_traits<T*> {
    using value_type = T;
};

#endif

```

*(Nota: Mantivemos o `T` na especialização por ser a
convenção universal do C++ para tipos genéricos).*

Aqui está a expansão do `iterator_traits.hpp` incluindo
todas as 5 propriedades (*associated types*) exigidas pelo
standard do C++: **`value_type`**, **`difference_type`**,
**`pointer`**, **`reference`** e **`iterator_category`**.

### `iterator_traits.hpp`

```cpp
#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef> // std::ptrdiff_t

// 1. Tags de Categoria de Iteradores (Conforme std::iterator_tags)
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 2. Template Primário (Para classes de iteradores customizadas ou da STL)
template<typename Iterator>
struct iterator_traits {
    using difference_type   = typename Iterator::difference_type;
    using value_type        = typename Iterator::value_type;
    using pointer           = typename Iterator::pointer;
    using reference         = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};

// 3. Especialização para Ponteiros Brutos Mutáveis (ex: int*, std::string*)
template<typename T>
struct iterator_traits<T*> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = random_access_iterator_tag; // Ponteiros brutos têm acesso aleatório
};

// 4. Especialização para Ponteiros Brutos Constantes (ex: const int*)
template<typename T>
struct iterator_traits<const T*> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = const T*;
    using reference         = const T&;
    using iterator_category = random_access_iterator_tag;
};

#endif

```

---

### O que cada associação significa na prática:

* **`difference_type`**: O tipo numérico com sinal usado para
guardar o resultado da subtração de dois iteradores (ex:
`it2 - it1`). Para ponteiros brutos, usa-se `std::ptrdiff_t`.
* **`pointer`**: O tipo do ponteiro para o elemento (ex:
`T*` ou `const T*`).
* **`reference`**: O tipo do retorno ao desreferenciar o
iterador via `*it` (geralmente `T&` ou `const T&`).
* **`iterator_category`**: Uma *tag* (struct
vazia) que identifica as capacidades do iterador
em tempo de compilação. Ponteiros brutos recebem
`random_access_iterator_tag` porque suportam aritmética de
ponteiros direta (como `ptr + 5` ou `ptr[3]`).

---

### Exemplo prático de uso com Dispatch por Tag

Com a `iterator_category`, você pode criar otimizações em
tempo de compilação usando *Tag Dispatching*:

```cpp
#include "iterator_traits.hpp"
#include <iostream>
#include <vector>
#include <list>

// Otimização O(1) para iteradores de Acesso Aleatório (ex: vector, ponteiros)
template<typename RandomAccessIt>
void advance_impl(RandomAccessIt& it, int n, random_access_iterator_tag) {
    it += n;
    std::cout << "[O(1)] Avançou via aritmética direta\n";
}

// Fallback O(n) para iteradores Bidirecionais/Forward (ex: std::list)
template<typename InputIt>
void advance_impl(InputIt& it, int n, input_iterator_tag) {
    while (n-- > 0) ++it;
    std::cout << "[O(N)] Avançou elemento por elemento\n";
}

// Interface pública que despacha para a versão correta
template<typename Iterator>
void custom_advance(Iterator& it, int n) {
    using Category = typename iterator_traits<Iterator>::iterator_category;
    advance_impl(it, n, Category{});
}

int main() {
    std::vector<int> vec = {10, 20, 30, 40, 50};
    auto vec_it = vec.begin();
    custom_advance(vec_it, 3); // Executa a versão O(1)

    std::list<int> lst = {10, 20, 30, 40, 50};
    auto lst_it = lst.begin();
    custom_advance(lst_it, 3); // Executa a versão O(N)
}

```

