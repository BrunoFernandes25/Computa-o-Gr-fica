#include <stdlib.h>
#include "tinyxml2.cpp"
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <algorithm>
#include <tuple>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <IL/il.h>

#define NULL nullptr
using namespace std;
using namespace tinyxml2;

float width = 0.0, height = 0.0;
float posX = 0.0, posY = 0.0, posZ = 0.0, laX = 0.0, laY = 0.0, laZ = 0.0, upX = 0.0, upY = 1.0, upZ = 0.0, fov = 0.0, near = 0.0, far = 0.0;

int timebase = 0, frame = 0;
int orbita = 1, eixo = 1;

GLuint buffers[16], buffersN[16], buffersT[16], vertexCount[16];

struct Coordenada {
	vector <float> c;
};

struct transformacao {
	string nome;
	float x;
	float y;
	float z;
	float angle = 0;
	float time = 0;
	string align;
	vector <Coordenada> coord;
	float current_time = 0;
	float t = 0;
};

struct cor {
	int f = 0;
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
};

struct iluminacao {
	string tipo;
	float pos[4];
	float dir[4];
	float cutoff;
};

struct objeto {
	vector <transformacao> transf;
	vector <float> model;
	vector <float> norm;
	vector <float> tex;
	cor color;
	GLuint tex_id = -1;
};

vector <objeto> objetos;
vector <iluminacao> luz;


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

tuple<vector<float>, vector<float>, vector<float>> readFile(char* f) {
	vector<float> pontos;
	vector<float> norm;
	vector<float> tex;
	ifstream file;

	file.open("..\\Fich3d\\" + (string)f);

	ifstream myfile("..\\Fich3d\\" + (string)f);

	if (file.is_open()) {
		int numLinhas;
		myfile >> numLinhas;

		float a, b, c;
		for (int i = 0; i < numLinhas; i++) {
			myfile >> a >> b >> c;
			pontos.push_back(a);
			pontos.push_back(b);
			pontos.push_back(c);
		}

		for (int i = numLinhas; i < 2 * numLinhas; i++) {
			myfile >> a >> b >> c;
			norm.push_back(a);
			norm.push_back(b);
			norm.push_back(c);
		}

		for (int i = 2 * numLinhas; i < 3 * numLinhas; i++) {
			myfile >> a >> b;
			tex.push_back(a);
			tex.push_back(b);
		}

		file.close();
	}
	return make_tuple(pontos, norm, tex);
}

void readgroup(XMLElement* group, vector <transformacao> vt) {
	objeto obj;
	vector <transformacao> backup = vt;
	transformacao t;

	for (; group != NULL; group = group->NextSiblingElement("group")) {
		vt = backup;

		XMLElement* transformation = group->FirstChildElement("transform");

		if (transformation) {

			for (XMLElement* n = transformation->FirstChildElement(); n != NULL; n = n->NextSiblingElement()) {
				string nome = string(n->Name());

				if (nome == "translate" && n->Attribute("time") == nullptr) {
					t.nome = nome;
					t.x = n->FloatAttribute("x");
					t.y = n->FloatAttribute("y");
					t.z = n->FloatAttribute("z");

					vt.push_back(t);
				}

				if (nome == "translate" && n->Attribute("time") != nullptr) {
					t.nome = "translate_A";
					t.time = n->FloatAttribute("time") * 1000;
					t.align = n->Attribute("align");

					t.coord = {};

					for (XMLElement* n0 = n->FirstChildElement("point"); n0 != NULL; n0 = n0->NextSiblingElement()) {
						Coordenada p;
						p.c.push_back(n0->FloatAttribute("x"));
						p.c.push_back(n0->FloatAttribute("y"));
						p.c.push_back(n0->FloatAttribute("z"));
						t.coord.push_back(p);
					}
					vt.push_back(t);
				}

				if (nome == "scale") {

					t.nome = nome;
					t.x = n->FloatAttribute("x");
					t.y = n->FloatAttribute("y");
					t.z = n->FloatAttribute("z");

					vt.push_back(t);
				}

				if (nome == "rotate" && n->Attribute("time") == nullptr) {
					t.nome = nome;
					t.angle = n->FloatAttribute("angle");
					t.x = n->FloatAttribute("x");
					t.y = n->FloatAttribute("y");
					t.z = n->FloatAttribute("z");

					vt.push_back(t);
				}

				if (nome == "rotate" && n->Attribute("time") != nullptr) {
					t.nome = "rotate_A";
					t.time = n->FloatAttribute("time") * 1000;
					t.x = n->FloatAttribute("x");
					t.y = n->FloatAttribute("y");
					t.z = n->FloatAttribute("z");

					vt.push_back(t);
				}

			}

		}

		XMLElement* model = group->FirstChildElement("models");

		if (model) {

			for (XMLElement* n = model->FirstChildElement("model"); n != NULL; n = n->NextSiblingElement()) {
				char* filename = (char*)n->Attribute("file");
				tuple<vector<float>, vector<float>, vector<float>> result = readFile(filename);
				vector <float> pontos = get<0>(result);
				vector <float> norm = get<1>(result);
				vector <float> tex = get<2>(result);

				obj.model = pontos;
				obj.norm = norm;
				obj.tex = tex;
				obj.transf = vt;

				XMLElement* color = n->FirstChildElement("color");
				if (color) {
					obj.color.f = 1;

					obj.color.diffuse[0] = color->FirstChildElement("diffuse")->FloatAttribute("R") / 255;
					obj.color.diffuse[1] = color->FirstChildElement("diffuse")->FloatAttribute("G") / 255;
					obj.color.diffuse[2] = color->FirstChildElement("diffuse")->FloatAttribute("B") / 255;
					obj.color.diffuse[3] = 1.0;

					obj.color.ambient[0] = color->FirstChildElement("ambient")->FloatAttribute("R") / 255;
					obj.color.ambient[1] = color->FirstChildElement("ambient")->FloatAttribute("G") / 255;
					obj.color.ambient[2] = color->FirstChildElement("ambient")->FloatAttribute("B") / 255;
					obj.color.ambient[3] = 1.0;

					obj.color.specular[0] = color->FirstChildElement("specular")->FloatAttribute("R") / 255;
					obj.color.specular[1] = color->FirstChildElement("specular")->FloatAttribute("G") / 255;
					obj.color.specular[2] = color->FirstChildElement("specular")->FloatAttribute("B") / 255;
					obj.color.specular[3] = 1.0;

					obj.color.emissive[0] = color->FirstChildElement("emissive")->FloatAttribute("R") / 255;
					obj.color.emissive[1] = color->FirstChildElement("emissive")->FloatAttribute("G") / 255;
					obj.color.emissive[2] = color->FirstChildElement("emissive")->FloatAttribute("B") / 255;
					obj.color.emissive[3] = 1.0;

					obj.color.shininess = color->FirstChildElement("shininess")->FloatAttribute("value");
				}

				XMLElement* texture = n->FirstChildElement("texture");
				if (texture) {
					string texfile = "..\\..\\test_files\\test_files_phase_4\\" + (string)texture->Attribute("file");
					const char* tex_file = texfile.c_str();

					unsigned int t, tw, th;
					unsigned char* texData;
					ilGenImages(1, &t);
					ilBindImage(t);

					ilLoadImage((ILstring)tex_file);
					tw = ilGetInteger(IL_IMAGE_WIDTH);
					th = ilGetInteger(IL_IMAGE_HEIGHT);
					ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
					texData = ilGetData();

					glGenTextures(1, &obj.tex_id);

					glBindTexture(GL_TEXTURE_2D, obj.tex_id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

				}

				objetos.push_back(obj);
			}

		}

		readgroup(group->FirstChildElement("group"), vt);
	}

}

void buildRotMatrix(float* x, float* y, float* z, float* m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void getCatmullRomPoint(float t, vector<float> p0, vector<float> p1, vector<float>  p2, vector<float> p3, float* pos, float* deriv) {

	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };

	for (int i = 0; i < 3; i++) {
		float A[4];
		float temp[] = { p0[i], p1[i], p2[i], p3[i] };

		// Compute A = M * P
		multMatrixVector((float*)m, temp, A);

		// Compute pos = T * A
		pos[i] = pow(t, 3) * A[0] + pow(t, 2) * A[1] + t * A[2] + A[3];

		// compute deriv = T' * A
		deriv[i] = 3 * pow(t, 2) * A[0] + 2 * t * A[1] + A[2];
	}
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, vector <Coordenada> coord) {
	const int POINT_COUNT = coord.size();

	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, coord[indices[0]].c, coord[indices[1]].c, coord[indices[2]].c, coord[indices[3]].c, pos, deriv);
}

void renderCatmullRomCurve(vector <Coordenada> coord) {
	// draw curve using line segments with GL_LINE_LOOP
	float pos[3], deriv[3];
	float LINE_SEGMENTS = 100;

	if (orbita == 1) {
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < LINE_SEGMENTS; i++) {
			getGlobalCatmullRomPoint(1 / LINE_SEGMENTS * i, pos, deriv, coord);
			glVertex3f(pos[0], pos[1], pos[2]);
		}
		glEnd();
	}
	else {
		for (int i = 0; i < LINE_SEGMENTS; i++) {
			getGlobalCatmullRomPoint(1 / LINE_SEGMENTS * i, pos, deriv, coord);
			glVertex3f(pos[0], pos[1], pos[2]);
		}
	}
}

void renderScene(void) {
	float fps;
	int time;
	char s[64];

	// clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	glLoadIdentity();
	gluLookAt(posX, posY, posZ,
		laX, laY, laZ,
		upX, upY, upZ);

	if (eixo == 1) {
		glDisable(GL_LIGHTING);
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
		glEnable(GL_LIGHTING);
	}

	for (int i = 0; i < luz.size(); i++) {

		if (luz[i].tipo == "point") {
			glLightfv(GL_LIGHT0 + i, GL_POSITION, luz[i].pos);
		}

		if (luz[i].tipo == "directional") {
			glLightfv(GL_LIGHT0 + i, GL_POSITION, luz[i].dir);
		}

		if (luz[i].tipo == "spot") {
			glLightfv(GL_LIGHT0 + i, GL_POSITION, luz[i].pos);
			glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, luz[i].dir);
			glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, luz[i].cutoff);
			glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, 0.0);
		}
	}

	for (int i = 0; i < objetos.size(); i++) {

		glPushMatrix();

		if (!objetos[i].color.f && objetos[i].tex_id == -1) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 1.0f, 1.0f);
			glPolygonMode(GL_FRONT, GL_LINE);
		}


		for (int j = 0; j < objetos[i].transf.size(); j++) {

			if (objetos[i].transf[j].nome == "translate") {
				glTranslatef(objetos[i].transf[j].x, objetos[i].transf[j].y, objetos[i].transf[j].z);
			}

			if (objetos[i].transf[j].nome == "translate_A") {

				float pos[3], deriv[3];
				renderCatmullRomCurve(objetos[i].transf[j].coord);
				getGlobalCatmullRomPoint(objetos[i].transf[j].t, pos, deriv, objetos[i].transf[j].coord);
				glTranslatef(pos[0], pos[1], pos[2]);

				if (objetos[i].transf[j].align == "true") {
					float x[3] = { deriv[0], deriv[1], deriv[2] };
					float y[3];
					float z[3];
					float m[16];
					float prev_y[3] = { 0,1,0 };

					normalize(x);
					cross(x, prev_y, z);
					normalize(z);
					cross(z, x, y);

					normalize(y);
					memcpy(prev_y, y, 3 * sizeof(float));

					buildRotMatrix(x, y, z, m);
					glMultMatrixf(m);
				}

				float new_time = glutGet(GLUT_ELAPSED_TIME);
				float diff = new_time - objetos[i].transf[j].current_time;
				objetos[i].transf[j].t += diff / objetos[i].transf[j].time;
				objetos[i].transf[j].current_time = new_time;
			}

			if (objetos[i].transf[j].nome == "rotate") {
				glRotatef(objetos[i].transf[j].angle, objetos[i].transf[j].x, objetos[i].transf[j].y, objetos[i].transf[j].z);
			}

			if (objetos[i].transf[j].nome == "rotate_A") {
				glRotatef(glutGet(GLUT_ELAPSED_TIME) * 360 / objetos[i].transf[j].time, objetos[i].transf[j].x, objetos[i].transf[j].y, objetos[i].transf[j].z);
			}

			if (objetos[i].transf[j].nome == "scale") {
				glScalef(objetos[i].transf[j].x, objetos[i].transf[j].y, objetos[i].transf[j].z);
			}
		}

		if (objetos[i].color.f) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, objetos[i].color.diffuse);
			glMaterialfv(GL_FRONT, GL_AMBIENT, objetos[i].color.ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, objetos[i].color.specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, objetos[i].color.emissive);
			glMaterialf(GL_FRONT, GL_SHININESS, objetos[i].color.shininess);
		}

		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffersN[i]);
		glNormalPointer(GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffersT[i]);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);

		glBindTexture(GL_TEXTURE_2D, objetos[i].tex_id);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount[i]);
		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();
		
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame * 1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	}

	glutSwapBuffers();
}


// ler o ficheiro xml
void readxml(XMLElement* world) {
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

	XMLElement* lights = world->FirstChildElement("lights");
	if (lights) {
		for (XMLElement* light = lights->FirstChildElement(); light != NULL; light = light->NextSiblingElement()) {
			iluminacao aux;
			aux.tipo = light->Attribute("type");
			if (aux.tipo == "point") {
				aux.pos[0] = light->FloatAttribute("posx");
				aux.pos[1] = light->FloatAttribute("posy");
				aux.pos[2] = light->FloatAttribute("posz");
				aux.pos[3] = 1.0;
			}

			if (aux.tipo == "directional") {
				aux.dir[0] = light->FloatAttribute("dirx");
				aux.dir[1] = light->FloatAttribute("diry");
				aux.dir[2] = light->FloatAttribute("dirz");
				aux.dir[3] = 0.0;
			}

			if (aux.tipo == "spot") {
				aux.pos[0] = light->FloatAttribute("posx");
				aux.pos[1] = light->FloatAttribute("posy");
				aux.pos[2] = light->FloatAttribute("posz");
				aux.pos[3] = 1.0;
				aux.dir[0] = light->FloatAttribute("dirx");
				aux.dir[1] = light->FloatAttribute("diry");
				aux.dir[2] = light->FloatAttribute("dirz");
				aux.dir[3] = 1.0;
				aux.cutoff = light->FloatAttribute("cutoff");
			}

			luz.push_back(aux);
		}
	}

	XMLElement* group = world->FirstChildElement("group");

	vector <transformacao> vt;
	readgroup(group, vt);

}

void processKeys(unsigned char key, int x, int y) {
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
	case 'o':
		if (orbita == 1) orbita = 0;
		else orbita = 1;
		break;
	case 'e':
		if (eixo == 1) eixo = 0;
		else eixo = 1;
		break;
	}
}

int main(int argc, char** argv) {

	int opcao;
	const char* ficheiro;
	cout << " ___________________________________________________ MENU __________________________________________________\n";
	cout << "|                                                                                                           |\n";
	cout << "|                                  Escolha o ficheiro que pretende testar:                                  |\n";
	cout << "|                                                                                                           |\n";
	cout << "|       Fase 1:                 Fase 2:                  Fase 3:                   Fase 4:                  |\n";
	cout << "|                                                                                                           |\n";
	cout << "|  1 - teste_1_1.xml       6 - test_2_1.xml        11 - test_3_1.xml         16 - test_4_1.xml              |\n";
	cout << "|  2 - teste_1_2.xml       7 - test_2_2.xml        12 - test_3_2.xml         17 - test_4_2.xml              |\n";
	cout << "|  3 - teste_1_3.xml       8 - test_2_3.xml        13 - jupiter.xml          18 - test_4_3.xml              |\n";
	cout << "|  4 - teste_1_4.xml       9 - test_2_4.xml        14 - saturno.xml          19 - test_4_4.xml              |\n";
	cout << "|  5 - teste_1_5.xml      10 - sistema_solar.xml   15 - sistema_solar2.xml   20 - test_4_5.xml              |\n";
	cout << "|                                                                            21 - test_4_6.xml              |\n";
	cout << "|                                                                            22 - jupiter.xml               |\n";
	cout << "|                                                                            23 - boneco_neve.xml           |\n";
	cout << "|                                                                            24 - sistema_solar3.xml        |\n";
	cout << "|___________________________________________________________________________________________________________|\n";


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
	else if (opcao == 11)
		ficheiro = "..\\..\\test_files\\test_files_phase_3\\test_3_1.xml";
	else if (opcao == 12)
		ficheiro = "..\\..\\test_files\\test_files_phase_3\\test_3_2.xml";
	else if (opcao == 13)
		ficheiro = "..\\..\\test_files\\test_files_phase_3\\jupiter.xml";
	else if (opcao == 14)
		ficheiro = "..\\..\\test_files\\test_files_phase_3\\saturno.xml";
	else if (opcao == 15)
		ficheiro = "..\\..\\test_files\\test_files_phase_3\\sistema_solar2.xml";
	else if (opcao == 16)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\test_4_1.xml";
	else if (opcao == 17)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\test_4_2.xml";
	else if (opcao == 18)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\test_4_3.xml";
	else if (opcao == 19)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\test_4_4.xml";
	else if (opcao == 20)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\test_4_5.xml";
	else if (opcao == 21)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\test_4_6.xml";
	else if (opcao == 22)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\jupiter.xml";
	else if (opcao == 23)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\boneco_neve.xml";
	else if (opcao == 24)
		ficheiro = "..\\..\\test_files\\test_files_phase_4\\sistema_solar3.xml";

	else cout << "Ficheiro de teste nao existe\n";

	XMLDocument doc;
	doc.LoadFile(ficheiro);

	XMLElement* world = doc.FirstChildElement("world");
	
	XMLElement* window = world->FirstChildElement("window");
	width = window->IntAttribute("width");
	height = window->IntAttribute("height");
	
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("CG@DI-UM");
	
	glEnable(GL_TEXTURE_2D);

	glewInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilInit();

	readxml(world);

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(processKeys);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glGenBuffers(objetos.size(), buffers);
	glGenBuffers(objetos.size(), buffersN);
	glGenBuffers(objetos.size(), buffersT);

	for (int i = 0; i < objetos.size(); i++) {
		vertexCount[i] = objetos[i].model.size() / 3;
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, objetos[i].model.size() * sizeof(float), objetos[i].model.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffersN[i]);
		glBufferData(GL_ARRAY_BUFFER, objetos[i].norm.size() * sizeof(float), objetos[i].norm.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffersT[i]);
		glBufferData(GL_ARRAY_BUFFER, objetos[i].tex.size() * sizeof(float), objetos[i].tex.data(), GL_STATIC_DRAW);
	}

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_RESCALE_NORMAL);

	if (luz.size() > 0) {
		// init
		float dark[4] = { 0.2, 0.2, 0.2, 1.0 };
		float white[4] = { 1.0, 1.0, 1.0, 1.0 };
		float amb[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		// controls global ambient light
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
		glEnable(GL_LIGHT0);

		for (int i = 0; i < luz.size(); i++) {
			if (i != 0) glEnable(GL_LIGHT0 + i);

			// light colors
			glLightfv(GL_LIGHT0 + i, GL_AMBIENT, dark);
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, white);
			glLightfv(GL_LIGHT0 + i, GL_SPECULAR, white);
		}
	}

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}