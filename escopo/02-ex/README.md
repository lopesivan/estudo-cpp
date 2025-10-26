**Não**, você não consegue fazer `m1_(s)` na lista de
inicialização porque `m1_` é um **array** (`char m1_[20]`),
e arrays não podem ser inicializados diretamente com ponteiros
na lista de inicialização.

## Por quê?

Arrays em C++ não são copiáveis. Você não pode atribuir
um array a outro array diretamente:

```cpp
char m1_[20];
m1_ = "texto";  // ERRO! Arrays não podem ser atribuídos
```

## O que você PODE fazer:

### 1. Manter como está (correto):

```cpp
C(const char* s = "Hello!", int i = 0, double d = 1.0)
{
    strncpy(m1_, s, sizeof(m1_) - 1);  // Copia no corpo do construtor
    m1_[sizeof(m1_) - 1] = '\0';       // Garante terminação
    m2_ = i;
    m3_ = d;
}
```

### 2. Usar lista de inicialização apenas para m2_ e m3_:

```cpp
C(const char* s = "Hello!", int i = 0, double d = 1.0)
    : m2_(i), m3_(d)  // Inicializa estes dois
{
    strncpy(m1_, s, sizeof(m1_) - 1);  // m1_ precisa ser no corpo
    m1_[sizeof(m1_) - 1] = '\0';
}
```

### 3. Se quiser usar lista de inicialização para tudo, troque por `std::string`:

```cpp
#include <string>

class C
{
  public:
    C(const char* s = "Hello!", int i = 0, double d = 1.0)
        : m1_(s), m2_(i), m3_(d)  // Agora funciona!
    {
    }
  private:
    std::string m1_;  // string em vez de char[]
    int    m2_;
    double m3_;
};
```

**Resumo**: Com `char m1_[20]`, você **precisa** usar
`strncpy` no corpo do construtor. A lista de inicialização
não funciona para arrays de caracteres.
