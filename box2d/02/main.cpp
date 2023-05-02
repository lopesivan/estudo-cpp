#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

int main()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SDL_Init (SDL_INIT_VIDEO); // inicializa a SDL

    // cria a janela e o renderer
    SDL_Window* window = SDL_CreateWindow ("Box2D + SDL", SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer (window, -1,
                             SDL_RENDERER_ACCELERATED);

    // cria o mundo de física
    b2Vec2 gravity (0.0f, 9.81f);
    b2World world (gravity);

    // cria o corpo do chão
    b2BodyDef ground_def;
    ground_def.position.Set (0.0f, SCREEN_HEIGHT);
    b2Body* ground = world.CreateBody (&ground_def);

    // define a forma do chão como um retângulo
    b2PolygonShape ground_shape;
    ground_shape.SetAsBox (SCREEN_WIDTH, 10.0f);

    // adiciona a fixação do chão ao corpo do chão
    b2FixtureDef ground_fixture_def;
    ground_fixture_def.shape = &ground_shape;
    ground_fixture_def.friction = 0.3f;
    ground_fixture_def.restitution = 0.5f;
    ground->CreateFixture (&ground_fixture_def);

    // cria o corpo da bola
    b2BodyDef ball_def;
    ball_def.type = b2_dynamicBody;
    ball_def.position.Set (SCREEN_WIDTH / 2.0f, 0.0f);
    b2Body* ball = world.CreateBody (&ball_def);

    // define a forma da bola como um círculo
    b2CircleShape ball_shape;
    ball_shape.m_radius = 20.0f;

    // adiciona a fixação da bola ao corpo da bola
    b2FixtureDef ball_fixture_def;
    ball_fixture_def.shape = &ball_shape;
    ball_fixture_def.density = 1.0f;
    ball_fixture_def.friction = 0.3f;
    ball_fixture_def.restitution = 0.5f;
    ball->CreateFixture (&ball_fixture_def);

    // inicia o loop de jogo
    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        // processa os eventos
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
                quit = true;
        }

        // atualiza o mundo de física
        world.Step (1.0f / 60.0f, 6, 2);

        // limpa a tela
        SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
        SDL_RenderClear (renderer);

        // desenha o chão
        SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
        b2Vec2 ground_position = ground->GetPosition();
        b2Vec2 ground_size = ground_shape.GetVertex (2);
        SDL_Rect ground_rect = { (int) (ground_position.x - ground_size.x), (int) (SCREEN_HEIGHT - ground_position.y - ground_size.y), (int) (ground_size.x * 2), (int) (ground_size.y * 2)};
        SDL_RenderFillRect (renderer, &ground_rect);

        // desenha a bola
        SDL_SetRenderDrawColor (renderer, 255, 0, 0, 255);
        b2Vec2 ball_position = ball->GetPosition();
        float ball_radius = ball_shape.m_radius;
        SDL_Rect ball_rect =
        {
            (int) (ball_position.x - ball_radius),
            (int) (SCREEN_HEIGHT - ball_position.y - ball_radius),
            (int) (ball_radius * 2),
            (int) (ball_radius * 2)
        };
        SDL_RenderFillRect (renderer, &ball_rect);

        // apresenta a tela
        SDL_RenderPresent (renderer);
    }

    // libera a memória
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    SDL_Quit();

    return 0;

}
