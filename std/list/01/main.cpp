#include <iostream>
#include <list>
#include <string>
#include <algorithm>

struct Musica
{
    std::string titulo;
    std::string artista;
};

void imprimir_playlist (const std::list<Musica>& playlist)
{
    std::cout << "--- PLAYLIST ATUAL ---\n";
    int pos = 1;
    for (const auto& m : playlist)
    {
        std::cout << pos++ << ". " << m.titulo << " - " << m.artista << "\n";
    }
    std::cout << "\n";
}

int main()
{
    std::list<Musica> playlist =
    {
        {"Bohemian Rhapsody", "Queen"},
        {"Hotel California", "Eagles"},
        {"Stairway to Heaven", "Led Zeppelin"},
        {"Sweet Child O' Mine", "Guns N' Roses"}
    };

    imprimir_playlist (playlist);

    // 1. Inserção rápida no meio da lista: Adicionar uma música na posição 2
    auto it = playlist.begin();
    std::advance (it, 1); // Avança até a 2ª posição (Hotel California)

    // O insert do std::list insere antes do iterador apontado em O(1)
    playlist.insert (it, {"Imagine", "John Lennon"});
    std::cout << "-> [Inserido 'Imagine' na posicao 2]\n";
    imprimir_playlist (playlist);

    // 2. Remoção rápida sem invalidar os outros iteradores
    // Vamos procurar e remover "Hotel California"
    auto it_remover = std::find_if (playlist.begin(), playlist.end(), [] (const Musica& m)
    {
        return m.titulo == "Hotel California";
    });

    if (it_remover != playlist.end())
    {
        playlist.erase (it_remover); // Remoção em O(1)
        std::cout << "-> [Removido 'Hotel California']\n";
    }
    imprimir_playlist (playlist);

    // 3. Mover elementos com .splice() em O(1) (Exclusivo de std::list)
    // Move 'Sweet Child O' Mine' (última) para ser a próxima a tocar (posição 1)
    auto it_ultima = --playlist.end();
    playlist.splice (playlist.begin(), playlist, it_ultima);

    std::cout << "-> [Movel a ultima musica para a primeira posicao com splice()]\n";
    imprimir_playlist (playlist);

    return 0;
}
