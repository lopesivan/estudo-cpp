// main.cpp (Versão C++20 com Concepts)
#include "iterator_traits.hpp"
#include <iostream>
#include <vector>
#include <list>

namespace custom {

// Versão O(1) para iteradores de Acesso Aleatório
template <typename Iterator>
    requires std::derived_from<typename iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>
void advance(Iterator &it, int n) {
    it += n;
    std::cout << "[C++20 Concept O(1)] Avançou via +=\n";
}

// Versão O(N) para iteradores genéricos (Forward/Bidirectional)
template <typename Iterator>
void advance(Iterator &it, int n) {
    while (n-- > 0) ++it;
    std::cout << "[C++20 Concept O(N)] Avançou via ++\n";
}

} // namespace custom

int main() {
    std::vector<int> vec = {10, 20, 30, 40, 50};
    auto vec_it = vec.begin();
    custom::advance(vec_it, 3); // Compila e seleciona a versão O(1)

    std::list<int> lst = {10, 20, 30, 40, 50};
    auto lst_it = lst.begin();
    custom::advance(lst_it, 3); // Compila e seleciona a versão O(N)
}
