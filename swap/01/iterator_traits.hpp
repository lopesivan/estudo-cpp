#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

// Extrai o tipo do valor para iteradores genéricos (ex: std::vector<T>::iterator)
template<typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::value_type value_type;
};

// Especialização para ponteiros nativos (ex: int*, std::string*)
template<typename ElementType>
struct iterator_traits<ElementType*>
{
    typedef ElementType value_type;
};

#endif
