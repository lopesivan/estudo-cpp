`distance<N>(x, y, f)` é o algoritmo genérico do EoP (Elements of Programming) que responde a uma pergunta só: **"quantas vezes preciso aplicar a transformação `f` em `x` até chegar em `y`?"**

```cpp
template <class N, class X, class F>
N distance(X x, X y, F f)
{
    N n = N(0);
    while (x != y)
    {
        x = f(x);
        n = n + N(1);
    }
    return n;
}
```

## Os três papéis

- **`X`** — o "espaço" onde `x` e `y` vivem. Não precisa ser número nem iterador: pode ser um `int`, um `Employee*`, uma largura de intervalo (`double`), um ângulo. A única exigência é suportar `==` (ou `>`, na variante contínua) e ser comparável a `y`.
- **`F`** — a transformação, `f: X -> X`. É o "passo": avançar um iterador, subir um nível de gerência, bissectar um intervalo, dobrar um ângulo reduzido.
- **`N`** — o tipo do contador (aqui sempre `int`).

`distance` não sabe nada sobre vetores, hierarquias ou trigonometria — ele só sabe **contar aplicações de `f` até bater no alvo `y`**. Essa é a essência do EoP: extrair o padrão comum por trás de problemas aparentemente diferentes.

## Onde usamos, nesta conversa

| Domínio | `X` | `f` | `y` | O que `distance` respondeu |
|---|---|---|---|---|
| Iteradores de vetor | `vector<int>::iterator` | `++it` | `end()` | tamanho do vetor |
| Conjectura de Collatz | `int` | `x/2` ou `3x+1` | `1` | quantos passos até 1 |
| Hierarquia corporativa | `Employee*` | `->manager` | `nullptr` | nível abaixo do CEO |
| Método da bisseção | `double` (largura) | `w/2` | `eps` | quantas iterações até a tolerância |
| Cálculo de seno | `double` (ângulo) | `θ/2` | `eps` | quantas reduções antes da aproximação de ângulo pequeno |

## O ganho real de pensar assim

Em vez de escrever um `for` diferente para cada um desses cinco problemas, você reconhece que todos são **a mesma estrutura algébrica**: uma órbita de `x` sob `f` até atingir `y`. Isso é o cerne do EoP — generalizar sobre **conceitos** (aqui, "transformação com órbita finita alcançável"), não sobre tipos concretos. Na prática de engenharia, isso vira uma ferramenta legítima: decidir quantas iterações um algoritmo numérico vai precisar antes mesmo de rodá-lo.

