Perfeito. Para esta etapa, basta trocar no C++:

```cpp
ng_command("source RC.sp");
```

por:

```cpp
ng_command("source RLC.sp");
```

O restante do código continua funcionando exatamente da mesma forma, porque os nós e componentes que ele usa continuam sendo:

```text
in
out
v1#branch
```

Então essa parte fica:

```cpp
ng_command("source RLC.sp");
ng_command("run");

const auto time =
    get_vector("time");

const auto vin =
    get_vector("in");

const auto vout =
    get_vector("out");

const auto v1_branch =
    get_vector("v1#branch");
```

Também vale corrigir o comentário inicial do arquivo, porque atualmente está:

```spice
* Circuito RC com fonte senoidal
```

mas o arquivo chama `RLC.sp`.

Por enquanto, porém, o circuito ainda é de fato **RC**:

```spice
R1 in out 1k
C1 out 0 1u
```

Não existe indutor ainda.

Se a intenção agora é transformar efetivamente o circuito em um **RLC série**, eu sugiro a próxima topologia:

```text
         R1         L1
 in ───/\/\/──────LLLL────── out
                               │
                               C1
                               │
                              GND
```

por exemplo:

```spice
V1 in 0 SIN(0 {A} {f})

R1 in n1 1k
L1 n1 out 100m
C1 out 0 1u
```

Aí o nosso C++ também pode passar a obter e calcular separadamente:

```text
V_R
V_L
V_C
I
P_R
P_L
P_C
P_fonte
```

e depois potência ativa, reativa e aparente de cada componente.

