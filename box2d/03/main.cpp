#include <Box2D/Box2D.h>
#include <iostream>

int main()
{
    const float32 TIME_STEP = 1.0f / 60.0f; // passo de tempo
    const int32 VELOCITY_ITERATIONS = 6; // número de iterações do solver de velocidade
    const int32 POSITION_ITERATIONS = 2; // número de iterações do solver de posição

    b2Vec2 gravity (0.0f, -9.81f); // vetor de gravidade
    b2World world (gravity); // mundo de física

    b2BodyDef body_def; // define o corpo do projétil
    body_def.type = b2_dynamicBody; // define o corpo como dinâmico (sujeito a forças)
    body_def.position.Set (0.0f, 0.0f); // define a posição inicial do corpo
    b2Body* body = world.CreateBody (&body_def); // cria o corpo no mundo de física

    b2CircleShape shape; // define a forma do projétil como um círculo
    shape.m_radius = 0.1f;

    b2FixtureDef fixture_def; // define a fixação do projétil
    fixture_def.shape = &shape;
    fixture_def.density = 1.0f;
    fixture_def.friction = 0.3f;
    fixture_def.restitution = 0.5f;
    body->CreateFixture (&fixture_def);

    b2Vec2 initial_velocity (10.0f, 20.0f); // define a velocidade inicial do projétil
    body->SetLinearVelocity (initial_velocity); // define a velocidade linear do corpo

    float32 time_elapsed = 0.0f; // tempo decorrido
    while (time_elapsed < 10.0f) // simula o lançamento por 10 segundos
    {
        world.Step (TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS); // atualiza a simulação

        b2Vec2 position = body->GetPosition(); // posição atual do projétil
        std::cout << "Posição: (" << position.x << ", " << position.y << ")" << std::endl;

        time_elapsed += TIME_STEP;
    }

    return 0;
}
