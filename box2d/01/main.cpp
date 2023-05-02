#include <Box2D/Box2D.h>
#include <iostream>

int main()
{
    b2Vec2 gravity (0.0f, -9.81f); // vetor de gravidade
    b2World world (gravity); // mundo de física

    // define o corpo A (corpo em repouso)
    b2BodyDef body_def_a;
    body_def_a.type = b2_dynamicBody; // define o corpo como dinâmico (sujeito a forças)
    body_def_a.position.Set (0.0f, 0.0f); // define a posição inicial do corpo
    b2Body* body_a = world.CreateBody (&body_def_a); // cria o corpo no mundo de física

    b2CircleShape shape_a; // define a forma do corpo A como um círculo
    shape_a.m_radius = 1.0f;

    b2FixtureDef fixture_def_a; // define a fixação do corpo A
    fixture_def_a.shape = &shape_a;
    fixture_def_a.density = 1.0f;
    fixture_def_a.friction = 0.3f;
    fixture_def_a.restitution = 0.5f; // define o coeficiente de restituição em 0.5 (colisão elástica)
    body_a->CreateFixture (&fixture_def_a);

    // define o corpo B (corpo em movimento)
    b2BodyDef body_def_b;
    body_def_b.type = b2_dynamicBody; // define o corpo como dinâmico (sujeito a forças)
    body_def_b.position.Set (10.0f, 0.0f); // define a posição inicial do corpo
    b2Body* body_b = world.CreateBody (&body_def_b); // cria o corpo no mundo de física

    b2CircleShape shape_b; // define a forma do corpo B como um círculo
    shape_b.m_radius = 1.0f;

    b2FixtureDef fixture_def_b; // define a fixação do corpo B
    fixture_def_b.shape = &shape_b;
    fixture_def_b.density = 1.0f;
    fixture_def_b.friction = 0.3f;
    fixture_def_b.restitution = 0.5f; // define o coeficiente de restituição em 0.5 (colisão elástica)
    body_b->CreateFixture (&fixture_def_b);

    b2Vec2 initial_velocity (10.0f, 0.0f); // define a velocidade inicial do corpo B
    body_b->SetLinearVelocity (initial_velocity); // define a velocidade linear do corpo B

    float32 time_elapsed = 0.0f; // tempo decorrido
    const float32 TIME_STEP = 1.0f / 60.0f; // passo de tempo
    const int32 VELOCITY_ITERATIONS = 6; // número de iterações do solver de velocidade
    const int32 POSITION_ITERATIONS = 2; // número de iterações do solver de posição
    while (time_elapsed < 5.0f) // simula a colisão por 5 segundos
    {
        world.Step (TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS); // atualiza a simulação

        b2Vec2 position_a = body_a->GetPosition(); // posição atual do corpo A
        b2Vec2 position_b = body_b->GetPosition(); // posição atual do corpo B
        std::cout << "Posição A: (" << position_a.x << ", " << position_a.y << ")" << std::endl;
        std::cout << "Posição B: (" << position_b.x << ", " << position_b.y << ")" << std::endl;

        time_elapsed += TIME_STEP;
    }

    return 0;
}
