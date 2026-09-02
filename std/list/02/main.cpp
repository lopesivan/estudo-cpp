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
