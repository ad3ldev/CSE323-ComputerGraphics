#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define A 	0
#define B 	1
#define D 	2
#define E 	3
#define	L	4
#define R	5
#define H	6
#define M	7
#define N	8

#define NAME	11
// Begin globals.

static std::vector<float> a_points = {
	0,0,
	0,6,
	
	0,6,
	4,8,
	
	4,8,
	8,6,
	
	8,6,
	8,0,
	
	0,4,
	8,4,
};
static std::vector<float> b_points = {
	0,0,
	0,8,
	
	0,8,
	5,8,
	
	5,8,
	8,7,
	
	8,7,
	8,5,
	
	8,5,
	5,4,
	
	5,4,
	8,3,
	
	8,3,
	8,1,
	
	8,1,
	5,0,
	
	5,0,
	0,0,
	
	0,4,
	5,4,
};
static std::vector<float> d_points = {
	0,0,
	0,8,
	
	0,8,
	5,8,
	
	5,8,
	8,6,
	
	8,6,
	8,2,
	
	8,2,
	5,0,
	
	5,0,
	0,0,
	
};
static std::vector<float> e_points = {
	0,0,
	0,8,
	
	0,0,
	8,0,
	
	0,4,
	8,4,
	
	0,8,
	8,8,
	
};
static std::vector<float> l_points = {
	0,0,
	0,8,

	0,0,
	8,0,
};
static std::vector<float> r_points = {
	0,0,
	0,8,
	
	0,8,
	5,8,
	
	5,8,
	8,7,
	
	8,7,
	8,5,
	
	8,5,
	5,4,
	
	5,4,
	8,3,
	
	8,3,
	8,0,
	
	0,4,
	5,4,
};
static std::vector<float> h_points = {
	0,0,
	0,8,
	
	8,0,
	8,8,
	
	0,4,
	8,4,
	
};
static std::vector<float> m_points = {
	0,0,
	0,8,
	
	0,8,
	4,4,
	
	4,4,
	8,8,
	
	8,8,
	8,0,
};
static std::vector<float> n_points = {
	0,0,
	0,8,
	0,8,
	8,0,
	8,0,
	8,8,
};

static std::vector<std::vector<float>> points = {
	a_points,
	b_points,
	d_points,
	e_points,
	l_points,
	r_points,
	h_points,
	m_points,
	n_points,
};

static std::vector<std::vector<float>> bre_name;
static std::vector<std::vector<float>> dda_name;

// Vertex color vectors for the triangle.
static float colors[] =
{
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0
};

// Triangle strip vertex indices in order.
static unsigned int stripIndices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };

static unsigned int buffer[22]; // Array of buffer ids.

static unsigned int vao[22]; // Array of VAO ids.
// End globals.

void turn_vector_to_array(std::vector<float> points, float * letter){
	unsigned long size = points.size();
	letter = (float *)malloc(sizeof(float) * size);
	
	for(auto i = 0; i < size; i++){
		letter[i] = points[i];
	}
}

// DDA Algorithm.
std::vector<float> DDA(int x0, int y0, int x1, int y1)
{
	std::vector<float> points;
	int dx = x1 - x0;
	int dy = y1 - y0;
	int steps, k;
	
	float x = x0;
	float y = y0;
	float x_inc, y_inc;
	
	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	
	x_inc = dx/(float)steps;
	y_inc = dy/(float)steps;
	
	points.push_back(round(x));
	points.push_back(round(y));
	points.push_back(0);
	
	for (k = 1; k <= steps; k++){
		x += x_inc;
		y += y_inc;
		
		points.push_back(round(x));
		points.push_back(round(y));
		points.push_back(0);
	}
	return points;
}

// Bresenham Algorithm.
std::vector<float> bresenham(int x0, int y0, int x1, int y1)
{
	std::vector<float> points;
	int dx = x1 - x0;
	int dy = y1 - y0;
	int abs_dx = abs(dx);
	int abs_dy = abs(dy);
	
	int x = x0;
	int y = y0;
	
	points.push_back(x);
	points.push_back(y);
	points.push_back(0);
	
	if(abs_dx > abs_dy){
		int p = 2 * abs_dy - abs_dx;
		for(int i = 0; i < abs_dx; i++){
			x = dx < 0 ? x-1 : x+1;
			if(p < 0){
				p = p + 2*abs_dy;
			}else{
				y = dy < 0 ? y -1: y+1;
				p = p + (2*abs_dy - 2*abs_dx);
			}
			points.push_back(x);
			points.push_back(y);
			points.push_back(0);
		}
	}else{
		int p = 2 * abs_dx - abs_dy;
		for(int i = 0; i < abs_dy; i++){
			y = dy < 0 ? y -1: y+1;
			if(p < 0){
				p = p + 2*abs_dx;
			}else{
				x = dx < 0 ? x-1 : x+1;
				p = p + (2*abs_dx - 2*abs_dy);
			}
			points.push_back(x);
			points.push_back(y);
			points.push_back(0);
		}

	}
	return points;
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glEnd();
	glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 125, 0.0, 125, -1.0, 1.0);
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
		default:
			break;
	}
}

std::vector<float> generate_points_dda(std::vector<float> letter){
	std::vector<std::vector<float>> lines;
	for(int i = 0; i < letter.size(); i+=4){
		std::vector<float> line = DDA(letter[i], letter[i+1], letter[i+2], letter[i+3]);
		lines.push_back(line);
	}
	std::vector<float> final_points;
	for (auto line : lines) {
		for (auto point : line) {
			final_points.push_back(point);
		}
	}
	return final_points;
}
std::vector<float> generate_points_bresenham(std::vector<float> letter){
	std::vector<std::vector<float>> lines;
	for(int i = 0; i < letter.size(); i+=4){
		std::vector<float> line = bresenham(letter[i], letter[i+1], letter[i+2], letter[i+3]);
		lines.push_back(line);
	}
	std::vector<float> final_points;
	for (auto line : lines) {
		for (auto point : line) {
			final_points.push_back(point);
		}
	}
	return final_points;
}

void init_letters(){
	bre_name.push_back(generate_points_bresenham(points[A]));
	bre_name.push_back(generate_points_bresenham(points[B]));
	bre_name.push_back(generate_points_bresenham(points[D]));
	bre_name.push_back(generate_points_bresenham(points[E]));
	bre_name.push_back(generate_points_bresenham(points[L]));
	bre_name.push_back(generate_points_bresenham(points[R]));
	bre_name.push_back(generate_points_bresenham(points[A]));
	bre_name.push_back(generate_points_bresenham(points[H]));
	bre_name.push_back(generate_points_bresenham(points[M]));
	bre_name.push_back(generate_points_bresenham(points[A]));
	bre_name.push_back(generate_points_bresenham(points[N]));
	
	dda_name.push_back(generate_points_dda(points[A]));
	dda_name.push_back(generate_points_dda(points[B]));
	dda_name.push_back(generate_points_dda(points[D]));
	dda_name.push_back(generate_points_dda(points[E]));
	dda_name.push_back(generate_points_dda(points[L]));
	dda_name.push_back(generate_points_dda(points[R]));
	dda_name.push_back(generate_points_dda(points[A]));
	dda_name.push_back(generate_points_dda(points[H]));
	dda_name.push_back(generate_points_dda(points[M]));
	dda_name.push_back(generate_points_dda(points[A]));
	dda_name.push_back(generate_points_dda(points[N]));
}

// Main routine.
int main(int argc, char **argv)
{
	init_letters();
	
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("My First Name");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	setup();
	
	glutMainLoop();
}
