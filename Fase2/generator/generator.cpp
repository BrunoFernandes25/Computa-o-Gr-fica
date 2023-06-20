#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

void wrtfile(std::string objeto, std::string name) {
    std::ofstream file("..\\engine\\Fich3d\\" + name);
    if (file.is_open()) {
        file << objeto << std::endl;
        file.close();
    }
    else {
        std::cout << "Nao foi possivel abrir o arquivo " << name << " para escrita." << std::endl;
    }
}

void drawplane(int length, int divisions, std::string name) {
    std::stringstream s;

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

void drawbox(int length, int divisions, std::string name) {
    std::stringstream s;

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

void drawcone(int radius, int height, int slices, int stacks, std::string name) {
    std::stringstream s;
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

void drawsphere(int radius, int slices, int stacks, std::string name) {
    std::stringstream s;
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

void drawtorus(int rf, int rd, int slices, int stacks, std::string name) {
    std::stringstream s;
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

    return 0;
}