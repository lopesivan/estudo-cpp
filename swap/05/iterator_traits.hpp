#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef>
#include <iterator>

namespace custom {

// Reuso de tags standard para compatibilidade total
using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;

// Template Primário
template <typename Iterator>
struct iterator_traits {
    using difference_type   = typename Iterator::difference_type;
    using value_type        = typename Iterator::value_type;
    using pointer           = typename Iterator::pointer;
    using reference         = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};

// Especialização para Ponteiros Brutos (T*)
template <typename T>
struct iterator_traits<T*> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = custom::random_access_iterator_tag;
};

// Especialização para Ponteiros Brutos Constantes (const T*)
template <typename T>
struct iterator_traits<const T*> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = const T*;
    using reference         = const T&;
    using iterator_category = custom::random_access_iterator_tag;
};

// --- Implementação do custom::advance ---

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

// --- Implementação do custom::distance ---

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

} // namespace custom

#endif
