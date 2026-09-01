#include "iterator_traits.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <cstddef>

// Otimização O(1) para iteradores de Acesso Aleatório (ex: vector,
// ponteiros)
template<typename RandomAccessIt> void
advance_impl(RandomAccessIt& it, int n, random_access_iterator_tag)
{
    it += n;
    std::cout << "[O(1)] Avançou via aritmética direta\n";
}

// Fallback O(n) para iteradores Bidirecionais/Forward (ex:
// std::list)
template<typename InputIt> void
advance_impl(InputIt& it, int n, input_iterator_tag)
{
    while(n-- > 0)
        ++it;
    std::cout << "[O(N)] Avançou elemento por elemento\n";
}

// Interface pública que despacha para a versão correta
template<typename Iterator> void custom_advance(Iterator& it, int n)
{
    using        Category =
        typename iterator_traits<Iterator>::iterator_category;
    advance_impl(it, n, Category{});
}

int main()
{
    std::vector<int> vec    = {10, 20, 30, 40, 50};
    auto             vec_it = vec.begin();
    custom_advance(vec_it, 3);  // Executa a versão O(1)

    std::list<int> lst    = {10, 20, 30, 40, 50};
    auto           lst_it = lst.begin();
    custom_advance(lst_it, 3);  // Executa a versão O(N)
}
