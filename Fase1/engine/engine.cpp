#include <stdlib.h>
#include "tinyxml2.cpp"
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>

#define NULL nullptr
using namespace std;

float width = 0.0, height = 0.0;
float posX = 0.0, posY = 0.0, posZ = 0.0, laX = 0.0, laY = 0.0, laZ = 0.0, upX = 0.0, upY = 1.0, upZ = 0.0, fov=0.0, near=0.0, far=0.0;


vector <float> Pontos;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(fov, ratio, near, far);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void readFile(const char* f) {
	ifstream file;
	file.open(f);

	ifstream myfile(f);

	if (file.is_open()) {
		float a, b, c;
		while (myfile >> a >> b >> c) {
			Pontos.push_back(a);
			Pontos.push_back(b);
			Pontos.push_back(c);
		}

		file.close();
	}

}

void renderScene(void)
{

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_LINE);

	// set camera
	glLoadIdentity();
	//para teapot, cubo, torus
	gluLookAt(posX,posY,posZ,
			  laX, laY,laZ,
			  upX, upY, upZ);
	
	// put axis drawing in here
    glBegin(GL_LINES);

    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);

    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);

    glEnd();

	// put axis drawing in here
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < Pontos.size(); i += 9) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(Pontos[i], Pontos[i+1], Pontos[i+2]);
		glVertex3f(Pontos[i+3], Pontos[i+4], Pontos[i+5]);
		glVertex3f(Pontos[i+6], Pontos[i+7], Pontos[i+8]);
	}

	glEnd();

	glutSwapBuffers();
}


// ler o ficheiro xml
void readxml(const char* source) {

	using namespace tinyxml2;
	XMLDocument doc;
	doc.LoadFile(source);

	XMLElement* world = doc.FirstChildElement("world");

	XMLElement* window = world->FirstChildElement("window");
	width = window->IntAttribute("width");
	height = window->IntAttribute("height");

	XMLElement* camera = world->FirstChildElement("camera");
	XMLElement* position = camera->FirstChildElement("position");
	posX = position->FloatAttribute("x");
	posY = position->FloatAttribute("y");
	posZ = position->FloatAttribute("z");

	XMLElement* lookat = camera->FirstChildElement("lookAt");
	laX = lookat->FloatAttribute("x");
	laY = lookat->FloatAttribute("y");
	laZ = lookat->FloatAttribute("z");

	XMLElement* up = camera->FirstChildElement("up");
	upX = up->FloatAttribute("x");
	upY = up->FloatAttribute("y");
	upZ = up->FloatAttribute("z");

	XMLElement* projection = camera->FirstChildElement("projection");
	fov = projection->FloatAttribute("fov");
	near = projection->FloatAttribute("near");
	far = projection->FloatAttribute("far");

	for (XMLElement* model = world->FirstChildElement("group")->FirstChildElement("models")->FirstChildElement("model"); model != NULL; model = model->NextSiblingElement()) {
		const char* filename = model->Attribute("file");
		readFile(filename);
	}

}


int main(int argc, char** argv) {

	int opcao;
	const char* ficheiro;
	cout << " __________________ MENU __________________\n";
	cout << "| Escolha o ficheiro que pretende testar:  |\n";
	cout << "|   1 - test_1_1.xml                       |\n";
	cout << "|   2 - test_1_2.xml                       |\n";
	cout << "|   3 - test_1_3.xml                       |\n";
	cout << "|   4 - test_1_4.xml                       |\n";
	cout << "|   5 - test_1_5.xml                       |\n";
	cout << "|__________________________________________|\n";

	cout << "Opcao: ";
	cin >> opcao;

	if (opcao == 1)
		ficheiro = "..\\..\\test_files\\test_files_phase_1\\test_1_1.xml";
	else if (opcao == 2)
		ficheiro = "..\\..\\test_files\\test_files_phase_1\\test_1_2.xml";
	else if (opcao == 3)
		ficheiro = "..\\..\\test_files\\test_files_phase_1\\test_1_3.xml";
	else if (opcao == 4)
		ficheiro = "..\\..\\test_files\\test_files_phase_1\\test_1_4.xml";
	else if (opcao == 5)
		ficheiro = "..\\..\\test_files\\test_files_phase_1\\test_1_5.xml";
	else {
		cout << "Ficheiro de teste nao existe\n";
	}

	readxml(ficheiro);
	

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}