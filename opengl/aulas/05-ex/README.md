Uma forma simples (e limpa) de “parametrizar” as dimensões
do **mundo** — isto é, o retângulo lógico que você quer
que apareça — é trocar todos os números mágicos (200 ×
150) por **constantes** ou **variáveis globais**. Assim você
muda um par de valores e o resto do programa se ajusta sozinho.

```cpp
// ────────────────────────────────────────────────────────────────
// Parâmetros do mundo (altere APENAS aqui)
// ────────────────────────────────────────────────────────────────
static const GLfloat WORLD_W = 300.0f;   // largura lógica
static const GLfloat WORLD_H = 200.0f;   // altura  lógica
```

Depois, use essas constantes em **três** pontos:

1. **Projeção inicial (`init`)**
2. **Cálculo de aspecto no `reshape`**
3. **Desenho da moldura**

```cpp
// -----------------------------------------------------------------
// Inicialização de estados do OpenGL
// -----------------------------------------------------------------
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WORLD_W, 0.0, WORLD_H);   // ← usa constantes!
}

// -----------------------------------------------------------------
// Callback de redimensionamento
// -----------------------------------------------------------------
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspectWin   = (GLfloat)w / (GLfloat)h;
    GLfloat aspectWorld = WORLD_W / WORLD_H;

    if (aspectWin >= aspectWorld) {      // janela mais “larga”
        GLfloat excess = aspectWin / aspectWorld;
        gluOrtho2D(0.0, WORLD_W * excess, 0.0, WORLD_H);
    } else {                             // janela mais “alta”
        GLfloat excess = aspectWorld / aspectWin;
        gluOrtho2D(0.0, WORLD_W, 0.0, WORLD_H * excess);
    }
}

// -----------------------------------------------------------------
// Callback de desenho
// -----------------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // ---------------- Moldura do mundo ----------------
    glColor3f(0.0f, 1.0f, 0.0f);           // verde
    glBegin(GL_LINE_LOOP);
        glVertex2f(0,        0);
        glVertex2f(WORLD_W,  0);
        glVertex2f(WORLD_W,  WORLD_H);
        glVertex2f(0,        WORLD_H);
    glEnd();

    // ---------------- Diagonais -----------------------
    glBegin(GL_LINES);
        glVertex2f(0,        0);
        glVertex2f(WORLD_W,  WORLD_H);

        glVertex2f(0,        WORLD_H);
        glVertex2f(WORLD_W,  0);
    glEnd();

    // -------------- Segmento de exemplo ---------------
    glColor3f(1.0f, 0.0f, 0.0f);           // vermelho
    glBegin(GL_LINES);
        glVertex2f(0.15f*WORLD_W, 0.70f*WORLD_H);   // 30 ,100 → percentuais
        glVertex2f(0.45f*WORLD_W, 0.10f*WORLD_H);   // 90 , 20
    glEnd();

    glFlush();
}
```

### Como funciona

* **Mude só `WORLD_W` e `WORLD_H`.**  Quer 500 × 500,
ou 640 × 480?  Troque aqui.
* **`reshape()`** compara a razão largura/altura da janela
(`aspectWin`) com a razão do mundo (`aspectWorld`) para
decidir se “estica” horizontes ou verticais, mantendo a
proporção correta — nada fica achatado.
* Nos vértices do segmento vermelho usei **frações da
largura/altura** (0.15 × `WORLD_W`, etc.).  Assim seus objetos
também “escalam” quando você altera o tamanho lógico;
se preferir números absolutos, mantenha os literais.

Com isso, basta alterar as duas constantes no topo do arquivo,
recompilar e seu programa exibirá um novo “mundo” 2-D
preservando sempre a proporção correta, independentemente
do tamanho da janela na tela.

A função `main()` muda pouquíssimo.
A única coisa que vale a pena “parametrizar” também é o **tamanho inicial da janela**, para que siga a mesma razão (*aspect ratio*) do “mundo” (`WORLD_W : WORLD_H`).  Basta calcular a altura a partir da largura (ou vice-versa) usando as mesmíssimas constantes.

```cpp
// -----------------------------------------------------------------
// main()
// -----------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // posição inicial da janela
    glutInitWindowPosition(200, 0);

    // tamanho inicial — escolha uma LARGURA base qualquer
    const int WIN_BASE_W = 600;                     // pixels
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
```

### O que foi feito

* **`WIN_BASE_W`** decide “quão grande” (em pixels) será
a janela inicial; troque esse número para 300, 800, 1024…
* **`WIN_BASE_H`** é calculado para preservar a razão
`WORLD_H / WORLD_W`.

  ```cpp
  WIN_BASE_H = WIN_BASE_W * (WORLD_H / WORLD_W);
  ```

  Assim, se você mudar o “mundo” lá em cima para 500
  × 500 ou 640 × 480, o retângulo continuará perfeito e
  sem distorção já na primeira abertura da janela.

Fora isso, nada mais muda — todo o controle de proporção
durante *resize* é feito pelo callback `reshape()` que você
já registrou.
