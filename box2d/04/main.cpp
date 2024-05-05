#include <Box2D/Box2D.h>
#include <iostream>

// Função principal
int main() {
  // Velocidade inicial em metros por segundo (300 km/h)
  float initialVelocity = 300.0f / 3.6;

  // Criação do mundo Box2D
  b2Vec2 gravity(0.0f, 0.0f); // Sem gravidade para simplificar
  b2World world(gravity);

  // Definição de um corpo dinâmico
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 0.0f);

  // Criar o corpo no mundo
  b2Body *body = world.CreateBody(&bodyDef);

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
