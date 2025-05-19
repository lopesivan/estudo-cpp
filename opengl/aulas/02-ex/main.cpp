#include <GL/glut.h>
// Um programa OpenGL/GLUT básico em C++
// #include <GL/freeglut.h> // ou <GL/glut.h>, dependendo da sua instalação

// ---------------------------------------------------------------------
// Funções auxiliares
// ---------------------------------------------------------------------

// Inicialização de estados do OpenGL
void init()
{
    // cor de fundo preta (RGBA)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Função de desenho (callback)
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // limpa o framebuffer

    // ... aqui vão as chamadas de desenho (ex.: glBegin()/glVertex()/glEnd())
    // ...

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
    glutInitWindowSize(400, 300);

    // Cria a janela e define seu título
    glutCreateWindow("Um programa OpenGL Exemplo");

    init(); // configura estados iniciais

    // Registra a função de desenho como callback
    glutDisplayFunc(display);

    // Entra no laço principal (nunca retorna até o programa terminar)
    glutMainLoop();

    return 0; // não será alcançado, mas mantém o padrão C++
}
