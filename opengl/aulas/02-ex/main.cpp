// Um programa OpenGL/GLUT básico em C++
#include <GL/freeglut.h> // ou <GL/glut.h>, dependendo da sua instalação
#include <GL/gl.h>
#include <GL/glu.h>

// ---------------------------------------------------------------------
// Funções auxiliares
// ---------------------------------------------------------------------

// Inicialização de estados do OpenGL
void init()
{
    // cor de fundo preta (RGBA)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // carrega a matriz de projeção
    glMatrixMode(GL_PROJECTION);

    //
    // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓   ---+
    // ┃                                          ┃      |
    // ┃                                          ┃      |
    // ┃ Ymax ∆──────────────────────────┐ ┄┄+    ┃      |
    // ┃      │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │   ╎    ┃      |
    // ┃      │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │   ╎    ┃      |
    // ┃      │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │   ╎    ┃      |  300
    // ┃      │  ▓▓▓▓▓▓200 x 150▓▓▓▓▓▓▓▓ │   ╎ 150┃      |
    // ┃      │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │   ╎    ┃      |
    // ┃      │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │   ╎    ┃      |
    // ┃      │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │   ╎    ┃      |
    // ┃ Ymin └──────────────────────────> ┄┄+    ┃      |
    // ┃      |------------- 200 --------|        ┃      |
    // ┃      Xmin                       Xmax     ┃      |
    // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛   ---+
    // |----------------------- 400 --------------|
    //
    // projeção ortogonal 2D
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
    //               Xmin,       Xmax,          Ymin,     Ymax
}

// Função de desenho (callback)
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // limpa o framebuffer

    // ... aqui vão as chamadas de desenho (ex.: glBegin()/glVertex()/glEnd())

    glColor3f(1.0, 0.0, 0.0); // altera atributo de cor para vermelho
    glBegin(GL_LINES);
    {
        //
        //  200  ∆
        //       │   + P1 (30,100)
        //       │    \
        //       │     \
        //       │      \
        //       │       \
        //       │        + P2 (90,20)
        //       │
        //       └──────────────────────────>
        //                                  150
        glVertex2i(30, 100);
        glVertex2i(90, 20);
    }
    glEnd();

    glFlush(); // força renderização imediata (porque usamos GLUT_SINGLE)
}

// ---------------------------------------------------------------------
// Função principal
// ---------------------------------------------------------------------
int main(int argc, char** argv)
{
    // Inicializa o GLUT e trata parâmetros da linha de comando
    glutInit(&argc, argv);

    // Modo de exibição: framebuffer simples + modelo de cor RGB
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Posição inicial da janela (x, y)
    glutInitWindowPosition(200, 0);

    // Tamanho inicial da janela (largura, altura)
    //
    // +-------------------+ -+
    // |                   |  |
    // |                   |  | 300
    // |                   |  |
    // |                   |  |
    // +-------------------+ -+
    // |                   |
    // |------ 400 --------|
    //
    glutInitWindowSize(400, 300); // (largura, altura) 4/3 -> (4 por 3)

    // Cria a janela e define seu título
    glutCreateWindow("Um programa OpenGL Exemplo");

    init(); // configura estados iniciais

    // Registra a função de desenho como callback
    glutDisplayFunc(display);

    // Entra no laço principal (nunca retorna até o programa terminar)
    glutMainLoop();

    return 0; // não será alcançado, mas mantém o padrão C++
}
