/* write user interaction here for good practice */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string.h>
static enum choice {ortho=1, prespective=2};
static enum userInput {zoomIn='i', zoomOut='o', stopSpinning=' ', CCW=GLUT_LEFT_BUTTON, CW= GLUT_RIGHT_BUTTON};
int userChoice = 0;
float orthoLeft = -50;
float orthoRight = 50;
float orthoBottom = -50;
float orthoTop = 50;
float orthoNear = -5;
float orthoFar = 5;
float fruLeft = -5;
float fruRight = 5;
float fruBottom = -5;
float fruTop = 5;
float fruNear = 5;
float fruFar = 100;
float offsetX = 100;
float offsetY = 100;
float zOffset = -15;//zoom out/in
float windowWidth = 500;
float windowHeight = 500;
float spinY = 0;
float spinZ = 0;
float spinSpeed = 5;
float currentSpin = 0;
float prevTime = 0;

int milliToSecond = 1000;
void printUserInteraction(std::string button);
// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1.0); // Default line width.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	switch (userChoice) {
	case ortho:
			glRotatef(currentSpin, 0, spinY, spinZ);
			glBegin(GL_TRIANGLES);
			glVertex3f(0, 10, 0);
			glVertex3f(-30, 0, 0);
			glVertex3f(30, 0, 0);
			glEnd();
		break;
	case prespective:
			glTranslatef(0, 0, zOffset);
			glRotatef(currentSpin, 0, spinY, spinZ);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0, 5, 0);
			glVertex3f(5, 0, 5);
			glVertex3f(5, 0, -5);
			glVertex3f(-5, 0, -5);
			glVertex3f(-5, 0, 5);
			glVertex3f(5, 0, 5);
			glEnd();
		break;
	default:
		break;
	}
	glFlush();
}
// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
//spin logic
void spinDisplay(void)
{
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	if((int)prevTime % milliToSecond == 0){
		prevTime = 0;
		currentSpin += spinSpeed;
		glutPostRedisplay();
	}
}

void spinDisplayReverse(void)
{
	prevTime=glutGet(GLUT_ELAPSED_TIME);
	if((int)prevTime % milliToSecond == 0){
		currentSpin -= spinSpeed;
		glutPostRedisplay();
	}
}

//keyboard & mouse
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case CCW:
		glutIdleFunc(*spinDisplay);
		printUserInteraction("Left Mouse Button");
		break;
	case CW:
		glutIdleFunc(*spinDisplayReverse);
		printUserInteraction("Right Mouse Button");
		break;
	default:
		break;
	}
}
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case zoomIn:
			zOffset += 1;
			glutPostRedisplay();
		break;
	case zoomOut:
			zOffset -= 1;
			glutPostRedisplay();
		break;
	case stopSpinning:
		glutIdleFunc(NULL);
		break;
	default:
		break;
	}
	std::string key_pressed;
	key_pressed+=key;
	printUserInteraction(key_pressed);
}
// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (userChoice) {
	case ortho:
		spinY = 0;
		spinZ = 1;
		glOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);
		printUserInteraction("1");
		break;
	case prespective:
		spinY = 1;
		spinZ = 0;
		glFrustum(fruLeft, fruRight, fruBottom, fruTop, fruNear, fruFar);
		printUserInteraction("2");
		break;
	default:
		break;
	}
	glMatrixMode(GL_MODELVIEW);
}
// user interaction
void printUserInteraction(std::string button) {
	std::cout << "User Pressed: {" << button << "}" << std::endl;
}


// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	std::cout << "Which projection type do you want\n1) parallel projection\n2) perspective projection\n>> ";
	std::cin >> userChoice;
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(offsetX, offsetY);
	glutCreateWindow("Lab 2.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouse);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
//Note: At first user interaction, if scene spins unexpectadly you need to handle this behaviour (Hint: use prevTime variable)
