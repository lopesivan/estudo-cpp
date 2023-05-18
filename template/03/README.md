Em C++, os templates permitem escrever funções ou classes
genéricas que podem ser usadas com diferentes tipos de dados.
Para usar um template, você precisa definir um parâmetro de
template, que é um identificador que representa um tipo de dado
desconhecido.

Vamos usar o exemplo do fatorial para explicar como especificar
um template em C++. Na função factorial, usamos typename T para
definir um parâmetro de template que representa um tipo de dados
desconhecido. Em seguida, usamos T em vez de um tipo de dados
específico em todo o código da função. Isso faz com que a função
factorial funcione com qualquer tipo de dados que escolhermos
usar com ela.

Para usar a função factorial com um tipo de dados específico,
precisamos especificar o tipo de dados quando chamamos a
função. Isso é feito colocando o tipo de dados dentro de
colchetes angulares (<>) imediatamente após o nome da
função. Por exemplo:

code


Neste exemplo, usamos dois parâmetros de template, typename T1 e
typename T2, para representar dois tipos de dados desconhecidos.
A função retorna o máximo entre a e b e usa a sintaxe decltype(a + b) 
para determinar o tipo de dados que a função deve retornar.

Para usar essa função, podemos chamar a função max e especificar
os tipos de dados dos argumentos usando colchetes angulares,
como mostrado no exemplo abaixo:

code
