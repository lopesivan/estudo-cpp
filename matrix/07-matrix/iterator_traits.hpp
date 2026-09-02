#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef>
#include <iterator>
#include <utility>

namespace custom {

// Tags Reutilizadas da STL
using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;

// Template Primario (Iteradores de Classe)
template <typename Iterator>
struct iterator_traits {
    using difference_type   = typename Iterator::difference_type;
    using value_type        = typename Iterator::value_type;
    using pointer           = typename Iterator::pointer;
    using reference         = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};

// Especialização Ponteiros Brutos (T*)
template <typename T>
struct iterator_traits<T*> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = custom::random_access_iterator_tag;
};

// Especialização Ponteiros Brutos Constantes (const T*)
template <typename T>
struct iterator_traits<const T*> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = const T*;
    using reference         = const T&;
    using iterator_category = custom::random_access_iterator_tag;
};

// --- custom::advance ---

template <typename RandomAccessIt>
void advance_impl(RandomAccessIt &it, int n, random_access_iterator_tag) {
    it += n;
}

template <typename InputIt>
void advance_impl(InputIt &it, int n, input_iterator_tag) {
    while (n-- > 0) ++it;
}

template <typename Iterator>
void advance(Iterator &it, int n) {
    using Category = typename iterator_traits<Iterator>::iterator_category;
    advance_impl(it, n, Category{});
}

// --- custom::distance ---

template <typename RandomAccessIt>
typename iterator_traits<RandomAccessIt>::difference_type
distance_impl(RandomAccessIt first, RandomAccessIt last, random_access_iterator_tag) {
    return last - first;
}

template <typename InputIt>
typename iterator_traits<InputIt>::difference_type
distance_impl(InputIt first, InputIt last, input_iterator_tag) {
    typename iterator_traits<InputIt>::difference_type count = 0;
    while (first != last) {
        ++first;
        ++count;
    }
    return count;
}

template <typename Iterator>
typename iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last) {
    using Category = typename iterator_traits<Iterator>::iterator_category;
    return distance_impl(first, last, Category{});
}

// --- custom::iter_swap ---

template <typename ForwardIt1, typename ForwardIt2>
void iter_swap(ForwardIt1 it1, ForwardIt2 it2) {
    typename iterator_traits<ForwardIt1>::value_type temp = std::move(*it1);
    *it1 = std::move(*it2);
    *it2 = std::move(temp);
}

} // namespace custom

#endif
