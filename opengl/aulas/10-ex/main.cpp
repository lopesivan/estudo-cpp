// Um programa OpenGL/GLUT básico em C++
#include <GL/freeglut.h> // ou <GL/glut.h>, dependendo da sua instalação
#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdio>
#include <cmath>

static const GLfloat OFF_SET = 0.5f;
// ────────────────────────────────────────────────────────────────
// Parâmetros do mundo (altere APENAS aqui)
// ────────────────────────────────────────────────────────────────
static const GLfloat WORLD_W = 500.0f; // largura lógica
static const GLfloat WORLD_H = 500.0f; // altura  lógica
// ---------------------------------------------------------------------
// Funções auxiliares
// ---------------------------------------------------------------------

// Inicialização de estados do OpenGL
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);

    // projeção ortogonal 2D
    gluOrtho2D(0, WORLD_W, 0, WORLD_H); // ← usa constantes!
    //         Xmin,       Xmax,            Ymin,     Ymax

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(OFF_SET, 0.0f, 0.0f);   // <— empurra tudo ½ pixel
}
// redimensiona
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspectWin = (GLfloat)w / (GLfloat)h;
    GLfloat aspectWorld = WORLD_W / WORLD_H;

    // ajusta a ortogonal mantendo aspecto
    if (aspectWin >= aspectWorld)
    { // janela mais “larga”
        GLfloat excess = aspectWin / aspectWorld;
        gluOrtho2D(0.0, WORLD_W * excess, 0.0, WORLD_H);
    }
    else
    { // janela mais “alta”
        GLfloat excess = aspectWorld / aspectWin;
        gluOrtho2D(0.0, WORLD_W, 0.0, WORLD_H * excess);
    }
}

// Função de desenho (callback)
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //
    // v3 ●──────────● v2       sequência enviada:
    //    │          │           v0 → v1 → v2 → v3 → (fecha) v0
    //    │          │
    // v0 ●──────────● v1
    //
    // ---------------- Moldura do mundo ----------------
    // contorno
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);       // v0
        glVertex2f(WORLD_W,   0);       // v1
        glVertex2f(WORLD_W,   WORLD_H); // v2
        glVertex2f(0, WORLD_H); // v3
    glEnd();


    // ---------------- Diagonais -----------------------
    glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(WORLD_W, WORLD_H);

        glVertex2f(0, WORLD_H);
        glVertex2f(WORLD_W, 0);
    glEnd();


    // -----------------------------------------------------------------
    // Circulo de centro (c_x , c_y) e raio r
    // -----------------------------------------------------------------
    GLfloat c_x = WORLD_W/2;   // centro X  (metade de WORLD_W)
    GLfloat c_y = WORLD_H/2;   // centro Y  (metade de WORLD_H)

    const int SEGMENTS = 300;          // resolução (quanto >, mais liso)

    int NUM_CIRCLES = 90;
    GLfloat R_MAX   = WORLD_H/2; // raio em unidades do mundo
    GLfloat r       = 0;         // raio em unidades do mundo

    glColor3f(1.0f, 1.0f, 1.0f);      // branco

    for (int k = 1; k <= NUM_CIRCLES; ++k)
    {
        r = R_MAX * k / NUM_CIRCLES;   // raio cresce em passos iguais

        glBegin(GL_LINE_LOOP);
            for (int i = 0; i < SEGMENTS; ++i) {
                float theta = 2.0f * M_PI * i / SEGMENTS;   // ângulo em radianos
                glVertex2f(c_x + r * cosf(theta),
                        c_y + r * sinf(theta));
            }
        glEnd();
    }


    // -----------------------------------------------------------------
    // Circulo de centro (c_x , c_y) e raio r
    // -----------------------------------------------------------------
    double diagonal = std::hypot(static_cast<double>(WORLD_W),
                           static_cast<double>(WORLD_H));
    r   =  diagonal/2;   // raio em unidades do mundo

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)
        {
            float rad = i * M_PI / 180.0;
            glVertex2f(r*cos(rad), r*sin(rad));
        }
    glEnd();

    glFlush();
}

// ---------------------------------------------------------------------
// Função principal
// ---------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    int screenW = glutGet(GLUT_SCREEN_WIDTH);   // pixels do monitor
    int screenH = glutGet(GLUT_SCREEN_HEIGHT);

    int windowW, windowH;

    windowH = static_cast<int>(screenH);
    windowW = static_cast<int>(screenW);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowW, windowH);
    glutInitWindowPosition(0,0);

    glutCreateWindow("Um programa OpenGL Exemplo");
    std::printf("Tela: %d × %d px\n", screenW, screenH);

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;   // nunca chega aqui, mas mantém padrão C++
}

