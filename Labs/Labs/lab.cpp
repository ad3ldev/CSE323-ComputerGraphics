#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <filesystem>

#include <GL/glew.h>
#include <GL/freeglut.h>
#define NUMNER_OF_BODIES 10

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static int width, height;
static float angle = 0;
static float xVal = -500, zVal = 0;
static int frameCount = 0;
static float speed = 20;
static float orbit = rand() % 360;
static float rotate_speed = 0;

const char * images[] = {
	"textures/sun.jpg",
	"textures/mercury.jpg",
	"textures/venus.jpg",
	"textures/earth.jpg",
	"textures/mars.jpg",
	"textures/jupiter.jpg",
	"textures/saturn.jpg",
	"textures/uranus.jpg",
	"Labs/textures/neptune.jpg",
	"textures/moon.jpg"
};

GLuint loadTexture(const char * filename) {
	int width, height, bpp = 0;
	unsigned char * data = stbi_load(filename, &width, &height, &bpp, 0);
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width, height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
	stbi_image_free(data);
	return textureId;
}

class Body
{
public:
	Body();
	Body(float r, float d, float s, float rotate, GLuint texture, float mat_em[4], float mat_am[4] ,float mat_sh[1]);
	float getRadius() { return radius; }
	float getDistance() { return distance; }
	float getSpeed() { return speed; }
	unsigned int getTexture() { return texture; }
	void draw();
	
private:
	GLUquadric *body = gluNewQuadric();
	float radius, distance, speed, rotation;
	GLuint texture;
	float mat_emission [4];
	float mat_ambient [4];
	float mat_shine [1];
};

Body::Body()
{
	radius = 0;
	distance = 0;
	texture = 0;
	rotation = 0;
}

Body::Body(float r, float d, float s, float rotate, GLuint textureImage, float mat_em[4], float mat_am[4], float mat_sh[1])
{
	radius = r;
	speed = s;
	distance = d;
	texture = textureImage;
	rotation = rotate;
	for (int i = 0; i < 4; i++){
		mat_emission[i] = mat_em[i];
		mat_ambient[i] = mat_am[i];
	}
	mat_shine[0] = mat_sh[0];
}

void Body::draw()
{
	if (radius > 0.0)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glRotatef(90, 1, 0, 0);
		glRotatef(rotate_speed*rotation, 1, 1, 1);
		gluQuadricTexture(body,true);
		gluSphere(body, radius, 100, 100);
		glPopMatrix();
	}
}

Body sun_and_planets[NUMNER_OF_BODIES];
void draw_spacecraft()
{
	glPushMatrix();
	glTranslatef(xVal, 0.0, zVal);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutWireCone(10.0, 40.0, 10, 10);
	glPopMatrix();
	glPopMatrix();
}
void draw_box()
{
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(-28.0, 25.0, -30.0);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-5.0, -5.0, -5.0);
	glVertex3f(-5.0, 5.0, -5.0);
	glVertex3f(5.0, 5.0, -5.0);
	glVertex3f(5.0, -5.0, -5.0);
	glEnd();
}
void draw_solar()
{
	int i = 0;
	GLfloat position[] = {0.0, 0.0, 0, 1.0};
	
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	sun_and_planets[i].draw();
	for (i = 0; i < NUMNER_OF_BODIES - 1; i++)
	{
		glPushMatrix();
		glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
		glTranslatef(-sun_and_planets[i].getDistance(), 0, -sun_and_planets[i].getDistance());
		glRotatef(orbit * sun_and_planets[i].getSpeed(), 0, 1, 0);
		glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
		sun_and_planets[i].draw();
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(sun_and_planets[3].getDistance(), 0, sun_and_planets[3].getDistance());
	glTranslatef(-sun_and_planets[3].getDistance(), 0, -sun_and_planets[3].getDistance());
	glRotatef(orbit * sun_and_planets[3].getSpeed(), 0, 1, 0);
	glTranslatef(sun_and_planets[3].getDistance(), 0, sun_and_planets[3].getDistance());
	glPushMatrix();
	glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
	glTranslatef(-sun_and_planets[i].getDistance(), 0, -sun_and_planets[i].getDistance());
	glRotatef(orbit * sun_and_planets[i].getSpeed(), 0, 1, 0);
	glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
	sun_and_planets[i].draw();
	glPopMatrix();
	glPopMatrix();
}
void draw_all()
{
	draw_solar();
	draw_spacecraft();
}
void orbiting(int value)
{
	rotate_speed += 1;
	orbit += 1;
	glutPostRedisplay();
	glutTimerFunc(100, orbiting, 1);
}
void frameCounter(int value)
{
	if (value != 0) // No output the first time frameCounter() is called (from main()).
		std::cout << "FPS = " << frameCount << std::endl;
	frameCount = 0;
	glutTimerFunc(1000, frameCounter, 1);
}

void init_bodys(){
	float distance = 0;
	float radius = 50;
	float speed = 0;
	float rotation = 10;
	float mat_emission[] = {1,1,1,1};
	float mat_ambient[] = {0,0,0,0};
	float mat_shine[] = {0};
	GLuint texture = loadTexture(images[0]);
	Body sun = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	mat_emission[0] = 0;
	mat_emission[1] = 0;
	mat_emission[2] = 0;
	mat_emission[3] = 0;
	mat_ambient[0] = 1;
	mat_ambient[1] = 1;
	mat_ambient[2] = 1;
	mat_ambient[3] = 1;
	
	distance = 60;
	radius = 5;
	speed = 8.8;
	texture = loadTexture(images[1]);
	Body mercury = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	distance = 100;
	radius = 12;
	speed = 6.5;
	texture = loadTexture(images[2]);
	Body venus = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	distance = 150;
	radius = 13;
	speed = 5.5;
	texture = loadTexture(images[3]);
	Body earth = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	distance = 200;
	radius = 7;
	speed = 4.5;
	texture = loadTexture(images[4]);
	Body mars = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	distance = 250;
	radius = 30;
	speed = 2.5;
	texture = loadTexture(images[5]);
	Body jupiter = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	distance = 300;
	radius = 20;
	speed = 2;
	texture = loadTexture(images[6]);
	Body saturn =Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	distance = 350;
	radius = 15;
	speed = 1.5;
	texture = loadTexture(images[7]);
	Body uranus = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	distance = 400;
	radius = 14;
	speed = 1;
	texture = loadTexture(images[8]);
	Body neptune = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	mat_shine[0] = 1;
	distance = 25;
	radius = 4;
	speed = 10;
	texture = loadTexture(images[9]);
	Body moon = Body(radius, distance, speed, rotation,texture, mat_emission, mat_ambient, mat_shine);
	
	sun_and_planets[0] = sun;
	sun_and_planets[1] = mercury;
	sun_and_planets[2] = venus;
	sun_and_planets[3] = earth;
	sun_and_planets[4] = mars;
	sun_and_planets[5] = jupiter;
	sun_and_planets[6] = saturn;
	sun_and_planets[7] = uranus;
	sun_and_planets[8] = neptune;
	sun_and_planets[9] = moon;
}
// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	init_bodys();
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	glutTimerFunc(0, frameCounter, 0);
	glutTimerFunc(0, orbiting, 0);
}

// Drawing routine.
void drawScene(void)
{
	frameCount++; // Increment number of frames every redraw.
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height); // demo
	glLoadIdentity();
	
	gluLookAt(xVal - 10 * sin((M_PI / 180.0) * angle),
			  0.0,
			  zVal - 10 * cos((M_PI / 180.0) * angle),
			  xVal - 11 * sin((M_PI / 180.0) * angle),
			  0.0,
			  zVal - 11 * cos((M_PI / 180.0) * angle),
			  0.0,
			  1.0,
			  0.0);
	draw_solar();
	
	glViewport(width / 1.5, 0, width / 3.0, height / 3.0);
	glLoadIdentity();
	draw_box();
	
	gluLookAt(0.0, 600.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	draw_all();
	
	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	width = w;
	height = h;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;
	
	// Compute next position.
	if (key == GLUT_KEY_LEFT)
		tempAngle = angle + 5.0;
	if (key == GLUT_KEY_RIGHT)
		tempAngle = angle - 5.0;
	if (key == GLUT_KEY_UP)
	{
		tempxVal = xVal - speed * sin(angle * M_PI / 180.0);
		tempzVal = zVal - speed * cos(angle * M_PI / 180.0);
	}
	if (key == GLUT_KEY_DOWN)
	{
		tempxVal = xVal + speed * sin(angle * M_PI / 180.0);
		tempzVal = zVal + speed * cos(angle * M_PI / 180.0);
	}
	
	// Angle correction.
	if (tempAngle > 360.0)
		tempAngle -= 360.0;
	if (tempAngle < 0.0)
		tempAngle += 360.0;
	
	// Move spacecraft to next position only if there will not be collision with an asteroid.
	xVal = tempxVal;
	zVal = tempzVal;
	angle = tempAngle;
	
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press the left/right arrow keys to turn the craft." << std::endl
	<< "Press the up/down arrow keys to move the craft." << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
	
	printInteraction();
	glutInit(&argc, argv);
	
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Solar System.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glewExperimental = GL_TRUE;
	glewInit();
	
	setup();
	
	glutMainLoop();
}
