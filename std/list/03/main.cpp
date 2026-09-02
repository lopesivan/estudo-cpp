#include <iostream>
#include <list>
#include <string>
#include <algorithm> // ADICIONE ESTE INCLUDE para std::reverse
//
struct Musica
{
    std::string titulo;
    std::string artista;
};

int main()
{
    std::list<Musica> playlist =
    {
        {"Bohemian Rhapsody", "Queen"},
        {"Hotel California", "Eagles"},
        {"Stairway to Heaven", "Led Zeppelin"},
        {"Imagine", "John Lennon"},
        {"Smells Like Teen Spirit", "Nirvana"},
        {"Like a Rolling Stone", "Bob Dylan"},
        {"Wonderwall", "Oasis"},
        {"Comfortably Numb", "Pink Floyd"},
        {"Purple Haze", "Jimi Hendrix"},
        {"One", "Metallica"}
    };

    //  //////////////////////////////////////////////////////////////////////
    std::cout << "--- PLAYLIST ATUAL ---\n";
    int pos = 1;
    for (const auto& m : playlist)
    {
        std::cout << pos++ << ". " << m.titulo << " - " << m.artista << "\n";
    }
    std::cout << "\n";

    // 1 -////////////////////////////////////////////////////////////////////
    std::cout << "--- PLAYLIST REVERSA ---\n";
    pos = playlist.size();
    for (auto it = playlist.rbegin(); it != playlist.rend(); ++it)
    {
        std::cout << pos-- << ". " << it->titulo << " - " << it->artista << "\n";
    }
    std::cout << "\n";

    // 2 -////////////////////////////////////////////////////////////////////
    std::cout << "--- PLAYLIST REVERSA ---\n";
    std::list<Musica> reversa = playlist;
    std::reverse (reversa.begin(), reversa.end());

    pos = 1;
    for (const auto& m : reversa)
    {
        std::cout << pos++ << ". " << m.titulo << " - " << m.artista << "\n";
    }
    std::cout << "\n";

    pos = 1;
    for (const auto& m : playlist)
    {
        std::cout << pos++ << ". " << m.titulo << " - " << m.artista << "\n";
    }
    std::cout << "\n";
    return 0;
}
