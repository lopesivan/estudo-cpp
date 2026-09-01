// iterator_traits.hpp
#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef>
#include <iterator> // Reusa std::input_iterator_tag, std::random_access_iterator_tag, etc.

namespace custom {

// Usamos as tags standard para ter interoperabilidade total com a STL
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

} // namespace custom

#endif
