#include <fstream>
#include <iostream>
#include <string>

struct Pessoa
{
    std::string nome;
    std::string telefone;
};

int main()
{
    Pessoa p = {"João Silva", "1234-5678"};

    std::ofstream file ("dados.bin", std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo" << std::endl;
        return 1;
    }

    size_t nomeSize = p.nome.size();
    file.write (reinterpret_cast<char*> (&nomeSize), sizeof (nomeSize));
    file.write (p.nome.c_str(), nomeSize);

    size_t telefoneSize = p.telefone.size();
    file.write (reinterpret_cast<char*> (&telefoneSize), sizeof (telefoneSize));
    file.write (p.telefone.c_str(), telefoneSize);

    file.close();
    return 0;
}
