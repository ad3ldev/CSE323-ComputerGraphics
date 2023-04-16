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
static int rotate = 0;
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

static std::vector<std::vector<float>> all_points = {
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

static std::vector<float> letters;

// Vertex color vectors for the triangle.
static float colors[1074];

static unsigned int vao[1];
static unsigned int buffer[1];
// End globals.

// DDA Algorithm.
std::vector<float> DDA(int x0, int y0, int x1, int y1){
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
std::vector<float> bresenham(int x0, int y0, int x1, int y1){
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
void init_offset(std::vector<std::vector<float>> &letters){
	int x_offset = 10;
	int y_offset = 75;
	for (int i = 0; i < NAME; i++){
		for(int j = 0; j < letters[i].size(); j+=3 ){
			letters[i][j] = letters[i][j] + x_offset;
			letters[i][j+1] = letters[i][j+1] + y_offset;
			letters[i][j+2] = letters[i][j+2];
		}
		x_offset += 10;
	}
	x_offset = 10;
	y_offset = 50;
	for (int i = NAME; i < 2*NAME; i++){
		for(int j = 0; j < letters[i].size(); j+=3 ){
			letters[i][j] = letters[i][j] + x_offset;
			letters[i][j+1] = letters[i][j+1] + y_offset;
			letters[i][j+2] = letters[i][j+2];
		}
		x_offset += 10;
	}
}
void init_letters(){
	std::vector<std::vector<float>> temp_letters;
	
	temp_letters.push_back(generate_points_bresenham(all_points[A]));
	temp_letters.push_back(generate_points_bresenham(all_points[B]));
	temp_letters.push_back(generate_points_bresenham(all_points[D]));
	temp_letters.push_back(generate_points_bresenham(all_points[E]));
	temp_letters.push_back(generate_points_bresenham(all_points[L]));
	temp_letters.push_back(generate_points_bresenham(all_points[R]));
	temp_letters.push_back(generate_points_bresenham(all_points[A]));
	temp_letters.push_back(generate_points_bresenham(all_points[H]));
	temp_letters.push_back(generate_points_bresenham(all_points[M]));
	temp_letters.push_back(generate_points_bresenham(all_points[A]));
	temp_letters.push_back(generate_points_bresenham(all_points[N]));
	
	temp_letters.push_back(generate_points_dda(all_points[A]));
	temp_letters.push_back(generate_points_dda(all_points[B]));
	temp_letters.push_back(generate_points_dda(all_points[D]));
	temp_letters.push_back(generate_points_dda(all_points[E]));
	temp_letters.push_back(generate_points_dda(all_points[L]));
	temp_letters.push_back(generate_points_dda(all_points[R]));
	temp_letters.push_back(generate_points_dda(all_points[A]));
	temp_letters.push_back(generate_points_dda(all_points[H]));
	temp_letters.push_back(generate_points_dda(all_points[M]));
	temp_letters.push_back(generate_points_dda(all_points[A]));
	temp_letters.push_back(generate_points_dda(all_points[N]));
	
	init_offset(temp_letters);
	for(auto letter: temp_letters){
		for (auto point : letter) {
			letters.push_back(point);
		}
	}
}
void init_colors(){
	float color = 107.4/1074.0;
	double sub_division = 3.0/1074.0;
	for(int i = 0; i < 1074; i += 3){
		colors[i] = color;
		colors[i+1] = 0;
		colors[i+2] = 0;
		color += sub_division;
	}
}

void animate(int value)
{
	glutPostRedisplay();
	glutTimerFunc(value, animate, value);
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0);

	float* bufferData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	
	float tempColors[1074];
	int colors_size =sizeof(colors) / sizeof(float);
	
	for (int i = 0; i < colors_size ; i++)
		tempColors[i] = bufferData[letters.size() + i];
	float first = tempColors[colors_size-3];
	for (int i = colors_size-3; i > 0; i-=3){
		tempColors[i] = tempColors[i-3];
	}
	tempColors[0] = first;
	for (int i = 0; i < colors_size; i+=3)
		bufferData[letters.size() + i] = tempColors[i];
		
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_POINTS, 0, (GLsizei)letters.size());
	glutSwapBuffers();
	glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	glGenVertexArrays(1, vao);
	
	glBindVertexArray(vao[0]);
	
	glGenBuffers(1, buffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, letters.size() * sizeof(float) + sizeof(colors), NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, letters.size() * sizeof(float), letters.data());
	glBufferSubData(GL_ARRAY_BUFFER, letters.size() * sizeof(float), sizeof(colors), colors);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glColorPointer(3, GL_FLOAT, 0, (void *)(letters.size() * sizeof(float)));
	int value = 3;
	glutTimerFunc(value, animate, value);
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

// Main routine.
int main(int argc, char **argv)
{
	init_letters();
	init_colors();
	
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
