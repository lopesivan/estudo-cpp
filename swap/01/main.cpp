#include "iterator_traits.hpp"
#include <utility>
#include <vector>
#include <string>
#include <iostream>

namespace custom {

    // Troca os valores APONTADOS por dois iteradores do mesmo tipo
    template<typename ForwardIt>
    void swap_pointed_values(ForwardIt it1, ForwardIt it2) {
        typename iterator_traits<ForwardIt>::value_type temp = *it1;
        *it1 = *it2;
        *it2 = temp;
    }

    // Troca os valores APONTADOS por dois iteradores de tipos diferentes
    // (ex: vector::iterator e vector::reverse_iterator)
    template<typename ForwardIt1, typename ForwardIt2>
    void swap_pointed_values(ForwardIt1 it1, ForwardIt2 it2) {
        typename iterator_traits<ForwardIt1>::value_type temp = *it1;
        *it1 = *it2;
        *it2 = temp;
    }

    // Se o seu objetivo for trocar a posição dos iteradores em si:
    template<typename ForwardIt>
    void swap_iterators_themselves(ForwardIt& it1, ForwardIt& it2) {
        std::swap(it1, it2);
    }

} // namespace custom

int main() {
    std::vector<std::string> words = {"a", "b", "c"};

    // Troca o valor no início ("a") pelo valor no final ("c")
    custom::swap_pointed_values(words.begin(), words.rbegin());

    std::cout << words[0] << " " << words[2] << std::endl; // Saída: c a

    return 0;
}
