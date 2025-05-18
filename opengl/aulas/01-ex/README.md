Neste exemplo, usamos a biblioteca GLUT para criar a janela e
gerenciar os eventos do OpenGL. A função display é chamada
sempre que a janela precisa ser redesenhada. Usamos a função
glClear para limpar o buffer de cor, e definimos a cor para
branco usando a função glColor3f. Em seguida, usamos o comando
glBegin(GL_LINE_LOOP) para começar a desenhar a forma
geométrica, no caso um círculo. Usamos um laço for para calcular
os pontos do círculo em coordenadas cartesianas, e adicionamos
cada ponto usando a função glVertex2f. Finalmente, usamos o
comando glEnd para encerrar o desenho do círculo, e a função
glFlush para atualizar a janela.
