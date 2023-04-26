#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#define NUMNER_OF_BODIES 10

static int width, height;
static float angle = 90;
static float xVal = -500, zVal = 0;
static unsigned int spacecraft;
static int frameCount = 0;
static float speed = 20;
static float orbit = rand()%360;

class Body
{
public:
	Body();
	Body(float x, float y, float z, float r, float d, float s, unsigned char colorR,
		 unsigned char colorG, unsigned char colorB, unsigned int texture, unsigned int map);
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRadius() { return radius; }
	float getDistance() { return distance; }
	float getSpeed() {return speed;}
	unsigned int getTexture() { return texture; }
	unsigned int getMap() { return map; }
	void draw();
	
private:
	float centerX, centerY, centerZ, radius, distance, speed;
	unsigned char color[3];
	//	TODO: change this so it actual texture and map
	unsigned int texture, map;
};

Body::Body()
{
	centerX = 0;
	centerY = 0;
	centerZ = 0;
	radius = 0;
	distance = 0;
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
	
	texture = 0;
	map = 0;
}

Body::Body(float x, float y, float z, float r, float d, float s, unsigned char colorR, unsigned char colorG, unsigned char colorB, unsigned int texture, unsigned map)
{
	centerX = x;
	centerY = y;
	centerZ = z;
	radius = r;
	speed = s;
	distance = d;
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
}

void Body::draw()
{
	if (radius > 0.0) // If asteroid exists.
	{
		glPushMatrix();
		glTranslatef(centerX, centerY, centerZ);
		glColor3ubv(color);
		glutWireSphere(radius, (int)radius * 6, (int)radius * 6);
		glPopMatrix();
	}
}

Body sun_and_planets[NUMNER_OF_BODIES];

void draw_solar(){
	int i = 0;
	sun_and_planets[i].draw();
	for (i = 0; i < NUMNER_OF_BODIES - 1; i++){
		glPushMatrix();
		glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
		glTranslatef(-sun_and_planets[i].getDistance(), 0, -sun_and_planets[i].getDistance());
		glRotatef(orbit*sun_and_planets[i].getSpeed(), 0, 1, 0);
		glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
		sun_and_planets[i].draw();
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(sun_and_planets[3].getDistance(), 0, sun_and_planets[3].getDistance());
	glTranslatef(-sun_and_planets[3].getDistance(), 0, -sun_and_planets[3].getDistance());
	glRotatef(orbit*sun_and_planets[3].getSpeed(), 0, 1, 0);
	glTranslatef(sun_and_planets[3].getDistance(), 0, sun_and_planets[3].getDistance());
	glPushMatrix();
	glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
	glTranslatef(-sun_and_planets[i].getDistance(), 0, -sun_and_planets[i].getDistance());
	glRotatef(orbit*sun_and_planets[i].getSpeed(), 0, 1, 0);
	glTranslatef(sun_and_planets[i].getDistance(), 0, sun_and_planets[i].getDistance());
	sun_and_planets[i].draw();
	glPopMatrix();
	glPopMatrix();
}
void orbiting(int value){
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

// Initialization routine.
void setup(void)
{
	spacecraft = glGenLists(1);
	glNewList(spacecraft, GL_COMPILE);
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutWireCone(5.0, 10.0, 10, 10);
	glPopMatrix();
	glEndList();
	
	float distance = 0;
	float radius = 30;
	float speed = 0;
	unsigned int red = 255;
	unsigned int green = 255;
	unsigned int blue = 0;
	Body sun = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 50;
	radius = 4;
	red = 26;
	green = 26;
	blue = 26;
	speed = 8.8;
	Body mercury = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 100;
	radius = 9;
	red = 230;
	green = 230;
	blue = 230;
	speed = 6.5;
	Body venus = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 150;
	radius = 10;
	red = 47;
	green = 106;
	blue = 105;
	speed = 5.5;
	Body earth = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 200;
	radius = 5;
	red = 153;
	green = 61;
	blue = 0;
	speed = 4.5;
	Body mars = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 250;
	radius = 20;
	red = 176;
	green = 127;
	blue = 53;
	speed = 2.5;
	Body jupiter = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 300;
	radius = 15;
	red = 176;
	green = 143;
	blue = 54;
	speed = 2;
	Body saturn = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 350;
	radius = 4;
	red = 13;
	green = 128;
	blue = 170;
	speed = 1.5;
	Body uranus = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 400;
	radius = 3;
	red = 12;
	green = 104;
	blue = 150;
	speed = 1;
	Body neptune = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
	distance = 25;
	radius = 5;
	red = 255;
	green = 255;
	blue = 255;
	speed = 10;
	Body moon = Body(0, 0, 0, radius, distance, speed, red, green, blue, 0, 0);
	
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
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
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
	glPushMatrix();
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
	glLineWidth(1.0);
	
	gluLookAt(0.0, 600.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	draw_solar();
	
	glPushMatrix();
	glTranslatef(xVal, 0.0, zVal);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glCallList(spacecraft);
	glPopMatrix();
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
		tempxVal = xVal - speed*sin(angle * M_PI / 180.0);
		tempzVal = zVal - speed*cos(angle * M_PI / 180.0);
	}
	if (key == GLUT_KEY_DOWN)
	{
		tempxVal = xVal + speed*sin(angle * M_PI / 180.0);
		tempzVal = zVal + speed*cos(angle * M_PI / 180.0);
	}
	
	// Angle correction.
	if (tempAngle > 360.0)
		tempAngle -= 360.0;
	if (tempAngle < 0.0)
		tempAngle += 360.0;
	
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
