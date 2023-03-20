///////////////////////////////////////////////////////////////////////////////////////
// hemisphere.cpp
//
// This program approximates a hemisphere with an array of latitudinal triangle strips.
//
// Interaction:
// Press P/p to increase/decrease the number of longitudinal slices.
// Press Q/q to increase/decrease the number of latitudinal slices.
// Press x, X, y, Y, z, Z to turn the hemisphere.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

// Globals.
static int shape = 0; // 0 for Helix, 1 for Sphere
static float R = 5.0; // Radius of Sphere and Helix
static int p = 10; // Number of longitudinal slices.
static int q = 5; // Number of latitudinal slices.
static int w = 0; // 0 Wireframe or 1 Filled Sphere
static float h = 2; // Pitch of Helix
static int n = 5; // Vertices of helix
static int t = 5; // Number of turns of the Helix
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float offset = -10;
static float spinSpeed = 5;
static float prev_time = 0;
static std::vector<std::vector<double>> pointColors;
// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	if(shape == 0){
		Xangle = -90;
	}
}

void drawSphere(void)
{
	if(w == 0){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glColor3f(0.0, 0.0, 0.0);
	int  i, j;
	// Array of latitudinal triangle strips, each parallel to the equator, stacked one
	// above the other from the equator to the north pole.
	for (j = -q; j < q; j++)
	{
		// One latitudinal triangle strip.
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= p; i++)
		{
			glVertex3f(R * cos((float)(j + 1) / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
					   R * sin((float)(j + 1) / q * M_PI / 2.0),
					   -R * cos((float)(j + 1) / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
			glVertex3f(R * cos((float)j / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
					   R * sin((float)j / q * M_PI / 2.0),
					   -R * cos((float)j / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
		}
		glEnd();
	}
	
}

void drawHelix(void)
{
	int Xc = 0;
	int Yc = 1;
	int Zc = -5;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 0.0);
	int count = 0;
	glBegin(GL_LINE_STRIP);
	for(int j = 0; j < t; j++){
		for(float i =0; i <2*M_PI; i += M_PI / n){
			glVertex3f(Xc + R * cos((float)(i)),Yc + R * sin((float)(i)), Zc + h*(j+(i/(2*M_PI))));
			count++;
		}
	}
	glEnd();
	
	while (count != pointColors.size()){
		if(count > pointColors.size()){
			std::vector<double> temp;
			double r =((double) rand() / (RAND_MAX));
			double g =((double) rand() / (RAND_MAX));
			double b =((double) rand() / (RAND_MAX));
			temp.push_back(r);
			temp.push_back(g);
			temp.push_back(b);
			pointColors.push_back(temp);
		}
		else if (count < pointColors.size()){
			pointColors.pop_back();
		}
	}
	
	glPointSize(5);
	int current = 0;
	glBegin(GL_POINTS);
	for(int j = 0; j < t; j++){
		for(float i =0; i <2*M_PI; i += M_PI / n){
			glColor3f(pointColors[current][0],pointColors[current][1],pointColors[current][2]);
			glVertex3f(Xc + R * cos((float)(i)),Yc + R * sin((float)(i)), Zc + h*(j+(i/(2*M_PI))));
			current++;
		}
	}
	glEnd();
}
// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	
	// Command to push the hemisphere, which is drawn centered at the origin,
	// into the viewing frustum.
	glTranslatef(0.0, 0.0, offset);
	
	// Commands to turn the hemisphere.
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);
	
	// Hemisphere properties.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 0.0);
	
	if(shape==1){
		drawSphere();
	}else{
		drawHelix();
	}
	glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void spinDisplay() {
	Yangle += spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
	prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
	if (Yangle > 360.0) Yangle -= 360.0;
	glutPostRedisplay();
}
void spinDisplayReverse() {
	
	Yangle -= spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
	prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
	std::cout << Yangle << std::endl;
	if (Yangle > 360.0) Yangle -= 360.0;
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
				glutIdleFunc(spinDisplay);
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
				glutIdleFunc(spinDisplayReverse);
			
			break;
			
		default:
			break;
	}
}

void keyInputSphere(unsigned char key){
	switch (key)
	{
		case 27:
			exit(0);
			break;
		case 'R':
			R += 0.5;
			glutPostRedisplay();
			break;
		case 'r':
			if (R > 1) R-=0.5;
			glutPostRedisplay();
			break;
		case 'Q':
			q += 1;
			glutPostRedisplay();
			break;
		case 'q':
			if (q > 1) q -= 1;
			glutPostRedisplay();
			break;
		case 'P':
			p += 1;
			glutPostRedisplay();
			break;
		case 'p':
			if (p > 3) p -= 1;
			glutPostRedisplay();
			break;
		case 'W':
			w = 0;
			glutPostRedisplay();
			break;
		case 'w':
			w = 1;
			glutPostRedisplay();
			break;
		case 'x':
			Xangle += 5.0;
			if (Xangle > 360.0) Xangle -= 360.0;
			glutPostRedisplay();
			break;
		case 'X':
			Xangle -= 5.0;
			if (Xangle < 0.0) Xangle += 360.0;
			glutPostRedisplay();
			break;
		case 'y':
			Yangle += 5.0;
			if (Yangle > 360.0) Yangle -= 360.0;
			glutPostRedisplay();
			break;
		case 'Y':
			Yangle -= 5.0;
			if (Yangle < 0.0) Yangle += 360.0;
			glutPostRedisplay();
			break;
		case 'z':
			Zangle += 5.0;
			if (Zangle > 360.0) Zangle -= 360.0;
			glutPostRedisplay();
			break;
		case 'Z':
			Zangle -= 5.0;
			if (Zangle < 0.0) Zangle += 360.0;
			glutPostRedisplay();
			break;
		case 'O':
			offset += 1;
			glutPostRedisplay();
			break;
		case 'o':
			offset -= 1;
			glutPostRedisplay();
			break;
		case ' ':
			glutIdleFunc(NULL);
			break;
		default:
			break;
	}
}

void keyInputHelix(unsigned char key){
	switch (key)
	{
		case 27:
			exit(0);
			break;
		case 'R':
			R += 0.5;
			glutPostRedisplay();
			break;
		case 'r':
			if (R > 1) R -= 0.5;
			glutPostRedisplay();
			break;
		case 'H':
			h += 0.5;
			glutPostRedisplay();
			break;
		case 'h':
			if (h > 1) h -= 0.5;
			glutPostRedisplay();
			break;
		case 'N':
			n += 1;
			glutPostRedisplay();
			break;
		case 'n':
			if (n > 2) n -= 1;
			glutPostRedisplay();
			break;
		case 'T':
			t += 1;
			glutPostRedisplay();
			break;
		case 't':
			if (t > 3) t -= 1;
			glutPostRedisplay();
			break;
		case 'x':
			Xangle += 5.0;
			if (Xangle > 360.0) Xangle -= 360.0;
			glutPostRedisplay();
			break;
		case 'X':
			Xangle -= 5.0;
			if (Xangle < 0.0) Xangle += 360.0;
			glutPostRedisplay();
			break;
		case 'y':
			Yangle += 5.0;
			if (Yangle > 360.0) Yangle -= 360.0;
			glutPostRedisplay();
			break;
		case 'Y':
			Yangle -= 5.0;
			if (Yangle < 0.0) Yangle += 360.0;
			glutPostRedisplay();
			break;
		case 'z':
			Zangle += 5.0;
			if (Zangle > 360.0) Zangle -= 360.0;
			glutPostRedisplay();
			break;
		case 'Z':
			Zangle -= 5.0;
			if (Zangle < 0.0) Zangle += 360.0;
			glutPostRedisplay();
			break;
		case 'O':
			offset += 1;
			glutPostRedisplay();
			break;
		case 'o':
			offset -= 1;
			glutPostRedisplay();
			break;
		case ' ':
			glutIdleFunc(NULL);
			break;
		default:
			break;
	}
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	if(shape == 1){
		keyInputSphere(key);
	}else{
		keyInputHelix(key);
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press '1' for Sphere and '0' for Helix:" << std::endl;
	std::cout << "Press R/r to increase/decrease the radius.\n"
	<<"Press P/p to increase/decrease the number of longitudinal slices.\n"
	<<"Press Q/q to increase/decrease the number of latitudinal slices.\n"
	<<"Press W/w to get a wireframe/filled shape.\n"
	<<"Press H/h to increase/decrease the pitch (height).\n"
	<<"Press N/n to increase/decrease the number of vertices.\n"
	<<"Press T/t to increase/decrease the number of turns.\n"
	<<"Press x, X, y, Y, z, Z to turn the Shape." << std::endl;
	std::cin >> shape;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);
	
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hemisphere.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouse);
	glewExperimental = GL_TRUE;
	glewInit();
	
	setup();
	
	glutMainLoop();
}
