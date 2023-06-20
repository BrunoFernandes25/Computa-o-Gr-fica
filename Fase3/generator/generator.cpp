#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;

struct Coordenada{
	vector <float> c;
};


void wrtfile(string objeto, string name) {
    ofstream file("..\\engine\\Fich3d\\" + name);
    if (file.is_open()) {
        file << objeto << endl;
        file.close();
    }
    else {
        cout << "Nao foi possivel abrir o arquivo " << name << " para escrita." << endl;
    }
}

void drawplane(int length, int divisions, string name) {
    stringstream s;

    float init = -((float)length / 2.0f);
    float unidade = (float)length / (float)divisions;
    float x1, x2, z1, z2;
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            x1 = init + i * unidade;
            z1 = init + j * unidade;
            x2 = init + (i + 1) * unidade;
            z2 = init + (j + 1) * unidade;

            // 1 triangulo
            s << x1 << ' ' << 0 << ' ' << z1 << '\n';
            s << x1 << ' ' << 0 << ' ' << z2 << '\n';
            s << x2 << ' ' << 0 << ' ' << z2 << '\n';

            //2 triangulo
            s << x2 << ' ' << 0 << ' ' << z1 << '\n';
            s << x1 << ' ' << 0 << ' ' << z1 << '\n';
            s << x2 << ' ' << 0 << ' ' << z2 << '\n';
        }
    }
    wrtfile(s.str(), name);
}

void drawbox(int length, int divisions, string name) {
    stringstream s;

    float init = -((float)length / 2.0);
    float unidade = (float)length / (float)divisions;
    float x1, x2, y1, y2, z1, z2;
    float altura = (float)length / 2.0;

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            x1 = init + i * unidade;
            z1 = init + j * unidade;
            x2 = init + (i + 1) * unidade;
            z2 = init + (j + 1) * unidade;

            //face de cima
            // 1 triangulo
            s << x1 << ' ' << altura << ' ' << z1 << '\n';
            s << x1 << ' ' << altura << ' ' << z2 << '\n';
            s << x2 << ' ' << altura << ' ' << z2 << '\n';

            //2 triangulo
            s << x2 << ' ' << altura << ' ' << z1 << '\n';
            s << x1 << ' ' << altura << ' ' << z1 << '\n';
            s << x2 << ' ' << altura << ' ' << z2 << '\n';

            //face de baixo
            //1 triangulo
            s << x1 << ' ' << -altura << ' ' << z1 << '\n';
            s << x2 << ' ' << -altura << ' ' << z2 << '\n';
            s << x1 << ' ' << -altura << ' ' << z2 << '\n';

            //2 triangulo
            s << x2 << ' ' << -altura << ' ' << z1 << '\n';
            s << x2 << ' ' << -altura << ' ' << z2 << '\n';
            s << x1 << ' ' << -altura << ' ' << z1 << '\n';
        }
    }

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            x1 = init + i * unidade;
            y1 = init + j * unidade;
            x2 = init + (i + 1) * unidade;
            y2 = init + (j + 1) * unidade;

            //face da esquerda
            // 1 triangulo
            s << x1 << ' ' << y2 << ' ' << altura << '\n';
            s << x1 << ' ' << y1 << ' ' << altura << '\n';
            s << x2 << ' ' << y1 << ' ' << altura << '\n';

            //2 triangulo
            s << x2 << ' ' << y2 << ' ' << altura << '\n';
            s << x1 << ' ' << y2 << ' ' << altura << '\n';
            s << x2 << ' ' << y1 << ' ' << altura << '\n';

            //face da direita
            // 1 triangulo
            s << x1 << ' ' << y2 << ' ' << -altura << '\n';
            s << x2 << ' ' << y1 << ' ' << -altura << '\n';
            s << x1 << ' ' << y1 << ' ' << -altura << '\n';

            //2 triangulo
            s << x2 << ' ' << y2 << ' ' << -altura << '\n';
            s << x2 << ' ' << y1 << ' ' << -altura << '\n';
            s << x1 << ' ' << y2 << ' ' << -altura << '\n';
        }
    }

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            y1 = init + i * unidade;
            z1 = init + j * unidade;
            y2 = init + (i + 1) * unidade;
            z2 = init + (j + 1) * unidade;

            //face da frente
            // 1 triangulo
            s << altura << ' ' << y2 << ' ' << z2 << '\n';
            s << altura << ' ' << y1 << ' ' << z2 << '\n';
            s << altura << ' ' << y1 << ' ' << z1 << '\n';

            //2 triangulo
            s << altura << ' ' << y2 << ' ' << z1 << '\n';
            s << altura << ' ' << y2 << ' ' << z2 << '\n';
            s << altura << ' ' << y1 << ' ' << z1 << '\n';

            //face de tras
            // 1 triangulo
            s << -altura << ' ' << y2 << ' ' << z2 << '\n';
            s << -altura << ' ' << y1 << ' ' << z1 << '\n';
            s << -altura << ' ' << y1 << ' ' << z2 << '\n';

            //2 triangulo
            s << -altura << ' ' << y2 << ' ' << z1 << '\n';
            s << -altura << ' ' << y1 << ' ' << z1 << '\n';
            s << -altura << ' ' << y2 << ' ' << z2 << '\n';

        }
    }
    wrtfile(s.str(), name);
}

void drawcone(int radius, int height, int slices, int stacks, string name) {
    stringstream s;
    float x1, x2, z1, z2;
    float atualH = 0;
    float proxH;
    float atualR = radius;
    float proxR;

    //desenhar a base
    for (int i = 0; i < slices; i++) {
        x1 = radius * sin(i * (2 * M_PI / (float)slices));
        x2 = radius * sin((i + 1) * (2 * M_PI / (float)slices));
        z1 = radius * cos(i * (2 * M_PI / (float)slices));
        z2 = radius * cos((i + 1) * (2 * M_PI / (float)slices));

        s << 0 << ' ' << 0 << ' ' << 0 << '\n';
        s << x2 << ' ' << 0 << ' ' << z2 << '\n';
        s << x1 << ' ' << 0 << ' ' << z1 << '\n';
    }
    
    //desenhar a lateral
    float angulo = 0;
    for (int i = 0; i < stacks; i++) {
        proxH = atualH + (float)height / (float)stacks;
        proxR = atualR - (float)radius / (float)stacks;

        for(int j = 0; j<slices; j++) {

            //1 triangulo
            s << atualR * sin(angulo) << ' ' << atualH << ' ' << atualR * cos(angulo) << '\n';
            s << atualR * sin(angulo + (2 * M_PI / (float)slices)) << ' ' << atualH << ' ' << atualR * cos(angulo + (2 * M_PI / (float)slices)) << '\n';
            s << proxR * sin(angulo) << ' ' << proxH << ' ' << proxR * cos(angulo) << '\n';

            //2 triangulo
            s << proxR * sin(angulo) << ' ' << proxH << ' ' << proxR * cos(angulo) << '\n';
            s << atualR * sin(angulo + (2 * M_PI / (float)slices)) << ' ' << atualH << ' ' << atualR * cos(angulo + (2 * M_PI / (float)slices)) << '\n';
            s << proxR * sin(angulo + (2 * M_PI / (float)slices))<< ' ' << proxH << ' ' << proxR * cos(angulo + (2 * M_PI / (float)slices)) << '\n';

            angulo += (2 * M_PI / (float)slices);
        }
        atualH = proxH;
        atualR = proxR;
    
    }
    wrtfile(s.str(), name);

}

void drawsphere(int radius, int slices, int stacks, string name) {
    stringstream s;
    float div;
    float camada;
    float step_div = 2 * M_PI / slices;
    float step_cam = M_PI / stacks;

    for(int i = 0; i < stacks; i++){
        camada = step_cam * i;
        for (int j=0; j < slices; j++){
            div = step_div * j;

            //fazer os dois triangulos que fazer a forma da face da esfera em cada iteração do circulo
            s << radius * sin(div) * sin(camada) << ' ' << radius * cos(camada) << ' ' << radius * sin(camada) * cos(div) << "\n";
            s << radius * sin(camada + step_cam) * sin(div + step_div) << ' ' << radius * cos(camada + step_cam) << ' ' << radius * sin(camada + step_cam) * cos(div + step_div) << "\n";
            s << radius * sin(camada) * sin(div + step_div) << ' ' << radius * cos(camada) << ' ' << radius * sin(camada) * cos(div + step_div) << "\n";
           
            s << radius * sin(div) * sin(camada) << ' ' << radius * cos(camada) << ' ' << radius * sin(camada) * cos(div) << "\n";
            s << radius * sin(camada + step_cam) * sin(div) << ' ' << radius * cos(camada + step_cam) << ' ' << radius * sin(camada + step_cam) * cos(div) << "\n";
            s << radius * sin(camada + step_cam) * sin(div + step_div) << ' ' << radius * cos(camada + step_cam) << ' ' << radius * sin(camada + step_cam) * cos(div + step_div) << "\n";
        }
    }
    wrtfile(s.str(), name);
}

void drawtorus(int rf, int rd, int slices, int stacks, string name) {
    stringstream s;
    float arch_alpha = (2 * M_PI) / stacks, arch_beta = (2 * M_PI) / slices;
    float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;

    rf = (rf + rd) / 2;
    rd = rf - rd;


    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            x1 = (rf + rd * cos(arch_alpha * i)) * cos(arch_beta * j);
            x2 = (rf + rd * cos(arch_alpha * (i+1))) * cos(arch_beta * j);
            x3 = (rf + rd * cos(arch_alpha * (i+1))) * cos(arch_beta * (j+1));
            x4 = (rf + rd * cos(arch_alpha * i)) * cos(arch_beta * (j+1));

            y1 = rd * sin(arch_alpha * i);
            y2 = rd * sin(arch_alpha * (i+1));
            y3 = rd * sin(arch_alpha * (i+1));
            y4 = rd * sin(arch_alpha * i);

            z1 = (rf + rd * cos(arch_alpha * i)) * sin(arch_beta * j);
            z2 = (rf + rd * cos(arch_alpha * (i+1))) * sin(arch_beta * j);
            z3 = (rf + rd * cos(arch_alpha * (i+1))) * sin(arch_beta * (j+1));
            z4 = (rf + rd * cos(arch_alpha * i)) * sin(arch_beta * (j+1));

            s << x1 << ' '  << y1 << ' '  << z1 << '\n';
            s << x2 << ' '  << y2 << ' '  << z2 << '\n';
            s << x4 << ' '  << y4 << ' '  << z4 << '\n';

            s << x2 << ' '  << y2 << ' '  << z2 << '\n';
            s << x3 << ' '  << y3 << ' '  << z3 << '\n';
            s << x4 << ' '  << y4 << ' '  << z4 << '\n';

        }
    }
    wrtfile(s.str(),name);
}

void drawbezier(string patch, int tesselation, string name){
    stringstream s;
    fstream f;
    f.open(patch,ios::in);
    int patches=0;
    int n_vertices=0;
    vector <vector <int>> indices_patch;
    vector <Coordenada> vertices;
    
    if(f.is_open()){
        string line;
        if(getline(f,line)) sscanf(line.c_str(),"%d\n",&patches);
        for(int i=0; i<patches ;i++){
             vector<int> indiceL;
            if(getline(f,line)){
                int i1=0, i2=0, i3=0, i4=0, i5=0, i6=0, i7=0, i8=0, i9=0, i10=0, i11=0, i12=0, i13=0, i14=0, i15=0, i16=0;
                sscanf(line.c_str(),"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",&i1,&i2,&i3,&i4,&i5,&i6,&i7,&i8,&i9,&i10,&i11,&i12,&i13,&i14,&i15,&i16);
                indiceL.push_back(i1);
                indiceL.push_back(i2);
                indiceL.push_back(i3);
                indiceL.push_back(i4);
                indiceL.push_back(i5);
                indiceL.push_back(i6);
                indiceL.push_back(i7);
                indiceL.push_back(i8);
                indiceL.push_back(i9);
                indiceL.push_back(i10);
                indiceL.push_back(i11);
                indiceL.push_back(i12);
                indiceL.push_back(i13);
                indiceL.push_back(i14);
                indiceL.push_back(i15);
                indiceL.push_back(i16);
            }
            
            indices_patch.push_back(indiceL);
        }
        if(getline(f,line)) sscanf(line.c_str(),"%d\n",&n_vertices);
        for(int i=0; i<n_vertices ;i++){
            float x=0,y=0,z=0;
            if(getline(f,line)) sscanf(line.c_str(),"%f, %f, %f\n",&x,&y,&z);
  
            Coordenada coord;
            coord.c.push_back(x);
            coord.c.push_back(y);
            coord.c.push_back(z);
            
            vertices.push_back(coord);
        }
        
        f.close();
    }
    
    float res[3];
    float t;
    int index, indices[4];
    float q[4][tesselation][3],r[tesselation][tesselation][3],tess = 1/((float)tesselation-1);
    
    for(int n=0; n<patches; n++){
        float p[16][3];
        for(int m=0; m<16; m++){
            p[m][0] = vertices[indices_patch[n][m]].c[0];
            p[m][1] = vertices[indices_patch[n][m]].c[1];
            p[m][2] = vertices[indices_patch[n][m]].c[2];
        }
        int j=0,k=0;

        for(int i=0; i<15; i+=4){
            indices[0] = i;
            indices[1] = i+1;
            indices[2] = i+2;
            indices[3] = i+3;

            for(int a=0; a<tesselation-1; a++){
                t = a*tess;
                index = floor(t);
                t = t - index;
                res[0] = (-p[indices[0]][0] +3*p[indices[1]][0] -3*p[indices[2]][0] +p[indices[3]][0])*t*t*t + (3*p[indices[0]][0] -6*p[indices[1]][0] +3*p[indices[2]][0])*t*t + (-3*p[indices[0]][0] +3*p[indices[1]][0])*t + p[indices[0]][0];
                res[1] = (-p[indices[0]][1] +3*p[indices[1]][1] -3*p[indices[2]][1] +p[indices[3]][1])*t*t*t + (3*p[indices[0]][1] -6*p[indices[1]][1] +3*p[indices[2]][1])*t*t + (-3*p[indices[0]][1] +3*p[indices[1]][1])*t + p[indices[0]][1];
                res[2] = (-p[indices[0]][2] +3*p[indices[1]][2] -3*p[indices[2]][2] +p[indices[3]][2])*t*t*t + (3*p[indices[0]][2] -6*p[indices[1]][2] +3*p[indices[2]][2])*t*t + (-3*p[indices[0]][2] +3*p[indices[1]][2])*t + p[indices[0]][2];
                q[j][k][0] = res[0];
                q[j][k][1] = res[1];
                q[j][k][2] = res[2];
                k++;
            }
            
            t = 1;
            
            res[0] = (-p[indices[0]][0] +3*p[indices[1]][0] -3*p[indices[2]][0] +p[indices[3]][0])*t*t*t + (3*p[indices[0]][0] -6*p[indices[1]][0] +3*p[indices[2]][0])*t*t + (-3*p[indices[0]][0] +3*p[indices[1]][0])*t + p[indices[0]][0];
            res[1] = (-p[indices[0]][1] +3*p[indices[1]][1] -3*p[indices[2]][1] +p[indices[3]][1])*t*t*t + (3*p[indices[0]][1] -6*p[indices[1]][1] +3*p[indices[2]][1])*t*t + (-3*p[indices[0]][1] +3*p[indices[1]][1])*t + p[indices[0]][1];
            res[2] = (-p[indices[0]][2] +3*p[indices[1]][2] -3*p[indices[2]][2] +p[indices[3]][2])*t*t*t + (3*p[indices[0]][2] -6*p[indices[1]][2] +3*p[indices[2]][2])*t*t + (-3*p[indices[0]][2] +3*p[indices[1]][2])*t + p[indices[0]][2];
            
            q[j][k][0] = res[0];
            q[j][k][1] = res[1];
            q[j][k][2] = res[2];
            j++;
            k=0;
        }
        
        for(int j=0; j<tesselation; j++){
            for(int a=0; a<tesselation-1; a++){
                t = a*tess;
                index = floor(t);
                t = t - index;
                
                res[0] = (-q[0][j][0] +3*q[1][j][0] -3*q[2][j][0] +q[3][j][0])*t*t*t + (3*q[0][j][0] -6*q[1][j][0] +3*q[2][j][0])*t*t + (-3*q[0][j][0] +3*q[1][j][0])*t + q[0][j][0];
                res[1] = (-q[0][j][1] +3*q[1][j][1] -3*q[2][j][1] +q[3][j][1])*t*t*t + (3*q[0][j][1] -6*q[1][j][1] +3*q[2][j][1])*t*t + (-3*q[0][j][1] +3*q[1][j][1])*t + q[0][j][1];
                res[2] = (-q[0][j][2] +3*q[1][j][2] -3*q[2][j][2] +q[3][j][2])*t*t*t + (3*q[0][j][2] -6*q[1][j][2] +3*q[2][j][2])*t*t + (-3*q[0][j][2] +3*q[1][j][2])*t + q[0][j][2];
                r[j][k][0] = res[0];
                r[j][k][1] = res[1];
                r[j][k][2] = res[2];
                k++;
            }
            
            t = 1;
            
            res[0] = (-q[0][j][0] +3*q[1][j][0] -3*q[2][j][0] +q[3][j][0])*t*t*t + (3*q[0][j][0] -6*q[1][j][0] +3*q[2][j][0])*t*t + (-3*q[0][j][0] +3*q[1][j][0])*t + q[0][j][0];
            res[1] = (-q[0][j][1] +3*q[1][j][1] -3*q[2][j][1] +q[3][j][1])*t*t*t + (3*q[0][j][1] -6*q[1][j][1] +3*q[2][j][1])*t*t + (-3*q[0][j][1] +3*q[1][j][1])*t + q[0][j][1];
            res[2] = (-q[0][j][2] +3*q[1][j][2] -3*q[2][j][2] +q[3][j][2])*t*t*t + (3*q[0][j][2] -6*q[1][j][2] +3*q[2][j][2])*t*t + (-3*q[0][j][2] +3*q[1][j][2])*t + q[0][j][2];
            
            r[j][k][0] = res[0];
            r[j][k][1] = res[1];
            r[j][k][2] = res[2];
            k=0;
        }
        
        for(int i=0; i<tesselation-1; i++)
            for(int j=0; j<tesselation-1; j++){
                s << r[i][j][0] << ' ' << r[i][j][1] << ' ' << r[i][j][2] << '\n';
                s << r[i+1][j][0] << ' ' << r[i+1][j][1] << ' ' <<r[i+1][j][2] << '\n';
                s << r[i][j+1][0] << ' ' << r[i][j+1][1] << ' ' << r[i][j+1][2] << '\n';
                
                s << r[i+1][j][0] << ' ' << r[i+1][j][1] << ' ' << r[i+1][j][2] << '\n';
                s << r[i+1][j+1][0] << ' ' << r[i+1][j+1][1] << ' ' << r[i+1][j+1][2] << '\n';
                s << r[i][j+1][0] << ' ' << r[i][j+1][1] << ' ' << r[i][j+1][2] << '\n';
            }
    }
    wrtfile(s.str(), name);
}

std::vector<std::vector<float>> readfile(std::string filename) {
    std::vector<std::vector<float>> vertices;
    std::ifstream file("..\\engine\\Fich3d\\" + filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::vector<float> vertex;
            float value;
            while (iss >> value) {
                vertex.push_back(value);
                cout << value << ' ';
                cout << "\n";
            }
            vertices.push_back(vertex);
        }
        file.close();
    }
    else {
        std::cout << "Nao foi possivel abrir o arquivo " << filename << " para leitura." << std::endl;
    }
    return vertices;
}

int main(int argc, char const* argv[]) {

    if (strcmp(argv[1], "sphere") == 0) {
        if (argc != 6) {
            return -1;
        }
        drawsphere(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
    }

    else if (strcmp(argv[1], "box") == 0) {
        if (argc != 5) {
            return -1;
        }
        drawbox(atoi(argv[2]), atoi(argv[3]), argv[4]);
    }

    else if (strcmp(argv[1], "cone") == 0) {
        if (argc != 7) {
            return -1;
        }
        drawcone(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }

    else if (strcmp(argv[1], "plane") == 0) {
        if (argc != 5) {
            return -1;
        }
        drawplane(atoi(argv[2]), atoi(argv[3]), argv[4]);
    }

    else if (strcmp(argv[1], "torus") == 0) {
        if (argc != 7) {
            return -1;
        }
        drawtorus(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if(strcmp(argv[1], "patch") == 0){
        if(argc!=5) { 
        	return -1;
        }
        drawbezier(argv[2], atoi(argv[3]),argv[4]);
    }

    return 0;
}