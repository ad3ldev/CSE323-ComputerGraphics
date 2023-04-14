//////////////////////////////////////////////////////////////
// squareAnnulusAndTriangle.cpp
//
// This program draws a square annulus and a triangle -
// it illustrates the use of multiple vertex/color arrays.
//
// Interaction:
// Press the space bar to toggle between wireframe and filled.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

// Begin globals.
static float *names;
static unsigned long size;

std::vector<float> A;
std::vector<float> B;
std::vector<float> D;
std::vector<float> E;
std::vector<float> L;
std::vector<float> R;
std::vector<float> H;
std::vector<float> M;
std::vector<float> N;

std::vector<float> A2;
std::vector<float> B2;
std::vector<float> D2;
std::vector<float> E2;
std::vector<float> L2;
std::vector<float> R2;
std::vector<float> H2;
std::vector<float> M2;
std::vector<float> N2;

// Vertex color vectors for the triangle.
static float colors2[] =
{
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0
};

// Triangle strip vertex indices in order.
static unsigned int stripIndices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };

static unsigned int buffer[2]; // Array of buffer ids.

static unsigned int vao[2]; // Array of VAO ids.
// End globals.

// DDA Algorithm.
void DDA(std::vector<float> &points, int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps, k;
	
	float x = x1;
	float y = y1;
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
}

// Bresenham Algorithm.
void bresenham(std::vector<float> &points, int x0, int y0, int x1, int y1)
{
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
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for(int i = 0; i < size; ++i) glArrayElement(i);
	glEnd();
	glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, names);
	glColorPointer(3, GL_FLOAT, 0, colors2);
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

void letter_A(){
	bresenham(A, 0,0,0,6);
	bresenham(A, 0,6,4,8);
	bresenham(A, 4,8,8,6);
	bresenham(A, 8,6,8,0);
	bresenham(A, 0,4,8,4);
}
void letter_B(){
	bresenham(B, 0,0,0,8);
	bresenham(B, 0,8,5,8);
	bresenham(B, 5,8,8,7);
	bresenham(B, 8,7,8,5);
	bresenham(B, 8,5,5,4);
	bresenham(B, 5,4,8,3);
	bresenham(B, 8,3,8,1);
	bresenham(B, 8,1,5,0);
	bresenham(B, 5,0,0,0);
	bresenham(B, 0,4,5,4);
}
void letter_D(){
	bresenham(D, 0,0,0,8);
	bresenham(D, 0,8,5,8);
	bresenham(D, 5,8,8,6);
	bresenham(D, 8,6,8,2);
	bresenham(D, 8,2,5,0);
	bresenham(D, 5,0,0,0);
}
void letter_E(){
	bresenham(E, 0,0,0,8);
	bresenham(E, 0,0,8,0);
	bresenham(E, 0,4,8,4);
	bresenham(E, 0,8,8,8);
}
void letter_L(){
	bresenham(L, 0,0,0,8);
	bresenham(L, 0,0,8,0);
}
void letter_R(){
	bresenham(R, 0,0,0,8);
	bresenham(R, 0,8,5,8);
	bresenham(R, 5,8,8,7);
	bresenham(R, 8,7,8,5);
	bresenham(R, 8,5,5,4);
	bresenham(R, 5,4,8,3);
	bresenham(R, 8,3,8,0);
	bresenham(R, 0,4,5,4);
}
void letter_H(){
	bresenham(H, 0,0,0,8);
	bresenham(H, 8,0,8,8);
	bresenham(H, 0,4,8,4);
}
void letter_M(){
	bresenham(M, 0,0,0,8);
	bresenham(M, 0,8,4,4);
	bresenham(M, 4,4,8,8);
	bresenham(M, 8,8,8,0);
}
void letter_N(){
	bresenham(N, 0,0,0,8);
	bresenham(N, 0,8,8,0);
	bresenham(N, 8,0,8,8);
}

void letter_A2(){
	DDA(A2, 0,0,0,6);
	DDA(A2, 0,6,4,8);
	DDA(A2, 4,8,8,6);
	DDA(A2, 8,6,8,0);
	DDA(A2, 0,4,8,4);
}
void letter_B2(){
	DDA(B2, 0,0,0,8);
	DDA(B2, 0,8,5,8);
	DDA(B2, 5,8,8,7);
	DDA(B2, 8,7,8,5);
	DDA(B2, 8,5,5,4);
	DDA(B2, 5,4,8,3);
	DDA(B2, 8,3,8,1);
	DDA(B2, 8,1,5,0);
	DDA(B2, 5,0,0,0);
	DDA(B2, 0,4,5,4);
}
void letter_D2(){
	DDA(D2, 0,0,0,8);
	DDA(D2, 0,8,5,8);
	DDA(D2, 5,8,8,6);
	DDA(D2, 8,6,8,2);
	DDA(D2, 8,2,5,0);
	DDA(D2, 5,0,0,0);
}
void letter_E2(){
	DDA(E2, 0,0,0,8);
	DDA(E2, 0,0,8,0);
	DDA(E2, 0,4,8,4);
	DDA(E2, 0,8,8,8);
}
void letter_L2(){
	DDA(L2, 0,0,0,8);
	DDA(L2, 0,0,8,0);
}
void letter_R2(){
	DDA(R2, 0,0,0,8);
	DDA(R2, 0,8,5,8);
	DDA(R2, 5,8,8,7);
	DDA(R2, 8,7,8,5);
	DDA(R2, 8,5,5,4);
	DDA(R2, 5,4,8,3);
	DDA(R2, 8,3,8,0);
	DDA(R2, 0,4,5,4);
}
void letter_H2(){
	DDA(H2, 0,0,0,8);
	DDA(H2, 8,0,8,8);
	DDA(H2, 0,4,8,4);
}
void letter_M2(){
	DDA(M2, 0,0,0,8);
	DDA(M2, 0,8,4,4);
	DDA(M2, 4,4,8,8);
	DDA(M2, 8,8,8,0);
}
void letter_N2(){
	DDA(N2, 0,0,0,8);
	DDA(N2, 0,8,8,0);
	DDA(N2, 8,0,8,8);
}
void init_bre_letters(){
	letter_A();
	letter_B();
	letter_D();
	letter_E();
	letter_L();
	letter_R();
	letter_H();
	letter_M();
	letter_N();
}
void init_dda_letters(){
	letter_A2();
	letter_B2();
	letter_D2();
	letter_E2();
	letter_L2();
	letter_R2();
	letter_H2();
	letter_M2();
	letter_N2();
}
void init_letters_with_offset(){
	int j = 5;
	int y_offset = 75;
	for (int i = 0; i < A.size(); i+=3) {
		A[i] = j+A[i];
		A[i+1] = y_offset+A[i+1];
		A[i+2] = A[i+2];
	}
	j += 10;
	for (int i = 0; i < B.size(); i+=3) {
		B[i] = j+B[i];
		B[i+1] = y_offset+B[i+1];
		B[i+2] = B[i+2];
	}
	j += 10;
	for (int i = 0; i < D.size(); i+=3) {
		D[i] = j+D[i];
		D[i+1] = y_offset+D[i+1];
		D[i+2] = D[i+2];
	}
	j += 10;
	for (int i = 0; i < E.size(); i+=3) {
		E[i] = j+E[i];
		E[i+1] = y_offset+E[i+1];
		E[i+2] = E[i+2];
	}
	j += 10;
	for (int i = 0; i < L.size(); i+=3) {
		L[i] = j+L[i];
		L[i+1] = y_offset+L[i+1];
		L[i+2] = L[i+2];
	}
	j += 10;
	for (int i = 0; i < R.size(); i+=3) {
		R[i] = j+R[i];
		R[i+1] = y_offset+R[i+1];
		R[i+2] = R[i+2];
	}
	j += 10;
	for (int i = 0; i < A.size(); i+=3) {
		A[i] = j+A[i];
		A[i+1] = y_offset+A[i+1];
		A[i+2] = A[i+2];
	}
	j += 10;
	for (int i = 0; i < H.size(); i+=3) {
		H[i] = j+H[i];
		H[i+1] = y_offset+H[i+1];
		H[i+2] = H[i+2];
	}
	j += 10;
	for (int i = 0; i < M.size(); i+=3) {
		M[i] = j+M[i];
		M[i+1] = y_offset+M[i+1];
		M[i+2] = M[i+2];
	}
	j += 10;
	for (int i = 0; i < A.size(); i+=3) {
		A[i] = j+A[i];
		A[i+1] = y_offset+A[i+1];
		A[i+2] = A[i+2];
	}
	j += 10;
	for (int i = 0; i < N.size(); i+=3) {
		N[i] = j+N[i];
		N[i+1] = y_offset+N[i+1];
		N[i+2] = N[i+2];
	}
	
	j = 5;
	 y_offset = 50;
	for (int i = 0; i < A2.size(); i+=3) {
		A2[i] = j+A2[i];
		A2[i+1] = y_offset+A2[i+1];
		A2[i+2] = A2[i+2];;
	}
	j += 10;
	for (int i = 0; i < B2.size(); i+=3) {
		B2[i] = j+B2[i];
		B2[i+1] = y_offset+B2[i+1];
		B2[i+2] = B2[i+2];
	}
	j += 10;
	for (int i = 0; i < D2.size(); i+=3) {
		D2[i] = j+D2[i];
		D2[i+1] = y_offset+D2[i+1];
		D2[i+2] = D2[i+2];
	}
	j += 10;
	for (int i = 0; i < E2.size(); i+=3) {
		E2[i] = j+E2[i];
		E2[i+1] = y_offset+E2[i+1];
		E2[i+2] = E2[i+2];
	}
	j += 10;
	for (int i = 0; i < L2.size(); i+=3) {
		L2[i] = j+L2[i];
		L2[i+1] = y_offset+L2[i+1];
		L2[i+2] = L2[i+2];
	}
	j += 10;
	for (int i = 0; i < R2.size(); i+=3) {
		R2[i] = j+R2[i];
		R2[i+1] = y_offset+R2[i+1];
		R2[i+2] = R2[i+2];
	}
	j += 10;
	for (int i = 0; i < A2.size(); i+=3) {
		A2[i] = j+A2[i];
		A2[i+1] = y_offset+A2[i+1];
		A2[i+2] = A2[i+2];
	}
	j += 10;
	for (int i = 0; i < H2.size(); i+=3) {
		H2[i] = j+H2[i];
		H2[i+1] = y_offset+H2[i+1];
		H2[i+2] = H2[i+2];
	}
	j += 10;
	for (int i = 0; i < M2.size(); i+=3) {
		M2[i] = j+M2[i];
		M2[i+1] = y_offset+M2[i+1];
		M2[i+2] = M2[i+2];
	}
	j += 10;
	for (int i = 0; i < A2.size(); i+=3) {
		A2[i] = j+A2[i];
		A2[i+1] = y_offset+A2[i+1];
		A2[i+2] = A2[i+2];
	}
	j += 10;
	for (int i = 0; i < N2.size(); i+=3) {
		N2[i] = j+N2[i];
		N2[i+1] = y_offset+N2[i+1];
		N2[i+2] = N2[i+2];
	}
}
void for_loop_assign(int &i ,std::vector<float> X){
	for (auto X_point : X) {
		names[i] = X_point;
		i++;
	}
}
void init_pointer(){
	unsigned long bre_length = A.size()+B.size()+D.size()+E.size()+L.size()+R.size()+A.size()+H.size()+M.size()+A.size()+N.size();
	unsigned long dda_length = A2.size()+B2.size()+D2.size()+E2.size()+L2.size()+R2.size()+A2.size()+H2.size()+M2.size()+A2.size()+N2.size();
	size = bre_length + dda_length;
	names = (float *)malloc(sizeof(float)* (size));
	int i = 0;
	for_loop_assign(i, A);
	for_loop_assign(i, B);
	for_loop_assign(i, D);
	for_loop_assign(i, E);
	for_loop_assign(i, L);
	for_loop_assign(i, R);
	for_loop_assign(i, A);
	for_loop_assign(i, H);
	for_loop_assign(i, M);
	for_loop_assign(i, A);
	for_loop_assign(i, N);
	for_loop_assign(i, A2);
	for_loop_assign(i, B2);
	for_loop_assign(i, D2);
	for_loop_assign(i, E2);
	for_loop_assign(i, L2);
	for_loop_assign(i, R2);
	for_loop_assign(i, A2);
	for_loop_assign(i, H2);
	for_loop_assign(i, M2);
	for_loop_assign(i, A2);
	for_loop_assign(i, N2);
	
}

// Main routine.
int main(int argc, char **argv)
{
	
	init_bre_letters();
	init_dda_letters();
	init_letters_with_offset();
	init_pointer();
	
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
