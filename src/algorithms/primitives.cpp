#include <GL/glut.h>

void drawGrid(int size) {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    for (int i = -size; i <= size; i++) {
        glVertex2f(i, -size);
        glVertex2f(i, size);
        glVertex2f(-size, i);
        glVertex2f(size, i);
    }
    glEnd();
}
