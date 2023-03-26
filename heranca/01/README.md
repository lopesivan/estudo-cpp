Em C++, a herança é implementada usando o operador : e a palavra-chave public. As classes filhas, "Gato" e "Cachorro", herdam da classe mãe abstrata "Mamifero". A classe mãe define um método abstrato falar() com a palavra-chave virtual e = 0, indicando que este método deve ser implementado por todas as classes filhas.

A seguir, as classes filhas "Gato" e "Cachorro" são definidas, e cada uma implementa seu próprio método falar(). Note que eles usam a mesma assinatura que o método abstrato na classe mãe.

Na função main(), instâncias das classes filhas são criadas e seus métodos falar() são chamados, produzindo a mesma saída que o exemplo Python.

Observe também que, em C++, é boa prática incluir um destrutor virtual na classe mãe abstrata para garantir que os objetos sejam destruídos adequadamente.
