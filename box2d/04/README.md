Para implementar a simulação, utilizaremos a biblioteca Box2D para modelar a dinâmica de corpos rígidos. Box2D oferece física realista, mas precisaremos ajustar os parâmetros conforme necessário para simular a frenagem.

### Passos:

1. Criar um mundo Box2D.
2. Adicionar um corpo (representando o objeto) ao mundo.
3. Configurar o corpo com a velocidade inicial.
4. Aplicar uma força de frenagem constante ao corpo.
5. Simular a dinâmica até que o corpo pare, monitorando o tempo e a distância percorrida.

Segue um exemplo:

```cpp
#include <iostream>
#include <box2d/box2d.h>

// Função principal
int main() {
    // Velocidade inicial em metros por segundo (300 km/h)
    float initialVelocity = 300.0f * (1000.0f / 3600.0f);

    // Criação do mundo Box2D
    b2Vec2 gravity(0.0f, 0.0f); // Sem gravidade para simplificar
    b2World world(gravity);

    // Definição de um corpo dinâmico
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 0.0f);

    // Criar o corpo no mundo
    b2Body* body = world.CreateBody(&bodyDef);

    // Definir as propriedades do corpo (massa)
    b2PolygonShape box;
    box.SetAsBox(1.0f, 1.0f); // Uma caixa de tamanho arbitrário

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    // Aplicar a velocidade inicial ao corpo
    body->SetLinearVelocity(b2Vec2(initialVelocity, 0.0f));

    // Configurar a força de frenagem constante
    float brakeForce = 5.0f;
    float mass = body->GetMass();
    float deceleration = brakeForce / mass;

    // Simular a dinâmica até que a velocidade seja zero
    float timeStep = 1.0f / 60.0f;
    float elapsedTime = 0.0f;
    float totalDistance = 0.0f;

    while (body->GetLinearVelocity().x > 0.0f) {
        // Aplicar uma força negativa (de frenagem) na direção contrária
        body->ApplyForceToCenter(b2Vec2(-brakeForce, 0.0f), true);

        // Simular um passo no mundo Box2D
        world.Step(timeStep, 8, 3);

        // Atualizar o tempo decorrido e a distância percorrida
        elapsedTime += timeStep;
        totalDistance += body->GetLinearVelocity().x * timeStep;
    }

    // Mostrar o tempo total e a distância percorrida
    std::cout << "Tempo para parar: " << elapsedTime << " segundos\n";
    std::cout << "Distância percorrida: " << totalDistance << " metros\n";

    return 0;
}
```

**Notas:**

1. **Velocidade Inicial:** Converte a velocidade de 300 km/h para m/s.
2. **Box2D:** Configura o mundo Box2D com gravidade zero e simula um corpo simples.
3. **Frenagem:** Aplica uma força de frenagem constante no sentido oposto da velocidade.
4. **Simulação:** Simula até que a velocidade seja zero, calculando o tempo e a distância.

Certifique-se de que o Box2D está instalado e configurado corretamente no seu ambiente de desenvolvimento para compilar e rodar o programa.
