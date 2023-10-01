#include "GL/glut.h"
#include <cmath>

// 函数用于绘制线段，参数为起点和终点的坐标
void drawLine(float startX, float startY, float endX, float endY) {
    glBegin(GL_LINES);
    glVertex2f(startX, startY);
    glVertex2f(endX, endY);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制五角星外部轮廓
    float centerX = 250;
    float centerY = 250;
    float outerRadius = 100;
    float innerRadius = 40;

    for (int i = 0; i < 5; i++) {
        float outerStartAngle = i * 72.0 * M_PI / 180.0;
        float outerEndAngle = (i + 1) * 72.0 * M_PI / 180.0;
        float innerStartAngle = (i + 0.5) * 72.0 * M_PI / 180.0;
        float innerEndAngle = (i + 1.5) * 72.0 * M_PI / 180.0;

        float outerStartX = centerX + outerRadius * cos(outerStartAngle);
        float outerStartY = centerY + outerRadius * sin(outerStartAngle);
        float outerEndX = centerX + outerRadius * cos(outerEndAngle);
        float outerEndY = centerY + outerRadius * sin(outerEndAngle);

        float innerStartX = centerX + innerRadius * cos(innerStartAngle);
        float innerStartY = centerY + innerRadius * sin(innerStartAngle);
        float innerEndX = centerX + innerRadius * cos(innerEndAngle);
        float innerEndY = centerY + innerRadius * sin(innerEndAngle);

        drawLine(outerStartX, outerStartY, innerEndX, innerEndY);
        drawLine(innerStartX, innerStartY, outerEndX, outerEndY);
    }

    glFlush();
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w - 1, 0, h - 1);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("simple");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}