#include <GL/glut.h>
#include <cmath>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++)
    {
        float rad = i * M_PI / 180.0;
        glVertex2f(cos(rad), sin(rad));
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Circle");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
