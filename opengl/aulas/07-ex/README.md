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




Está **correto do jeito que estava**—o fator precisa ser `WORLD_H / WORLD_W`.

### Por quê?

Queremos que a janela inicial mantenha **a mesma razão de aspecto** do “mundo” lógico:

$$
\frac{\text{WIN\_BASE\_W}}{\text{WIN\_BASE\_H}}
\;=\;
\frac{\text{WORLD\_W}}{\text{WORLD\_H}}
$$

Isolando `WIN_BASE_H`:

$$
\text{WIN\_BASE\_H}
\;=\;
\text{WIN\_BASE\_W}\;\times\;\frac{\text{WORLD\_H}}{\text{WORLD\_W}}
$$

Portanto a expressão original

```cpp
const int WIN_BASE_H = static_cast<int>(WIN_BASE_W * (WORLD_H / WORLD_W));
```

está correta.

---

#### Verificando com números

* `WORLD_W = 200`
* `WORLD_H = 150` → razão 4 : 3 (≈ 1,333…)

Se escolher `WIN_BASE_W = 400 px`:

```cpp
WIN_BASE_H = 400 * 150 / 200 = 400 * 0.75 = 300 px
```

A janela fica 400 × 300 px → 4 : 3, exatamente como o mundo.

Se trocasse o fator (`WORLD_W / WORLD_H`):

```cpp
WIN_BASE_H = 400 * 200 / 150 ≈ 533 px
```

A razão ficaria 400 : 533 ≈ 0,75 — **invertida** (quase 3 : 4), distorcendo a cena.

---

**Resumindo:** mantenha `WIN_BASE_W * (WORLD_H / WORLD_W)` para preservar o aspecto.




O esboço é válido, mas do jeito que está você desenha **um círculo de raio 1** centrado na origem - isto é, só um pontinho no canto inferior-esquerdo do retângulo (0, 0) → (200, 150).
Para escolher **centro** $(c_x, c_y)$ e **raio** $r$ (em coordenadas do seu “mundo” lógico) basta escalar e transladar os vértices:

```cpp
// -----------------------------------------------------------------
// Circulo de centro (c_x , c_y) e raio r
// -----------------------------------------------------------------
GLfloat c_x = 100.0f;   // centro X  (metade de WORLD_W)
GLfloat c_y =  75.0f;   // centro Y  (metade de WORLD_H)
GLfloat r   =  40.0f;   // raio em unidades do mundo

const int SEGMENTS = 72;          // resolução (quanto >, mais liso)

glColor3f(1.0f, 1.0f, 1.0f);      // branco
glBegin(GL_LINE_LOOP);
    for (int i = 0; i < SEGMENTS; ++i) {
        float theta = 2.0f * M_PI * i / SEGMENTS;   // ângulo em radianos
        glVertex2f(c_x + r * cosf(theta),
                   c_y + r * sinf(theta));
    }
glEnd();
```

## Pontos-chave

| O que mudar       | Por quê                                                                                                                            |
| ----------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| `cosf()/sinf()`   | Versões `float` (evita casting implícito).                                                                                         |
| `SEGMENTS`        | 360 é exagero; 60–100 já fica suave.                                                                                               |
| `r`, `c_x`, `c_y` | Escalam/posicionam o círculo dentro de 200 × 150.                                                                                  |
| `M_PI`            | Inclua `<cmath>` **antes** e defina `#define _USE_MATH_DEFINES` em MSVC se necessário, ou declare `const float PI = 3.14159265f;`. |

Resultado → um círculo de 40 unid de raio, centralizado na tela, preservando o aspecto mesmo que a janela seja redimensionada.





A lógica está certa, mas **o seu círculo está com raio 1 (unidade lógica)** — quase imperceptível no “mundo” de 200 × 150. Basta multiplicar pelas dimensões desejadas (raio `r`) e somar o centro (`cx`, `cy`):

```cpp
// -------------------- Círculo --------------------
void drawCircle(GLfloat cx, GLfloat cy,
                GLfloat r, int segments = 60)
{
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float theta = 2.0f * M_PI * i / segments;   // ângulo
            glVertex2f(cx + r * cosf(theta),
                       cy + r * sinf(theta));
        }
    glEnd();
}
```

Use dentro do `display()` assim:

```cpp
glColor3f(1.0f, 1.0f, 1.0f);         // branco
drawCircle(WORLD_W * 0.5f,           // centro X  (meio da tela)
           WORLD_H * 0.5f,           // centro Y
           30.0f,                    // raio em unidades do mundo
           100);                     // segmentos (suavidade)
```

### O que mudou

| Antes                   | Depois                                       |
| ----------------------- | -------------------------------------------- |
| `cos(rad), sin(rad)`    | `cx + r · cos(θ), cy + r · sin(θ)`           |
| Raio fixo = 1           | Raio arbitrário `r`                          |
| Centro implícito (0, 0) | Centro parametrizado `(cx, cy)`              |
| 360 vértices            | `segments` ajustável (≈ 60–100 já fica liso) |

Assim o círculo aparece no tamanho esperado, centralizado (ou onde você quiser) e sem distorções quando a janela for redimensionada. Se quiser preenchê-lo, troque `GL_LINE_LOOP` por `GL_TRIANGLE_FAN` (começando por `glVertex2f(cx, cy)` para o ponto central).



“**Norma**” pode significar duas coisas diferentes no universo OpenGL / GLSL:

| Palavra em PT             | Termo técnico      | Para que serve                                         | Como se usa                                                                                  |
| ------------------------- | ------------------ | ------------------------------------------------------ | -------------------------------------------------------------------------------------------- |
| **norma** (comprimento)   | **norm / length**  | medir o tamanho de um vetor                            | função `length(v)` no GLSL                                                                   |
| **normal** (vetor normal) | **surface normal** | definir a orientação de uma superfície para iluminação | função fixa `glNormal*()` (OpenGL antigo) ou atributo/`in` num VAO + `normalize()` no shader |

---

## 1  Norma (comprimento de vetor)

No **GLSL** (shaders) existe a função built-in

```glsl
float len = length(vec3( x, y, z ));
```

* Retorna √(x²+y²+z²).
* Para normalizar basta `normalize(v)` — divide o vetor pelo próprio comprimento.

Em **C/C++ do lado da CPU** não há função pronta; você calcula “na mão”:

```cpp
float length3(const glm::vec3& v) {          // se estiver usando GLM
    return glm::length(v);
}
// ou, sem GLM
float length3(float x, float y, float z) {
    return std::sqrt(x*x + y*y + z*z);
}
```

---

## 2  Vetor normal (iluminação)

### Pipeline **antigo** (immediate mode)

```cpp
glBegin(GL_TRIANGLES);
    glNormal3f(nx, ny, nz);   // normal do próximo vértice
    glVertex3f(...);
    ...
glEnd();
```

* Você fornece uma **normal por vértice** (ou por face, repetindo-a em todos os vértices daquela face).
* O OpenGL fixa interpola e usa nas contas de iluminação (*Gouraud shading*).

### Pipeline **moderno** (OpenGL 3.3+)

1. Coloque as normais num **VBO** junto dos vértices.

2. No **vertex shader** receba-as como atributo:

   ```glsl
   layout(location = 0) in vec3 aPos;
   layout(location = 1) in vec3 aNormal;

   out vec3 vNormal;          // passa p/ fragment shader já no espaço correto

   void main()
   {
       vNormal = mat3(normalMatrix) * aNormal;   // normalMatrix = inverseTranspose(model)
       gl_Position = MVP * vec4(aPos, 1.0);
   }
   ```

3. No **fragment shader** normalize e faça a iluminação:

   ```glsl
   in vec3  vNormal;
   out vec4 fragColor;

   void main()
   {
       vec3 N = normalize(vNormal);
       vec3 L = normalize(lightDir);
       float diff = max(dot(N, L), 0.0);
       fragColor = vec4(diff * lightColor, 1.0);
   }
   ```

---

### Resumo rápido

* **Quer só o comprimento?** → `length()` (GLSL) ou `sqrt(x²+…)` em C/C++.
* **Vai fazer iluminação?** → use **normais**, não “normas”.

  * Immediate mode: `glNormal3f`.
  * Moderno: atributo + `normalize()` no shader.

Se precisar de um exemplo completo (VBO/VAO, shaders, cálculo de normais de uma malha, etc.), é só dizer!



Depende exatamente do que você quer dizer com “fazer isso” — calcular a **norma** (comprimento) de um vetor ou **normalizar** um vetor que já representa a **normal** de uma superfície:

| Tarefa                                                                                                        | Existe “função do OpenGL”?                                                                                   | Onde / como usar                                           |
| ------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------- |
| **Comprimento de vetor** (‖v‖)                                                                                | **Não** no lado da CPU.  Você mesmo calcula em C/C++ ou usa uma biblioteca de matemática (GLM, Eigen, etc.). | `float len = sqrtf(x*x + y*y + z*z);` ou `glm::length(v);` |
| **Normalizar um vetor genérico**                                                                              | **Não** (CPU).  Faça você mesmo ou `glm::normalize(v)`. No shader há `normalize()` (GLSL).                   | —                                                          |
| **Garantir que as *normais* enviadas para a pipeline fiquem unitárias depois de uma transformação de escala** | **Sim** → `glEnable(GL_NORMALIZE)` (ou `GL_RESCALE_NORMAL`).  Funciona no pipeline *fixed-function*.         | `cpp glEnable(GL_NORMALIZE); // antes de desenhar`         |
| **Obter o comprimento ou normalizar dentro do shader**                                                        | **Sim** → funções **GLSL** : `length(v)`, `normalize(v)`, `dot()`, etc.                                      | `glsl float d = length(vec3); vec3 n = normalize(vec3);`   |

### Detalhes rápidos

1. **CPU (seu código C/C++)**
   A API OpenGL “clássica” nunca teve rotinas de álgebra vetorial; ela só recebe vértices/primitivas.
   *Use GLM* (OpenGL Mathematics) se não quiser escrever contas à mão:

   ```cpp
   #include <glm/glm.hpp>
   #include <glm/gtc/type_ptr.hpp>

   glm::vec3 v(1.0f, 2.0f, 3.0f);
   float len   = glm::length(v);     // norma
   glm::vec3 n = glm::normalize(v);  // vetor unitário
   ```

2. **Shaders (GLSL)**
   Dentro do `vertex` ou `fragment shader` você tem várias funções built-in:

   ```glsl
   vec3 v    = vec3(1.0, 2.0, 3.0);
   float len = length(v);
   vec3  n   = normalize(v);
   ```

3. **Normals na pipeline fixa**
   Se você ainda usa `glBegin/glEnd` + iluminação de vértices (`glLight*`, `glMaterial*`):

   ```cpp
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glEnable(GL_NORMALIZE);   // força OpenGL a renormalizar
   ```

   Útil quando o `modelview` contém escala diferente de 1; caso contrário as normais ficariam “esticadas” e a luz sairia errada.

---

#### TL;DR

* **Comprimento/normalização genéricos?**  Faça em C++ (ou GLM) ou nos shaders (`length`, `normalize`).
* **Normais de iluminação na pipeline fixa?**  Sim, existe: `glEnable(GL_NORMALIZE)` (ou `GL_RESCALE_NORMAL`).
* Fora isso, não há “função OpenGL” para matemática vetorial — OpenGL sempre assumiu que você trata desses cálculos do lado da aplicação ou no shader.



Não precisa ― desde C++11 o `<cmath>` oferece três sobre-cargas de `std::hypot`:

```cpp
float       std::hypot(float       x, float       y);   // ↙ usa se x/y forem float
double      std::hypot(double      x, double      y);   // ↙ usa se x/y forem double
long double std::hypot(long double x, long double y);
```

`GLfloat` é, na prática, um `float`, portanto

```cpp
GLfloat diag = std::hypot(WORLD_W, WORLD_H);   // OK, chama overload float
```

compila sem cast e devolve um `float` (mesmo tipo dos argumentos).

### Quando **poderia** fazer cast?

* Se quiser **mais precisão**, converta para `double`:

  ```cpp
  double diag = std::hypot(static_cast<double>(WORLD_W),
                           static_cast<double>(WORLD_H));
  ```

* Se estiver preso a um compilador **antigo** (pré-C++11) que só possui `double hypot(double,double)` ― hoje é raro ―
  basta o cast ou usar a forma C:

  ```cpp
  float  diag = hypotf(WORLD_W, WORLD_H);   // versão `hypotf` do C99
  ```

Mas, em compiladores modernos, `std::hypot(WORLD_W, WORLD_H)` já é o caminho mais limpo e seguro, sem necessidade de conversão nenhuma.
