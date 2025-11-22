#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Temporary render
    glBegin(GL_LINES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cosmic Observatory");

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
