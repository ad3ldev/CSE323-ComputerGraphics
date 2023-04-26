#include "headers.hpp"
#include "planet.hpp"

#define SUN_RADIUS 30
#define NUMNER_OF_BODIES 9

// Globals.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = -50, zVal = 0; // Co-ordinates of the spacecraft.
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft; // Display lists base index.
static int frameCount = 0; // Number of frames

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
	char *c;
	
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

class Body
{
public:
	Body();
	Body(float x, float y, float z, float r, float d, unsigned char colorR,
		 unsigned char colorG, unsigned char colorB, unsigned int texture, unsigned int map);
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRadius() { return radius; }
	float getDistance() {return distance; }
	
	unsigned int getTexture(){ return texture;}
	unsigned int getMap() { return map; }
	void draw();
	
private:
	float centerX, centerY, centerZ, radius, distance;
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

// Asteroid constructor.
Body::Body(float x, float y, float z, float r, float d, unsigned char colorR, unsigned char colorG, unsigned char colorB, unsigned int texture, unsigned map)
{
	centerX = x;
	centerY = y;
	centerZ = z;
	radius = r;
	distance = d;
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
}

// Function to draw asteroid.
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

Body sun_and_planets[NUMNER_OF_BODIES]; // Global array of asteroids.

// Routine to count the number of frames drawn every second.
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
	glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
	glColor3f(1.0, 1.0, 1.0);
	glutWireCone(5.0, 10.0, 10, 10);
	glPopMatrix();
	glEndList();
	
	float distance = SUN_RADIUS;
	float radius = SUN_RADIUS;
	Body sun = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	
	distance += radius;
	radius = SUN_RADIUS;
	Body mercury = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	
	distance += radius;
	radius = SUN_RADIUS;
	Body venus = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	distance += radius;
	
	distance += radius;
	radius = SUN_RADIUS;
	Body earth = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	
	distance += radius;
	radius = SUN_RADIUS;
	Body mars = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	distance += radius;
	
	distance += radius;
	radius = SUN_RADIUS;
	Body jupiter = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	distance += radius;
	
	distance += radius;
	radius = SUN_RADIUS;
	Body saturn = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	distance += radius;
	
	distance += radius;
	radius = SUN_RADIUS;
	Body uranus = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	distance += radius;
	
	distance += radius;
	radius = SUN_RADIUS;
	Body neptune = Body(0, 0, 0, radius, distance, rand()%256, rand()%256, rand()%256, 0, 0);
	distance += radius;
	
	sun_and_planets[0] = sun;
	sun_and_planets[1] = mercury;
	sun_and_planets[2] = venus;
	sun_and_planets[3] = earth;
	sun_and_planets[4] = mars;
	sun_and_planets[5] = jupiter;
	sun_and_planets[6] = saturn;
	sun_and_planets[7] = uranus;
	sun_and_planets[8] = neptune;
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glutTimerFunc(0, frameCounter, 0); // Initial call of frameCounter().
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
							 float x2, float y2, float z2, float r2)
{
	return ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) <= (r1 + r2)*(r1 + r2));
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
	int i, j;
	
	// Check for collision with each asteroid.
	for (int i = 0; i < NUMNER_OF_BODIES ; i++){
		if (sun_and_planets[i].getRadius() > 0) // If asteroid exists.
			if (checkSpheresIntersection(x - 5 * sin((M_PI / 180.0) * a), 0.0,
										 z - 5 * cos((M_PI / 180.0) * a), 7.072,
										 sun_and_planets[i].getCenterX(), sun_and_planets[i].getCenterY(),
										 sun_and_planets[i].getCenterZ(), sun_and_planets[i].getRadius()))
				return 1;
	}
	
	return 0;
}

// Drawing routine.
void drawScene(void)
{
	frameCount++; // Increment number of frames every redraw.
	
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Beg	in left viewport.
	glViewport(0, 0, width, height);//demo
	glLoadIdentity();
	
	// Write text in isolated (i.e., before gluLookAt) translate block.
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(-28.0, 25.0, -30.0);
	if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
	glPopMatrix();
	
	// Fixed camera.
	
	gluLookAt(xVal - 10 * sin((M_PI / 180.0) * angle),
			  0.0,
			  zVal - 10 * cos((M_PI / 180.0) * angle),
			  xVal - 11 * sin((M_PI / 180.0) * angle),
			  0.0,
			  zVal - 11 * cos((M_PI / 180.0) * angle),
			  0.0,
			  1.0,
			  0.0);
	
	// Draw all the asteroids in arrayAsteroids.
	for (i = 0; i<NUMNER_OF_BODIES; i++)
		sun_and_planets[i].draw();
	
	// End left viewport.
	
	// Begin right viewport.
	glViewport(width/1.5, 0, width / 3.0, height/3.0);
	glLoadIdentity();
	
	// Write text in isolated (i.e., before gluLookAt) translate block.
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(-28.0, 25.0, -30.0);
	if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
	glPopMatrix();
	
	// Draw a vertical line on the left of the viewport to separate the two viewports
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-5.0, -5.0, -5.0);
	glVertex3f(-5.0, 5.0, -5.0);
	glVertex3f(5.0, 5.0, -5.0);
	glVertex3f(5.0, -5.0, -5.0);
	glEnd();
	glLineWidth(1.0);
	
	// Locate the camera at the tip of the cone and pointing in the direction of the cone.
	gluLookAt(0.0, 100.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// Draw all the asteroids in arrayAsteroids.
	for (i = 0; i<NUMNER_OF_BODIES; i++)
		sun_and_planets[i].draw();
	
	
	// Draw spacecraft.
	glPushMatrix();
	glTranslatef(xVal, 0.0, zVal);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glCallList(spacecraft);
	glPopMatrix();
	// End right viewport.
	
	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
	glMatrixMode(GL_MODELVIEW);
	
	// Pass the size of the OpenGL window.
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

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;
	
	// Compute next position.
	if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
	if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
	if (key == GLUT_KEY_UP)
	{
		tempxVal = xVal - sin(angle * M_PI / 180.0);
		tempzVal = zVal - cos(angle * M_PI / 180.0);
	}
	if (key == GLUT_KEY_DOWN)
	{
		tempxVal = xVal + sin(angle * M_PI / 180.0);
		tempzVal = zVal + cos(angle * M_PI / 180.0);
	}
	
	// Angle correction.
	if (tempAngle > 360.0) tempAngle -= 360.0;
	if (tempAngle < 0.0) tempAngle += 360.0;
	
	// Move spacecraft to next position only if there will not be collision with an asteroid.
	if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle))
	{
		isCollision = 0;
		xVal = tempxVal;
		zVal = tempzVal;
		angle = tempAngle;
	}
	else isCollision = 1;
	
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
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("spaceTravel.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	setup();
	
	glutMainLoop();
}
