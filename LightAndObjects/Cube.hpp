#pragma once
#include <GL/glut.h>

// Класс куба
class Cube {
private:
    float cubeSide; // длина ребра куба
public:
    Cube(float side_lenght); // конструктор
    ~Cube(); // деструктор
    void Draw(); // функция отрисовки куба
};
