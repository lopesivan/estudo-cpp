`std::move` é uma forma de dizer ao compilador:

> “você pode roubar os recursos deste objeto”

Normalmente:

```text
heap
buffer
ponteiros internos
memória dinâmica
```

---

# 📌 Sem move → cópia

```cpp id="w1zcda"
#include <iostream>
#include <string>

int main() {

    std::string a = "hello";

    std::string b = a;

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
}
```

Resultado:

```text id="8lf3zv"
a = hello
b = hello
```

Internamente:

```text id="g6ghqh"
a → buffer "hello"
b → NOVO buffer "hello"
```

👉 houve cópia.

---

# 📌 Com move

```cpp id="y9r26f"
#include <iostream>
#include <string>

int main() {

    std::string a = "hello";

    std::string b = std::move(a);

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
}
```

Possível resultado:

```text id="bw4nzz"
a =
b = hello
```

Agora:

```text id="1smv8m"
b roubou o buffer de a
```

Sem copiar caracteres.

---

# 📌 O que std::move faz REALMENTE?

Ele NÃO move sozinho.

Ele apenas faz cast:

```cpp id="qt8lgq"
std::move(x)
```

≈

```cpp id="62cfqj"
static_cast<T&&>(x)
```

Ou seja:

```text id="gw9i80"
"trate isto como um objeto movível"
```

---

# 📌 Exemplo visual

Sem move:

```text id="pv5z9k"
a ---> [HELLO]

b ---> [HELLO]
```

Dois buffers.

---

Com move:

```text id="a50p62"
a ---> vazio/null

b ---> [HELLO]
```

O buffer foi transferido.

---

# 📌 Onde move é MUITO útil

## std::vector

Sem move:

```cpp id="r5i1rv"
std::vector<int> a(1000000);

std::vector<int> b = a;
```

👉 copia 1 milhão de elementos.

---

Com move:

```cpp id="zv74q2"
std::vector<int> a(1000000);

std::vector<int> b = std::move(a);
```

👉 só transfere ponteiros internos.

Muito rápido.

---

# 📌 Exemplo clássico de função

## Sem move

```cpp id="msyazv"
class Test {
public:

    Test(std::string s)
        : value_(s)
    {
    }

private:
    std::string value_;
};
```

Aqui:

```text id="dbs49p"
s → value_
```

faz cópia.

---

## Com move

```cpp id="7q6ikj"
class Test {
public:

    Test(std::string s)
        : value_(std::move(s))
    {
    }

private:
    std::string value_;
};
```

Agora:

```text id="vzn9fy"
value_ rouba o conteúdo de s
```

---

# 📌 Importante

Depois do move:

```cpp id="fhj31g"
std::string a = "hello";

std::string b = std::move(a);
```

`a` continua válido.

MAS:

```text id="c7ecf9"
estado indefinido semanticamente
```

Você pode:

✔ destruir
✔ atribuir novo valor
✔ chamar métodos válidos

Mas NÃO deve assumir conteúdo.

---

# 📌 Exemplo seguro

```cpp id="qnpk7x"
a.clear();
a = "new value";
```

---

# 📌 Quando usar move?

## ✔ Quando o objeto não será mais usado

```cpp id="1d4do6"
return std::move(x);
```

(embora hoje muitas vezes o compilador já otimize)

---

## ✔ Transferindo ownership

```cpp id="5ekffz"
vec.push_back(std::move(big_object));
```

---

## ✔ Em construtores

```cpp id="yn9kiz"
member_(std::move(value))
```

---

# 📌 Quando NÃO usar

## ❌ Em objetos que você continuará usando

```cpp id="hm4qgg"
std::move(x);

std::cout << x;
```

ruim semanticamente.

---

## ❌ Em objetos const

```cpp id="2mrjpo"
const std::string s = "abc";

std::move(s);
```

não move de verdade.

---

# 📌 Analogia simples

Cópia:

```text id="xjpbxq"
fotocopiar um livro
```

Move:

```text id="7mwg6t"
entregar o livro original
```

---

# 📌 Insight importante

Move existe porque objetos modernos geralmente controlam:

```text id="hl80e7"
heap
arquivos
sockets
buffers
GPU memory
threads
```

Copiar tudo seria caro.

Move transfere posse sem duplicar recursos.

---

# 📌 Regra de ouro

> move = transferência de recursos/ownership

---

# 📌 Conclusão

✔ `std::move` não move sozinho
✔ ele permite chamar move constructors
✔ move evita cópias caras
✔ extremamente importante para C++ moderno/EOP/STL

