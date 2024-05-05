#include <Box2D/Box2D.h>
#include <iostream>

// Função principal
int main() {
  // Velocidade inicial em m/s (90 km/h)
  float initialVelocity = 90.0f * (1000.0f / 3600.0f);

  // Configurações de frenagem
  float coefficientFriction = 0.8f;
  float gravity = 9.8f; // m/s^2

  // Criação do mundo Box2D
  b2Vec2 gravityVec(0.0f, 0.0f); // Gravidade zero (horizontal)
  b2World world(gravityVec);

  // Definição de um corpo dinâmico representando o carro
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 0.0f);

  // Criar o corpo no mundo
  b2Body *car = world.CreateBody(&bodyDef);

  // Configuração da forma do carro e sua densidade
  b2PolygonShape carShape;
  carShape.SetAsBox(2.0f, 1.0f); // Tamanho arbitrário para simplificação
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &carShape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = coefficientFriction;
  car->CreateFixture(&fixtureDef);

  // Aplicar velocidade inicial ao carro
  car->SetLinearVelocity(b2Vec2(initialVelocity, 0.0f));

  // Simular o tempo de reação de 1 segundo
  float reactionTime = 1.0f;
  float timeStep = 1.0f / 60.0f; // 60 fps
  int reactionSteps = static_cast<int>(reactionTime / timeStep);

  float elapsedTime = 0.0f;
  float distanceTraveledDuringReaction = initialVelocity * reactionTime;

  // Simular tempo de reação
  for (int i = 0; i < reactionSteps; ++i) {
    world.Step(timeStep, 8, 3);
    elapsedTime += timeStep;
  }

  // Calcular a força de frenagem necessária
  float carMass = car->GetMass();
  float brakingForce = carMass * coefficientFriction * gravity;
  float deceleration = brakingForce / carMass;

  // Simular a frenagem até parar completamente
  float distanceTraveledWhileBraking = 0.0f;
  while (car->GetLinearVelocity().x > 0.0f) {
    car->ApplyForceToCenter(b2Vec2(-brakingForce, 0.0f), true);
    world.Step(timeStep, 8, 3);
    distanceTraveledWhileBraking += car->GetLinearVelocity().x * timeStep;
    elapsedTime += timeStep;
  }

  // Calcular a distância total percorrida
  float totalDistanceTraveled =
      distanceTraveledDuringReaction + distanceTraveledWhileBraking;

  // Mostrar os resultados
  std::cout << "Tempo total para parar: " << elapsedTime << " segundos\n";
  std::cout << "Distância total percorrida: " << totalDistanceTraveled
            << " metros\n";

  return 0;
}
