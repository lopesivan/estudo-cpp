// Um programa OpenGL/GLUT básico em C++
#include <GL/freeglut.h> // ou <GL/glut.h>, dependendo da sua instalação
#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdio>
#include <cmath>

// ────────────────────────────────────────────────────────────────
// Parâmetros do mundo (altere APENAS aqui)
// ────────────────────────────────────────────────────────────────
static const GLfloat WORLD_W = 1920.0f; // largura lógica
static const GLfloat WORLD_H = 1080.0f; // altura  lógica
// ---------------------------------------------------------------------
// Funções auxiliares
// ---------------------------------------------------------------------

// Inicialização de estados do OpenGL
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);

    // projeção ortogonal 2D
    gluOrtho2D(0.0, WORLD_W, 0.0, WORLD_H); // ← usa constantes!
    //                Xmin,       Xmax,            Ymin,     Ymax
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

    // ---------------- Moldura do mundo ----------------
    glColor3f(0.0f, 1.0f, 0.0f); // verde
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(WORLD_W, 0);
        glVertex2f(WORLD_W, WORLD_H);
        glVertex2f(0, WORLD_H);
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
    GLfloat r   = WORLD_H/2;   // raio em unidades do mundo

    const int SEGMENTS = 300;          // resolução (quanto >, mais liso)

    glColor3f(1.0f, 1.0f, 1.0f);      // branco
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < SEGMENTS; ++i) {
            float theta = 2.0f * M_PI * i / SEGMENTS;   // ângulo em radianos
            glVertex2f(c_x + r * cosf(theta),
                    c_y + r * sinf(theta));
        }
    glEnd();


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

    std::printf("Tela: %d × %d px\n", screenW, screenH);

    GLfloat aspectWorld = WORLD_W / WORLD_H;           // 1.333…
    GLfloat aspectScr   = (GLfloat)screenW / screenH;        // 1.777…


    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    int windowW, windowH;

    // ocupa 90 % da dimensão “limitante” da tela
    const GLfloat FILL = 0.90f;                        // 90 %

    if (aspectScr >= aspectWorld) {                        // tela mais “larga” que o mundo
        windowH = static_cast<int>(screenH * FILL);        // 0.90·1080 ≈ 972
        windowW = static_cast<int>(windowH * aspectWorld); // 972·4/3  ≈ 1296
    } else {                                               // (não é o caso aqui, mas fica genérico)
        windowW = static_cast<int>(screenW * FILL);
        windowH = static_cast<int>(windowW / aspectWorld);
    }

    // centraliza
    glutInitWindowSize(windowW, windowH);
    glutInitWindowPosition((screenW - windowW)/2, (screenH - windowH)/2);

    // posição inicial da janela
    glutInitWindowPosition(200, 0);

    glutCreateWindow("Um programa OpenGL Exemplo");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;   // nunca chega aqui, mas mantém padrão C++
}

