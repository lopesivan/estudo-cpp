A área de desenho que você definiu com `gluOrtho2D(0 , 200 , 0 , 150)` vai de (0, 0) até (200, 150) no plano XY.  Para “ver” esses limites basta traçar o retângulo-moldura dessa região e, em seguida, suas duas diagonais.
A única mudança necessária é acrescentar alguns vértices na função `display()`:

```cpp
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // ──────────────────────────────────────────
    // Moldura da “caixa” e suas diagonais
    // ──────────────────────────────────────────
    glColor3f(0.0f, 1.0f, 0.0f);      // verde p/ a moldura
    glBegin(GL_LINE_LOOP);            // percorre os 4 cantos
        glVertex2i(  0,   0);         // canto inferior-esquerdo
        glVertex2i(200,   0);         // canto inferior-direito
        glVertex2i(200, 150);         // canto superior-direito
        glVertex2i(  0, 150);         // canto superior-esquerdo
    glEnd();

    glBegin(GL_LINES);                // diagonais
        glVertex2i(  0,   0);         // ↗
        glVertex2i(200, 150);

        glVertex2i(  0, 150);         // ↘
        glVertex2i(200,   0);
    glEnd();

    // ──────────────────────────────────────────
    // Seu segmento original (vermelho)
    // ──────────────────────────────────────────
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2i(30, 100);
        glVertex2i(90,  20);
    glEnd();

    glFlush();
}
```

### O que isso faz

* **`GL_LINE_LOOP`** liga os quatro vértices em ordem, fechando a moldura retangular que corresponde exatamente aos limites do sistema de coordenadas ortogonais.
* Depois abrimos outro **`GL_LINES`** para as diagonais, unindo os cantos opostos.
* Usei cores diferentes para distinguir moldura (verde) das diagonais e do segmento original (vermelho).

### Manter proporção ao redimensionar a janela

Se você der `resize` na janela, o retângulo pode ficar “esticado”.  Para preservar a proporção 4:3 (200 × 150) registre um callback de **reshape**:

```cpp
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // ajusta a ortogonal mantendo aspecto
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    if (aspect >= (200.0f/150.0f))
        gluOrtho2D(0.0, 200.0*aspect/(200.0f/150.0f), 0.0, 150.0);
    else
        gluOrtho2D(0.0, 200.0, 0.0, 150.0*(200.0f/150.0f)/aspect);
}
...
glutReshapeFunc(reshape);
```

Não é obrigatório para ver a caixa, mas mantém tudo “quadradinho” se a janela for alongada.

Com isso você visualiza claramente a área de desenho de 0 ≤ x ≤ 200 e 0 ≤ y ≤ 150 — e qualquer objeto que ficar fora desse retângulo simplesmente não será mostrado.

Sim — o registro do **callback** `glutReshapeFunc` normalmente é feito no `main()`, junto com os demais callbacks (`glutDisplayFunc`, `glutKeyboardFunc`, etc.).  O ponto exato não é crítico, desde que seja **depois** de `glutCreateWindow` (quando a janela já existe) e **antes** de `glutMainLoop` (quando o loop de eventos começa).

```cpp
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200, 0);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Um programa OpenGL Exemplo");

    init();                       // estados iniciais

    glutDisplayFunc(display);     // redesenhar
    glutReshapeFunc(reshape);     // redimensionar ✨  ⟵  aqui
    /* se tiver teclado, mouse etc.
       glutKeyboardFunc(keyboard);
       glutMouseFunc(mouse);
    */

    glutMainLoop();               // loop infinito
    return 0;
}
```

### Por que no `main()`?

* **Ordem de criação:** `glutCreateWindow` deve vir antes para que haja um *contexto* OpenGL válido; só então você pode associar‐lhe callbacks.
* **Único ponto de configuração:** concentrar as chamadas de registro no `main()` torna fácil ver todas as “entradas” de evento do seu programa.

Depois disso, toda vez que o usuário redimensionar a janela, o GLUT chamará a função `reshape(int w, int h)` que você definiu, passando as novas largura `w` e altura `h`. Aí você ajusta `glViewport` e recompõe a matriz de projeção (com `gluOrtho2D`, `glFrustum`, `gluPerspective`, etc.) para manter o aspecto ou o enquadramento que desejar.
