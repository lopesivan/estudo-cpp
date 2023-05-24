# Diferença entre `consteval` e `constexpr`

`consteval` e `constexpr` são palavras-chave introduzidas no
padrão C++20 para especificar funções que podem ser avaliadas em
tempo de compilação. Embora eles sejam semelhantes em alguns
aspectos, existem diferenças importantes entre eles.

## As principais diferenças

Aqui estão as diferenças principais entre `consteval` e
`constexpr`:

1. Tempo de avaliação: 
   - `consteval`: A função `consteval` deve ser avaliada em
   tempo de compilação, garantindo que seu resultado seja
   conhecido em tempo de compilação. Se uma chamada a uma função
   `consteval` não puder ser avaliada em tempo de compilação,
   ocorrerá um erro de compilação.
   - `constexpr`: A função `constexpr` pode ser avaliada tanto
   em tempo de compilação quanto em tempo de execução,
   dependendo do contexto em que é usada. Se uma chamada a uma
   função `constexpr` não puder ser avaliada em tempo de
   compilação, ela será avaliada em tempo de execução.

2. Restrições de uso:
   - `consteval`: Uma função `consteval` só pode conter um
   conjunto limitado de instruções, conhecidas como instruções
   "core constant expressions" (expressões constantes
           principais). Isso significa que o corpo da função
   `consteval` deve ser composto por instruções que podem ser
   avaliadas em tempo de compilação, como atribuições, loops e
   chamadas a outras funções `consteval`.
   - `constexpr`: Uma função `constexpr` tem menos restrições em
   relação ao seu corpo. Ela pode conter uma variedade mais
   ampla de instruções, incluindo instruções condicionais e
   loops mais complexos. No entanto, é importante lembrar que o
   corpo da função `constexpr` deve ser avaliável em tempo de
   compilação sempre que for necessário.

Em resumo, `consteval` é mais restritivo e garante que a função
seja avaliada em tempo de compilação, enquanto `constexpr` é
mais flexível e permite avaliação em tempo de compilação ou em
tempo de execução, dependendo do contexto. A escolha entre
`consteval` e `constexpr` depende das necessidades específicas e
das restrições do problema em questão.
