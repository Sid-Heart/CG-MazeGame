#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <cstring>
#include <stdlib.h>
using namespace std;

const int Height = 10, Width = 10;
const float w = 500, h = 500, o = 10;
const float ww = w - 2 * o, hh = h - 2 * o;
int posx, posy, endx, endy;
void DFS(int i, int j);

struct Cell {
	bool visited;	// has this cell visited for making maze
	bool road[4];// is each four directions of cells(up, down, right, left) connected to this cell

	Cell() {
		visited = false;
		road[0] = false;
		road[1] = false;
		road[2] = false;
		road[3] = false;
	}
} Node[Height][Width];

void createMaze() {
	posy = 0;
	posx = rand() % Width;
	endy = Height - 1;
	endx = rand() % Width;
	Node[0][posx].road[0] = Node[Height - 1][endx].road[1] = true;
	DFS(rand() % Height, rand() % Width);
	for (int i = 0; i < Height; ++i)
		for (int j = 0; j < Width; ++j)
			Node[i][j].visited = false;
}

void DrawCells() {

	for (int i = 0; i < Height; ++i) {
		for (int j = 0; j < Width; ++j) {
			float cx = (2 * (j) + 1) * ww / Width / 2 + o, cy = (2
					* (Height - i - 1) + 1) * hh / Height / 2 + o, dx = ww
					/ Width / 2, dy = hh / Height / 2;

			if (Node[i][j].visited) {
				glColor3f(205 / 255.0, 133 / 255.0, 63 / 255.0);
				glBegin(GL_POLYGON);
				glVertex2f(cx - dx, cy + dy);
				glVertex2f(cx + dx, cy + dy);
				glVertex2f(cx + dx, cy - dy);
				glVertex2f(cx - dx, cy - dy);
				glEnd();
			}

			if (i == posy && j == posx) {
				glColor3f(139 / 255.0, 69 / 255.0, 19 / 255.0);
				glPointSize(min(dx, dy));
				glBegin(GL_POINTS);
				glVertex2f(cx, cy);
				glEnd();
			}

			glLineWidth(2);
			if (!Node[i][j].road[0]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy + dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy + dy);
				glEnd();
			}
			if (!Node[i][j].road[1]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy - dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy - dy);
				glEnd();
			}
			if (!Node[i][j].road[2]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy - dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy + dy);
				glEnd();
			}
			if (!Node[i][j].road[3]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy - dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy + dy);
				glEnd();
			}
		}
		cerr << endl;
	}
}

void DFS(int i, int j) {
	if (Node[i][j].visited)
		return;
	else {
		Node[i][j].visited = true;
		srand(time(NULL) * i * j);
		int i1 = rand() % 100;
		cerr << "i1=" << i1;
		for (int j1 = 0; j1 < 4; j1++) {
			if (i + 1 < Height && !Node[i + 1][j].visited && i1 % 4 == 0) {
				Node[i][j].road[1] = true;
				Node[i + 1][j].road[0] = true;
				DFS(i + 1, j);
			}
			if (i - 1 >= 0 && !Node[i - 1][j].visited && i1 % 4 == 1) {
				Node[i][j].road[0] = true;
				Node[i - 1][j].road[1] = true;
				DFS(i - 1, j);
			}
			if (j + 1 < Width && !Node[i][j + 1].visited && i1 % 4 == 2) {
				Node[i][j].road[2] = true;
				Node[i][j + 1].road[3] = true;
				DFS(i, j + 1);
			}
			if (j - 1 >= 0 && !Node[i][j - 1].visited && i1 % 4 == 3) {
				Node[i][j].road[3] = true;
				Node[i][j - 1].road[2] = true;
				DFS(i, j - 1);
			}
			i1++;
		}
	}
}

void init() {

	createMaze();

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(255 / 255.0, 228 / 255.0, 181 / 255.0, 1);
	glColor3f(0, 0, 0);
	DrawCells();
	glutSwapBuffers();
}

void idle() {
	glutPostRedisplay();
}

void keyboard(unsigned char ch, int x, int y) {
	Node[posy][posx].visited = true;
	switch (ch) {
	case 'W':
	case 'w':
		if (Node[posy][posx].road[0] && posy > 0
				&& !Node[posy - 1][posx].visited)
			posy--;
		break;

	case 'S':
	case 's':
		if (Node[posy][posx].road[1] && posy < Height - 1
				&& !Node[posy + 1][posx].visited)
			posy++;
		break;

	case 'A':
	case 'a':
		if (Node[posy][posx].road[3] && posx > 0
				&& !Node[posy][posx - 1].visited)
			posx--;
		break;

	case 'D':
	case 'd':
		if (Node[posy][posx].road[2] && posx < Width - 1
				&& !Node[posy][posx + 1].visited)
			posx++;
		break;
	}
	if (posx == endx && posy == endy) {
		glutTimerFunc(1000, exit, 0);
	}
}

void exit() {
	exit(0);
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("MAZE GAME");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
}
