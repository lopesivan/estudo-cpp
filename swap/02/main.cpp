#include "iterator_traits.hpp"
#include <utility>
#include <vector>
#include <list>
#include <array>
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
    template<typename ForwardIt1, typename ForwardIt2>
    void swap_pointed_values(ForwardIt1 it1, ForwardIt2 it2) {
        typename iterator_traits<ForwardIt1>::value_type temp = *it1;
        *it1 = *it2;
        *it2 = temp;
    }

    // Troca os iteradores em si (troca de referências/ponteiros)
    template<typename ForwardIt>
    void swap_iterators_themselves(ForwardIt& it1, ForwardIt& it2) {
        std::swap(it1, it2);
    }

} // namespace custom

// Função auxiliar apenas para imprimir coleções
template<typename Container>
void print_container(const std::string& label, const Container& c) {
    std::cout << label << ": ";
    for (const auto& elem : c) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== Exemplo 1: Iterador Normal vs Reverse Iterator (std::vector) ===\n";
    std::vector<std::string> words = {"a", "b", "c"};
    print_container("Antes ", words);

    // .rbegin() retorna um reverse_iterator, que é um tipo diferente de .begin()
    custom::swap_pointed_values(words.begin(), words.rbegin());
    print_container("Depois", words); // Saída esperada: c b a


    std::cout << "\n=== Exemplo 2: Ponteiros Brutos (C-Arrays) ===\n";
    int numbers[] = {10, 20, 30, 40};

    std::cout << "Antes : " << numbers[0] << " " << numbers[3] << "\n";
    // Usa a especialização de iterator_traits<T*> para ponteiros nativos
    custom::swap_pointed_values(&numbers[0], &numbers[3]);
    std::cout << "Depois: " << numbers[0] << " " << numbers[3] << "\n"; // Saída esperada: 40 10


    std::cout << "\n=== Exemplo 3: Troca entre Containers Diferentes (std::vector e std::list) ===\n";
    std::vector<int> vec = {100, 200};
    std::list<int> lst = {999, 888};

    print_container("Vector antes", vec);
    print_container("List antes  ", lst);

    // Tipos de iteradores completamente diferentes (vector::iterator vs list::iterator)
    custom::swap_pointed_values(vec.begin(), lst.begin());

    print_container("Vector depois", vec); // Vector[0] virou 999
    print_container("List depois  ", lst); // List[0] virou 100


    std::cout << "\n=== Exemplo 4: Troca dos Iteradores em si (Variáveis) ===\n";
    std::array<int, 3> arr = {1, 2, 3};
    auto it_first = arr.begin();
    auto it_last = arr.end() - 1;

    std::cout << "Apontado por it_first: " << *it_first << "\n"; // 1
    std::cout << "Apontado por it_last : " << *it_last << "\n";  // 3

    // Aqui trocamos para ONDE as variáveis it_first e it_last apontam
    custom::swap_iterators_themselves(it_first, it_last);

    std::cout << "Após swap_iterators_themselves:\n";
    std::cout << "Apontado por it_first: " << *it_first << "\n"; // Agora é 3
    std::cout << "Apontado por it_last : " << *it_last << "\n";  // Agora é 1
    print_container("Array original (intacto)", arr);             // Array não mudou: 1 2 3

    return 0;
}
