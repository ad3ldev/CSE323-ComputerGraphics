#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

// Globals.
static GLuint cylinders;
static int wireframe = 0;
static float x_rotate = 90;
static float y_rotate = 0;
static float z_rotate = 0;

static float radius = 12;
static float origin = -65;
static float slices = 25;
static float loops = 10;
static float height = 10;
static int step = 10;
static float leg_angle = 15;

float to_rad(float angle)
{
    return angle * M_PI / 180;
}
// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    if (wireframe == 0)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL_NV);
    };
    glPushMatrix();

    //	glRotatef(x_rotate, 1, 0, 0);
    //	glRotatef(y_rotate, 0, 1, 0);
    //	glRotatef(z_rotate, 0, 0, 1);

    float angle = 0;
    float x_translate = radius * sin(to_rad(angle));
    float y_translate = radius * cos(to_rad(angle));
    float z_translate = height;

    glPushMatrix();
    glTranslatef(0, 0, origin);

    glPushMatrix();
    glTranslatef(x_translate, y_translate, z_translate);
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(-x_translate, -y_translate, -z_translate);
    glRotatef(x_rotate, 1, 0, 0);
    glRotatef(y_rotate, 0, 1, 0);
    glRotatef(-z_rotate, 0, 0, 1);
    glTranslatef(x_translate, y_translate, z_translate);
    glPushMatrix();
    glRotatef(leg_angle, -cos(to_rad(angle)), sin(to_rad(angle)), 0);
    glCallList(cylinders);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    angle = 120;
    x_translate = radius * sin(to_rad(angle));
    y_translate = radius * cos(to_rad(angle));
    z_translate = height;
    glPushMatrix();
    glTranslatef(x_translate, y_translate, z_translate);
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(-x_translate, -y_translate, -z_translate);
    glRotatef(x_rotate, 1, 0, 0);
    glRotatef(y_rotate, 0, 1, 0);
    glRotatef(-z_rotate, 0, 0, 1);
    glTranslatef(x_translate, y_translate, z_translate);
    glPushMatrix();
    glRotatef(leg_angle, -cos(to_rad(angle)), sin(to_rad(angle)), 0);
    glCallList(cylinders);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    angle = 240;
    x_translate = radius * sin(to_rad(angle));
    y_translate = radius * cos(to_rad(angle));
    z_translate = height;
    glPushMatrix();
    glTranslatef(x_translate, y_translate, z_translate);
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-x_translate, -y_translate, -z_translate);
    glRotatef(x_rotate, 1, 0, 0);
    glRotatef(y_rotate, 0, 1, 0);
    glRotatef(-z_rotate, 0, 0, 1);
    glTranslatef(x_translate, y_translate, z_translate);
    glPushMatrix();
    glRotatef(leg_angle, -cos(to_rad(angle)), sin(to_rad(angle)), 0);
    glCallList(cylinders);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef(x_rotate, 1, 0, 0);
    glRotatef(y_rotate, 0, 1, 0);
    glRotatef(z_rotate, 0, 0, 1);
    glPushMatrix();
    gluDisk(gluNewQuadric(), 0, radius * 1.5, slices, loops);
    gluCylinder(gluNewQuadric(), radius * 1.5, radius * 1.5, height, slices, loops);
    glPushMatrix();
    glTranslatef(0, 0, height);
    gluDisk(gluNewQuadric(), 0, radius * 1.5, slices, loops);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
    glFlush();
}

// Initialization routine.
void setup(void)
{
    cylinders = glGenLists(1);
    // Begin create a display list.
    glNewList(cylinders, GL_COMPILE);
    gluDisk(gluNewQuadric(), 0, radius / 4, slices, loops);
    gluCylinder(gluNewQuadric(), radius / 4, radius / 4, 3 * height, slices, loops);
    glPushMatrix();
    glTranslatef(0, 0, 3 * height);
    gluDisk(gluNewQuadric(), 0, radius / 4, slices, loops);
    glPopMatrix();
    glEndList();
    // End create a display list.
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'x':
        x_rotate += step;
        break;
    case 'X':
        x_rotate -= step;
        break;
    case 'y':
        y_rotate += step;
        break;
    case 'Y':
        y_rotate -= step;
        break;
    case 'z':
        z_rotate += step;
        break;
    case 'Z':
        z_rotate -= step;
        break;
    case ' ':
        wireframe = wireframe == 0 ? 1 : 0;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Stool");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
