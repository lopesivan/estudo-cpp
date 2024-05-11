#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

int main() {
    std::string filename = "exemplo.txt";
    std::ifstream file;

    try {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir o arquivo: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }

        file.close();
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

