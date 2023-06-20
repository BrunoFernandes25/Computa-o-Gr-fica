#include <stdlib.h>
#include "tinyxml2.cpp"
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
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
using namespace tinyxml2;

float width = 0.0, height = 0.0;
float posX = 0.0, posY = 0.0, posZ = 0.0, laX = 0.0, laY = 0.0, laZ = 0.0, upX = 0.0, upY = 1.0, upZ = 0.0, fov = 0.0, near = 0.0, far = 0.0;

struct transformacao {
	string nome;
	float x;
	float y;
	float z;
	float angle;
};

struct objeto {

	vector <transformacao> transf;
	vector <float> model;

};

vector <objeto> objetos;


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

vector <float> readFile(char* f) {
	vector <float> Pontos;
	ifstream file;

	file.open("..\\Fich3d\\" + (string)f);

	ifstream myfile("..\\Fich3d\\" + (string)f);

	if (file.is_open()) {

		float a, b, c;
		while (myfile >> a >> b >> c) {
			Pontos.push_back(a);
			Pontos.push_back(b);
			Pontos.push_back(c);
		}

		file.close();
	}

	return Pontos;
}

void readgroup(tinyxml2::XMLElement* group, vector <transformacao> vt) {

	objeto obj;
	vector <transformacao> backup = vt;
	transformacao t;

	for (; group != NULL; group = group->NextSiblingElement("group")) {
		vt = backup;

		XMLElement* transformation = group->FirstChildElement("transform");

		if (transformation) {

			XMLElement* n = transformation->FirstChildElement();

			for (; n != NULL; n = n->NextSiblingElement()) {
				string nome = string(n->Name());

				if (nome == "translate" || nome == "scale") {

					t.nome = nome;
					t.x = atof(n->Attribute("x"));
					t.y = atof(n->Attribute("y"));
					t.z = atof(n->Attribute("z"));
					t.angle = 0;

					vt.push_back(t);
				}

				if (nome == "rotate") {

					t.nome = nome;
					t.angle = atof(n->Attribute("angle"));
					t.x = atof(n->Attribute("x"));
					t.y = atof(n->Attribute("y"));
					t.z = atof(n->Attribute("z"));

					vt.push_back(t);
				}

			}

		}

		XMLElement* model = group->FirstChildElement("models");

		if (model) {

			XMLElement* n = model->FirstChildElement("model");

			for (; n != NULL; n = n->NextSiblingElement()) {
				char* filename = (char*)n->Attribute("file");
				obj.model = readFile(filename);

			}
			obj.transf = vt;

			objetos.push_back(obj);
		}

		readgroup(group->FirstChildElement("group"), vt);
	}

}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_LINE);

	// set camera
	glLoadIdentity();
	gluLookAt(posX, posY, posZ,
		laX, laY, laZ,
		upX, upY, upZ);

	// put axis drawing in here
	glBegin(GL_LINES);

	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);

	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);

	glEnd();

	glutPostRedisplay();

	// put axis drawing in here

	for (int i = 0; i < objetos.size(); i++) {

		glPushMatrix();

		for (int j = 0; j < objetos[i].transf.size(); j++) {

			if (objetos[i].transf[j].nome == "translate") {
				glTranslatef(objetos[i].transf[j].x, objetos[i].transf[j].y, objetos[i].transf[j].z);
			}

			if (objetos[i].transf[j].nome == "rotate") {
				glRotatef(objetos[i].transf[j].angle, objetos[i].transf[j].x, objetos[i].transf[j].y, objetos[i].transf[j].z);
			}

			if (objetos[i].transf[j].nome == "scale") {
				glScalef(objetos[i].transf[j].x, objetos[i].transf[j].y, objetos[i].transf[j].z);
			}

		}

		glBegin(GL_TRIANGLES);

		for (int j = 0; j < objetos[i].model.size(); j += 9) {
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(objetos[i].model[j], objetos[i].model[j + 1], objetos[i].model[j + 2]);
			glVertex3f(objetos[i].model[j + 3], objetos[i].model[j + 4], objetos[i].model[j + 5]);
			glVertex3f(objetos[i].model[j + 6], objetos[i].model[j + 7], objetos[i].model[j + 8]);
		}

		glEnd();
		glPopMatrix();
	}

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

	XMLElement* group = world->FirstChildElement("group");

	vector <transformacao> vt;
	readgroup(group, vt);

}


void moves(unsigned char key, int x, int y) { 
	switch (key) {
	case 'd':
		posX += 1;
		break;
	case 'a':
		posX -= 1;
		break;
	case 'w':
		posY += 1;
		break;
	case 's':
		posY -= 1;
		break;
	case 'z':
		posZ += 1;
		break;
	case 'x':
		posZ -= 1;
		break;
	case 'm':
		posX += 1;
		posZ += 1;
		break;
	case 'n':
		posX -= 1;
		posZ -= 1;
		break;
	}
}




int main(int argc, char** argv) {

	int opcao;
	const char* ficheiro;
	cout << " ______________________ MENU ______________________\n";
	cout << "|                                                  |\n";
	cout << "|    Escolha o ficheiro que pretende testar:       |\n";
	cout << "|                                                  |\n";
	cout << "|       Fase 1:                 Fase 2:            |\n";
	cout << "|   1 - teste_1_1.xml       6 - test_2_1.xml       |\n";
	cout << "|   2 - teste_1_2.xml       7 - test_2_2.xml       |\n";
	cout << "|   3 - teste_1_3.xml       8 - test_2_3.xml       |\n";
	cout << "|   4 - teste_1_4.xml       9 - test_2_4.xml       |\n";
	cout << "|   5 - teste_1_5.xml      10 - sistema_solar.xml  |\n";
	cout << "|__________________________________________________|\n";

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
	else if (opcao == 6)
		ficheiro = "..\\..\\test_files\\test_files_phase_2\\test_2_1.xml";
	else if (opcao == 7)
		ficheiro = "..\\..\\test_files\\test_files_phase_2\\test_2_2.xml";
	else if (opcao == 8)
		ficheiro = "..\\..\\test_files\\test_files_phase_2\\test_2_3.xml";
	else if (opcao == 9)
		ficheiro = "..\\..\\test_files\\test_files_phase_2\\test_2_4.xml";
	else if (opcao == 10)
		ficheiro = "..\\..\\test_files\\test_files_phase_2\\sistema_solar.xml";
	
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

	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(moves);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}