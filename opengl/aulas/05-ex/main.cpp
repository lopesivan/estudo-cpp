// Um programa OpenGL/GLUT básico em C++
#include <GL/freeglut.h> // ou <GL/glut.h>, dependendo da sua instalação
#include <GL/gl.h>
#include <GL/glu.h>

// ────────────────────────────────────────────────────────────────
// Parâmetros do mundo (altere APENAS aqui)
// ────────────────────────────────────────────────────────────────
static const GLfloat WORLD_W = 300.0f; // largura lógica
static const GLfloat WORLD_H = 200.0f; // altura  lógica

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

    // ──────────────────────────────────────────
    // Seu segmento original (vermelho)
    // ──────────────────────────────────────────
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2i(30, 100);
    glVertex2i(90, 20);
    glEnd();

    glFlush();
}
// ---------------------------------------------------------------------
// Função principal
// ---------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // posição inicial da janela
    glutInitWindowPosition(200, 0);

    // tamanho inicial — escolha uma LARGURA base qualquer
    // **`WIN_BASE_W`** decide “quão grande” (em pixels) será
    // a janela inicial; troque esse número para 300, 800, 1024…
    const int WIN_BASE_W = 1024;                    // pixels
    const int WIN_BASE_H = static_cast<int>(        // mantém proporção
                            WIN_BASE_W * (WORLD_H / WORLD_W));

    glutInitWindowSize(WIN_BASE_W, WIN_BASE_H);

    glutCreateWindow("Um programa OpenGL Exemplo");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;   // nunca chega aqui, mas mantém padrão C++
}
