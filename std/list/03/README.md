Para usar o `playlist.back()`, você ganha em leitura direta
do código, mas precisa prestar atenção em uma **diferença
importante em relação aos iteradores**:

* **`playlist.back()`** retorna uma **referência** (`Musica&`)
ao último elemento real.
* **`--playlist.end()`** retorna um **iterador**
(`std::list<Musica>::iterator`) apontando para o último
elemento real.

---

### Exemplo Prático: Lendo e Modificando com `playlist.back()`

Você pode usar o `playlist.back()` para inspecionar, copiar
ou até alterar a última música diretamente:

```cpp
#include <iostream>
#include <list>
#include <string>

struct Musica {
    std::string titulo;
    std::string artista;
};

int main() {
    std::list<Musica> playlist = {
        {"Bohemian Rhapsody", "Queen"},
        {"Hotel California", "Eagles"},
        {"Stairway to Heaven", "Led Zeppelin"}
    };

    // 1. Apenas LER o último elemento
    Musica ultima = playlist.back();
    std::cout << "Tocando agora (última): " << ultima.titulo << "\n";

    // 2. MODIFICAR o último elemento via referência
    playlist.back().titulo = "Stairway to Heaven (Remastered)";

    std::cout << "Título atualizado: " << playlist.back().titulo << "\n";

    return 0;
}

```

---

### Quando usar `back()` vs `--end()`?

* **Use `playlist.back()**` quando você quiser apenas
**acessar ou modificar** o valor do último elemento. É mais
legível e não expõe a complexidade dos iteradores.

* **Use `--playlist.end()**` quando você precisar
do **iterador** para passar para métodos que exigem
posições, como `playlist.insert()`, `playlist.erase()` ou
`playlist.splice()`.

Por exemplo, se tentássemos mover a última música com
`splice()` usando `back()`, o compilador daria erro porque o
`splice()` espera o iterador e não a referência do elemento:

```cpp
// ❌ Não compila (back() é referência, splice exige iterador):
// playlist.splice(playlist.begin(), playlist, playlist.back());

// Erro de tipo! O splice exige o iterador:
playlist.splice(playlist.begin(), playlist, --playlist.end());

```
