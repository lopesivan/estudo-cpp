O que estes exemplos demonstram:
Exemplo 1 (words.begin() vs words.rbegin()): Mostra a utilidade da overload com dois tipos de templates diferentes (ForwardIt1 e ForwardIt2), permitindo combinar tipos de iteradores incompatíveis.

Exemplo 2 (Ponteiros brutos): Prova que a especialização iterator_traits<T*> funciona corretamente ao extrair o value_type de tipos de ponteiros nativos como int*.

Exemplo 3 (std::vector com std::list): Ilustra a flexibilidade do design em C++: podemos trocar valores apontados mesmo que o layout de memória de cada estrutura seja totalmente distinto.

Exemplo 4 (swap_iterators_themselves): Deixa evidente a diferença conceitual entre trocar o dado dentro da coleção e trocar a posição para onde a variável iterador aponta.
