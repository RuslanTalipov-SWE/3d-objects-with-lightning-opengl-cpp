#include "Cube.hpp"

Cube::Cube(float side_lenght) {
    cubeSide = side_lenght; // длина ребра куба
}

Cube::~Cube() {
}

// Функция отрисовки куба
void Cube::Draw() {

    // Передняя грань
    glBegin(GL_POLYGON); // начало отрисовки передней грани куба
    
    glNormal3d(0.0f, cubeSide, 0.0f); // нормаль
    
    glTexCoord2d(0, 0); // координаты текстуры левой нижней вершины полигона
    glVertex3f(cubeSide, cubeSide, 0.0f); // координаты левой нижней вершины полигона
    
    glTexCoord2d(1, 0); // координаты текстуры правой нижней вершины полигона
    glVertex3f(-cubeSide, cubeSide, 0.0f); // координаты правой нижней вершины полигона
    
    glTexCoord2d(1, 1); // координаты текстуры правой верхней вершины полигона
    glVertex3f(-cubeSide, cubeSide, 2 * cubeSide); // координаты правой верхней вершины полигона
    
    glTexCoord2d(0, 1); // координаты текстуры левой нижней вершины полигона
    glVertex3f(cubeSide, cubeSide, 2 * cubeSide); // координаты левой нижней вершины полигона
    
    glEnd(); // окончание отрисовки передней грани куба
    
    // Задняя грань
    glBegin(GL_POLYGON);
    glNormal3d(0.0f, -cubeSide, 0.0f);
    glTexCoord2d(0, 0);
    glVertex3f(cubeSide, -cubeSide, 2 * cubeSide);
    glTexCoord2d(1, 0);
    glVertex3f(-cubeSide, -cubeSide, 2 * cubeSide);
    glTexCoord2d(1, 1);
    glVertex3f(-cubeSide, -cubeSide, 0.0f);
    glTexCoord2d(0, 1);
    glVertex3f(cubeSide, -cubeSide, 0.0f);
    glEnd();

    // Верхняя грань
    glBegin(GL_POLYGON);
    glNormal3d(0.0f, 0.0f, 2 * cubeSide);
    glTexCoord2d(0, 0);
    glVertex3f(cubeSide, cubeSide, 2 * cubeSide);
    glTexCoord2d(1, 0);
    glVertex3f(-cubeSide, cubeSide, 2 * cubeSide);
    glTexCoord2d(1, 1);
    glVertex3f(-cubeSide, -cubeSide, 2 * cubeSide);
    glTexCoord2d(0, 1);
    glVertex3f(cubeSide, -cubeSide, 2 * cubeSide);
    glEnd();

    // Нижняя грань
    glBegin(GL_POLYGON);
    glNormal3d(0.0f, 0.0f, -cubeSide);
    glTexCoord2d(0, 0);
    glVertex3f(cubeSide, -cubeSide, 0.0f);
    glTexCoord2d(1, 0);
    glVertex3f(-cubeSide, -cubeSide, 0.0f);
    glTexCoord2d(1, 1);
    glVertex3f(-cubeSide, cubeSide, 0.0f);
    glTexCoord2d(0, 1);
    glVertex3f(cubeSide, cubeSide, 0.0f);
    glEnd();

    // Правая грань
    glBegin(GL_POLYGON);
    glNormal3d(-cubeSide, 0.0f, 0.0f);
    glTexCoord2d(0, 0);
    glVertex3f(-cubeSide, cubeSide, 2 * cubeSide);
    glTexCoord2d(1, 0);
    glVertex3f(-cubeSide, cubeSide, 0.0f);
    glTexCoord2d(1, 1);
    glVertex3f(-cubeSide, -cubeSide, 0.0f);
    glTexCoord2d(0, 1);
    glVertex3f(-cubeSide, -cubeSide, 2 * cubeSide);
    glEnd();

    // Левая грань
    glBegin(GL_POLYGON);
    glNormal3d(cubeSide, 0.0f, 0.0f);
    glTexCoord2d(0, 0);
    glVertex3f(cubeSide, cubeSide, 0.0f);
    glTexCoord2d(1, 0);
    glVertex3f(cubeSide, cubeSide, 2 * cubeSide);
    glTexCoord2d(1, 1);
    glVertex3f(cubeSide, -cubeSide, 2 * cubeSide);
    glTexCoord2d(0, 1);
    glVertex3f(cubeSide, -cubeSide, 0.0f);
    glEnd();
}
