#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <vector>
#include <GL/glut.h>

#include "Cube.hpp"

GLuint  texture;

// Структура для загрузки текстуры
struct texture {
    int wdth; // ширина
    int hght; // высота
    unsigned char *image;
}get_texture;

Cube cube(0.8f); // длина ребра куба

// Угол поворота сцены
float angle = 0.0f;
// Положение камеры
float eyeX = 0.0f, eyeY = 12.0f, eyeZ = 5.0f; // позиция камеры (зрителя)
float centerX = 0.5f, centerY = 0.0f, centerZ = 0.5f; // положение цели (направление взгляда)

// Положение объектов на сцене
GLfloat dodecahedron_x = 1.0f, dodecahedron_y = 0.0f, dodecahedron_z = 4.3f; // двенадцатигранник
GLfloat sphere_x = 1.0f, sphere_y = 1.0f, sphere_z = 1.0f; // сфера
GLfloat cube_x = 1.0f, cube_y = -1.0f, cube_z = 0.0f; // куб

// Свойства источника света
GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f}; // цвет фонового освещения
GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f}; // цвет диффузного освещения
GLfloat light_specular[] = {0.9f, 0.9f, 0.9f}; // цвет зеркального отражения
GLfloat light_position[] = {0.0f, 0.0f, 2.5f}; // положение источника света

// Свойства материала поверхности (цвет хром)
GLfloat floor_ambient[] = {0.25f, 0.25f, 0.25f}; // цвет материала
GLfloat floor_diffuse[] = {0.4f, 0.4f, 0.4f}; // цвет диффузного отражения
GLfloat floor_specular[] = {0.774597f, 0.774597f, 0.774597f}; // цвет зеркального отражения
GLfloat floor_shininess[] = {76.8f}; // степень зеркального отражения

// Свойств материала куба (цвет жемчужный)
const GLfloat cube_ambient[] = {0.25f, 0.20725f, 0.20725f, 0.922f}; // цвет материала
const GLfloat cube_diffuse[] = {1.0f, 0.829f, 0.829f, 0.922f}; // цвет диффузного отражения
const GLfloat cube_specular[] = {0.296648f, 0.296648f, 0.296648f, 0.922f}; // цвет зеркального отражения
const GLfloat cube_shininess[] = {11.264f}; // степень зеркального отражения

// Свойств материала сферы (цвет золотой)
const GLfloat sphere_ambient[] = {0.24725f, 0.1995f, 0.0745f}; // цвет материала
const GLfloat sphere_diffuse[] = {0.75164f, 0.60648f, 0.22648f, 0.55f}; // цвет диффузного отражения
const GLfloat sphere_specular[] = {0.628281f, 0.555802f, 0.366065f}; // цвет зеркального отражения
const GLfloat sphere_shininess[] = {51.2f}; // степень зеркального отражения

// Свойств материала двенадцатигранника (цвет бирюзовый)
const GLfloat dodecahedron_ambient[] = {0.1f, 0.18725f, 0.1745f}; // цвет материала
const GLfloat dodecahedron_diffuse[] = {0.396f, 0.74151f, 0.69102f}; // цвет диффузного отражения
const GLfloat dodecahedron_specular[] = {0.297254f, 0.30829f, 0.306678f}; // цвет зеркального отражения
const GLfloat dodecahedron_shininess[] = {12.8f}; // степень зеркального отражения

// Функция загрузки текстуры
int load_texture(char *File, GLuint  *texture)
{
    // Загрузка из файла и считывание в память данных о изображении
    FILE *F;
    if ((F = fopen(File, "rb")) == NULL)
        return 0;
    fseek(F, 18, SEEK_SET);
    fread(&(get_texture.wdth), 2, 1, F);
    fseek(F, 2, SEEK_CUR);
    fread(&(get_texture.hght), 2, 1, F);

    if ((get_texture.image = (unsigned char *)malloc(sizeof(unsigned char) * 3 * get_texture.wdth * get_texture.hght)) == NULL) {
        fclose(F);
        return 0;
    }
    fseek(F, 30, SEEK_CUR);
    fread(get_texture.image, 3, get_texture.wdth * get_texture.hght, F);

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // метод фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // метод фильтрации
    glTexImage2D(GL_TEXTURE_2D, 0, 3, get_texture.wdth, get_texture.hght, 0, GL_RGB, GL_UNSIGNED_BYTE, get_texture.image);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, get_texture.wdth, get_texture.hght, GL_RGB, GL_UNSIGNED_BYTE, get_texture.image);
    free(get_texture.image); // освобождение памяти
    fclose(F); // закрытие файла
    return 1;
}

// Функция обработки изменения размеров окна
void resize(int width, int height) {
    const float ratio = (float)width / (float)height;

    glViewport(0, 0, width, height); // установка области вывода
    glMatrixMode(GL_PROJECTION); // режим матрицы проекций
    glLoadIdentity(); // сброс матрицы на единичную
    glFrustum(-0.75 * ratio, 0.75 * ratio, -1.0, 1.0, 2.0, 500.0); // перспективная проекция
    glMatrixMode(GL_MODELVIEW); // возвращение к матрице проекции
    glLoadIdentity(); // сброс матрицы на единичную
}

// Функция обработки нажатий клавиш перемещения
void press_arrow_keys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        light_position[1]-= 0.3f;
        break;
    case GLUT_KEY_DOWN:
        light_position[1]+= 0.3f;
        break;
    case GLUT_KEY_LEFT:
        light_position[0]+= 0.3f;
        break;
    case GLUT_KEY_RIGHT:
        light_position[0]-= 0.3f;
        break;
    }
}

// Функция обработки нажатий клавиатуры
void press_keys(unsigned char key, int x, int y) {
    
    // Выход из программы (кнопка Escape)
    switch (key)
    {
    case 27:
        exit(0);
        break;
    
    // Увеличение яркости истоника света
    case '+':
        light_ambient[0] += 0.1f;
        light_ambient[1] += 0.1f;
        light_ambient[2] += 0.1f;
        break;
            
    // Уменьшение яркости истоника света
    case '-':
        light_ambient[0] -= 0.1f;
        light_ambient[1] -= 0.1f;
        light_ambient[2] -= 0.1f;
        break;

    // Увеличение/уменьшение красного оттенка истоника света
    case 'r':
        light_ambient[0] += 0.1f;
        break;
    case 'f':
        light_ambient[0] -= 0.1f;
        break;

    // Увеличение/уменьшение зеленого оттенка истоника света
    case 't':
        light_ambient[1] += 0.1f;
        break;
    case 'g':
        light_ambient[1] -= 0.1f;
        break;

    // Увеличение/уменьшение синего оттенка истоника света
    case 'y':
        light_ambient[2] += 0.1f;
        break;
    case 'h':
        light_ambient[2] -= 0.1f;
        break;

    // Увеличение/уменьшение красного оттенка материала поверхности
    case 'u':
        floor_diffuse[0] += 0.1f;
        break;
    case 'j':
        floor_diffuse[0] -= 0.1f;
        break;
            
    // Увеличение/уменьшение зеленого оттенка материала поверхности
    case 'i':
        floor_diffuse[1] += 0.1f;
        break;
    case 'k':
        floor_diffuse[1] -= 0.1f;
        break;
    
    // Увеличение/уменьшение синего оттенка материала поверхности
    case 'o':
        floor_diffuse[2] += 0.1f;
        break;
    case 'l':
        floor_diffuse[2] -= 0.1f;
        break;

    // Движение камеры вперед/назад/влево/вправо
    case 'w':
        eyeY -= 0.1f;
        centerY -= 0.1f;
        break;
    case 's':
        eyeY += 0.1f;
        centerY += 0.1f;
        break;
    case 'a':
        eyeX += 0.1f;
        centerX += 0.1f;
        break;
    case 'd':
        eyeX -= 0.1f;
        centerX -= 0.1f;
        break;
    }
    
    // Установка максимальной и минимальной яркости источника света
    for (int i = 0; i < 3; i++) {
        if (light_ambient[i] > 1)
            light_ambient[i] = 1;
        if (light_ambient[i] < 0)
            light_ambient[i] = 0;
    }

    glutPostRedisplay(); // функция перерисовки текущего окна
}

// Функция отрисовки поверхности
void draw_plane(void) {

    // Свойства материала поверхности
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floor_ambient); // цвет фонового освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floor_diffuse); // цвет диффузного освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floor_specular); // цвет зеркального отражения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, floor_shininess); // степень зеркального отражения

    // Границы поверхности
    for (int i = -10; i <= 10; i++) {
        for (int j = -10; j <= 10; j++) {

            glBegin(GL_POLYGON); // начало отрисовки поверхности

            glNormal3d(0, 0, 1); // нормаль

            glVertex3d(i, j, 0);
            glVertex3d(i + 1, j, 0);
            glVertex3d(i + 1, j + 1, 0);
            glVertex3d(i, j + 1, 0);

            glEnd();  // окончание отрисовки поверхности
        }
    }
}

// Функция отрисовки
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка буфера цвета и буфера глубины

    // Характеристики источника света
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // цвет
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // положение

    glLoadIdentity(); // сброс матрицы на единичную

    // Позиционирование камеры
    gluLookAt(eyeX, eyeY, eyeZ,
              centerX, centerY, centerZ,
              0, 0, 1);

    // Свойства материала источника света
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient); // цвет фонового освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse); // цвет диффузного освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_specular); // цвет зеркального отражения

    // Отрисовка источника света
    glPushMatrix(); // сохранение текущих координат
    glTranslatef(light_position[0], light_position[1], light_position[2]); // положение
    glutSolidSphere(0.2, 30, 30); // отрисовка сферы
    glPopMatrix(); // возврат к старой системе координат

    // Вращение сцены
    glPushMatrix(); // сохранение текущих координат
    glRotatef(angle, 0, 0, 1); // поворот на заданную величину
    angle += 0.3; // угол поворота сцены
    
    // Функция отрисовки поверхности
    draw_plane();

    // Включение режима наложения текстур
    glEnable(GL_TEXTURE_2D);

    // Свойства материала куба
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cube_ambient); // цвет фонового освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cube_diffuse); // цвет диффузного освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cube_specular); // цвет зеркального отражения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cube_shininess); // степень зеркального отражения

    // Отрисовка куба
    glPushMatrix(); // сохранение текущих координат
    glTranslatef(cube_x, cube_y, cube_z); // положение на сцене
    cube.Draw(); // функция отрисовки куба
    glPopMatrix(); // возврат к старой системе координат

    // Выключение режима наложения текстуры
    glDisable(GL_TEXTURE_2D);

    // Свойства материала двенадцатигранника
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dodecahedron_ambient); // цвет фонового освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dodecahedron_diffuse); // цвет диффузного освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dodecahedron_specular); // цвет зеркального отражения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dodecahedron_shininess); // степень зеркального отражения

    // Отрисовка двенадцатигранника
    glPushMatrix(); // сохранение текущих координат
    glTranslatef(dodecahedron_x, dodecahedron_y, dodecahedron_z); // положение на сцене
    glutSolidDodecahedron(); // функция отрисовки двенадцатигранника
    glPopMatrix(); // возврат к старой системе координат

    // Включение режима смешивания цветов и выбор его параметров
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Свойства материала сферы
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sphere_ambient); // цвет фонового освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sphere_diffuse); // цвет диффузного освещения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_specular); // цвет зеркального отражения
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sphere_shininess); // степень зеркального отражения

    // Отрисовка сферы
    glPushMatrix(); // сохранение текущих координат
    glTranslatef(sphere_x, sphere_y, sphere_z); // положение на сцене
    glutSolidSphere(1, 50, 50);// функция отрисовки сферы
    glPopMatrix(); // возврат к старой системе координат

    // Выключение режима смешивания цветов
    glDisable(GL_BLEND);

    glPopMatrix(); // возврат к старой системе координат

    glutSwapBuffers(); // обмен буферов
}

// Главная функция программы
int main(int argc, char *argv[]) {
    // Инициализация GLUT и создание окна
    glutInit(&argc, argv); // инициализация окна
    glutInitWindowSize(800, 800); // размер окна
    glutInitWindowPosition(0, 0); // положение окна
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // установка режима отображения
    glutCreateWindow("РАБОТА С ИСТОЧНИКОМ СВЕТА В ТРЕХМЕРНЫХ СЦЕНАХ"); // создание окна

    // Установка функций обратного вызова
    glutDisplayFunc(display); // функция отрисовки
    glutReshapeFunc(resize); // функция обработки изменения размеров окна
    glutKeyboardFunc(press_keys); // функция обработки нажатий клавиатуры
    glutSpecialFunc(press_arrow_keys); // функция обработки нажатий клавиш перемещения
    glutIdleFunc(glutPostRedisplay); // функция перерисовки текущего окна

    glClearColor(0, 0, 0, 1);  // цвет очистки

    // Тест глубины
    glEnable(GL_DEPTH_TEST);  // тест глубины
    glDepthFunc(GL_LESS); // условие проверки глубины
    glDepthMask(GL_TRUE); // статус записи для буфера глубины

    // Система освещения
    glEnable(GL_LIGHTING); // включение системы освещения
    glEnable(GL_LIGHT0); // включение источника света
    glEnable(GL_NORMALIZE); // автоматическое приведение нормалей к единичной длине

    // Свойства материала источника света
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // фоновое освещение
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); // рассеянное освещение
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);  // отраженное освещение
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // положение источника света

    // Режимом интерполяции цветов
    glShadeModel(GL_SMOOTH);

    load_texture((char *)"/Users/ruslantalipov/Desktop/Practice/OpenGL_iMac/OpenGL_iMac/texture1.bmp", &texture);

    // Основной цикл
    glutMainLoop();

    return 0;
}
