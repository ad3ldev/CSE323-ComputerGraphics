///////////////////////////////////////////////////////////
// helixList.cpp
//
// This program draws several helixes using a display list.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////

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
static float x_rotate = 0;
static float y_rotate = 0;
static float z_rotate = 0;

static float radius = 12;
static float origin = -60;
static float slices = 25;
static float loops = 10;
static float height = 10;

static float top_x_scale = 1.5;
static float top_y_scale = 1.5;
static float top_z_scale = 1;
static float leg_x_scale = 0.4;
static float leg_y_scale = 0.4;
static float leg_z_scale = 3;

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	
	if(wireframe == 0){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};
	
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(radius, -height, origin);
	glRotatef(90+x_rotate, 1, 0.5, 0);
	glRotatef(y_rotate, 0, 1, 0);
	glRotatef(z_rotate, 0, 0, 1);
	glScalef(leg_x_scale, leg_y_scale, leg_z_scale);
	glCallList(cylinders);
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0, -height, origin-radius);
	glRotatef(90+x_rotate, 0.5, 0, 0);
	glRotatef(y_rotate, 0, 1, 0);
	glRotatef(z_rotate, 0, 0, 1);
	glScalef(leg_x_scale, leg_y_scale, leg_z_scale);
	glCallList(cylinders);
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0);
	glPushMatrix();
	glTranslatef(-radius, -height, origin);
	glRotatef(90+x_rotate, 1, -0.5, 0);
	glRotatef(y_rotate, 0, 1, 0);
	glRotatef(z_rotate, 0, 0, 1);
	glScalef(leg_x_scale, leg_y_scale, leg_z_scale);
	glCallList(cylinders);
	glPopMatrix();
	
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0, 0, origin);
	glRotatef(90+x_rotate, 1, 0, 0);
	glRotatef(y_rotate, 0, 1, 0);
	glRotatef(z_rotate, 0, 0, 1);
	glScalef(top_x_scale, top_y_scale, top_z_scale);
	glCallList(cylinders);
	glPopMatrix();
	
	glFlush();
}

// Initialization routine.
void setup(void)
{
	cylinders = glGenLists(1);
	// Begin create a display list.
	glNewList(cylinders, GL_COMPILE);
	gluDisk(gluNewQuadric(), 0, radius, slices, loops);
	gluCylinder(gluNewQuadric(), radius, radius, height, slices, loops);
	glPushMatrix();
	glTranslatef(0, 0, height);
	gluDisk(gluNewQuadric(), 0, radius, slices, loops);
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
			x_rotate += 1;
			break;
		case 'X':
			x_rotate -= 1;
			break;
		case 'y':
			y_rotate += 1;
			break;
		case 'Y':
			y_rotate -= 1;
			break;
		case 'z':
			z_rotate += 1;
			break;
		case 'Z':
			z_rotate -= 1;
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
