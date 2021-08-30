#include <iostream>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include "stbimage.h"
#include "Textura.h"
#include<vector>
#include "Vetor.h"
#include "ObjLoader.h"


using namespace std;

#define FONTE GLUT_BITMAP_9_BY_15

float ANGULO_INICIAL_BRACO = 0.0f;
float ANGULO_FINAL_BRACO = -2.0f;
float ORDENADA_INICIAL_BLOCO = 2.75f;
float ORDENADA_FINAL_BLOCO = 1.5f;

double angle, fAspect;
double eyex, eyey, eyez;
float orbitDegreesX, orbitDegreesY, orbitDegreesZ;

float angleBraco = ANGULO_INICIAL_BRACO;
float speedRotacao = -0.5f;
float eixoBloco = ORDENADA_INICIAL_BLOCO;
float speedQueda = -0.25f;


float angulo = 30.0;

static unsigned blenderModeId[5];
enum OBJETOS { MESA, ROBO, BLOCO, LIXEIRA, ARMARIO};

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}


static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	/*if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		cout << "GLFW_MOUSE_BUTTON_RIGHT";*/

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//  cout << "GLFW_MOUSE_BUTTON_LEFT";
		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
	}

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		if (angle >= 4) angle -= 4;
	}
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
		if (angle <= 72) angle += 4;
	}
}
void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		// The cursor entered the content area of the window
		cout << "entrou";
	}
	else
	{
		// The cursor left the content area of the window
		cout << "deixou";
	}
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_RIGHT) {
		cout << "GLFW_KEY_RIGHT";
		orbitDegreesX += 5;
		if (orbitDegreesX > 360)
			orbitDegreesX -= 360.0;
	}
	if (action == GLFW_PRESS && key == GLFW_KEY_LEFT) {
		cout << "GLFW_KEY_LEFT";
		orbitDegreesX -= 5;
		if (orbitDegreesX > 360)
			orbitDegreesX -= 360.0;

	}

	if (action == GLFW_PRESS && key == GLFW_KEY_UP) {
		cout << "GLFW_KEY_UP";
		orbitDegreesY += 5;
		if (orbitDegreesY > 360)
			orbitDegreesY -= 360.0;

	}
	if (action == GLFW_PRESS && key == GLFW_KEY_DOWN) {
		cout << "GLFW_KEY_DOWN";
		orbitDegreesY -= 5;
		if (orbitDegreesY > 360)
			orbitDegreesY -= 360.0;

	}
	if (action == GLFW_PRESS && key == GLFW_KEY_W) {

		cout << "GLFW_KEY_W";
		orbitDegreesX = 0;
		orbitDegreesY = 0;
		orbitDegreesZ = 0;
		eyex = 0;
		eyey = 80;
		eyez = 200;


	}
	if (action == GLFW_PRESS && key == GLFW_KEY_S) {


	}
	if (action == GLFW_PRESS && key == GLFW_KEY_A) {
		orbitDegreesZ += 5;
		if (orbitDegreesZ > 360)
			orbitDegreesZ -= 360.0;

	}
	if (action == GLFW_PRESS && key == GLFW_KEY_D) {
		orbitDegreesZ -= 5;
		if (orbitDegreesZ > 360)
			orbitDegreesZ -= 360.0;

	}
}

void init()
{
	glClearColor(1.f, 1.f, 1.f, 0.f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	float globalAmb[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[4][4] = {
				{0.1f, 0.1f, 0.1f, 1.f}, // ambient
				{0.8f, 0.8f, 0.8f, 1.f}, // diffuse
				{ 1.f,  1.f,  1.f, 1.f }, // specular
				{0.f, 0.f, 1.f, 1.f}    // position
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);




}
void desenhaMesa()
{
	//opcoes();
	ObjLoader::loadOBJ(blenderModeId[MESA], "Blender/sala.obj", 1);

	glPushMatrix();
	//glTranslated(-26, 10, 7);
	glTranslatef(-5.f, 0.f, 40.f);
	glRotatef(angulo, 1.f, 0.f, 0.f);
	glCallList(blenderModeId[MESA]);
	//glRotatef(angulo, 0.f, 1.f, 0.f);
	//glRotatef(angulo, 0.f, 0.f, 1.f);
	glPopMatrix();
}
void desenhaBloco()
{
	ObjLoader::loadOBJ(blenderModeId[BLOCO], "Blender/bloco.obj", 2);
	glPushMatrix();
	glTranslated(-3.9f, eixoBloco, 46.0f);
	glCallList(blenderModeId[BLOCO]);
	glPopMatrix();
	
}
void desenhaRobo()
{
	
	ObjLoader::loadOBJ(blenderModeId[ROBO], "Blender/braco.obj", 3);

	glPushMatrix();
	//glTranslated(-26, 10, 7);
	glTranslatef(-0.3f, 3.0f, 45.f);
	glRotatef(angulo, 1.f, 0.f, 0.f);
	glCallList(blenderModeId[ROBO]);
	//glRotatef(angulo, 0.f, 1.f, 0.f);
	//glRotatef(angulo, 0.f, 0.f, 1.f);
	glPopMatrix();
}
void desenhaLixeira()
{
	ObjLoader::loadOBJ(blenderModeId[LIXEIRA], "Blender/cilindro.obj", 4);

	glPushMatrix();
	glTranslatef(-2.82f, 1.2f, 47.0f);
	//glRotatef(10, 0.f, 0.f, 0.f);
	glCallList(blenderModeId[LIXEIRA]);
	glPopMatrix();
}
void desenhaArmario()
{
	ObjLoader::loadOBJ(blenderModeId[ARMARIO], "Blender/armario.obj", 5);

	glPushMatrix();
	//glTranslated(-26, 10, 7);
	glTranslatef(2.5f, 1.0f, 38.f);
	glRotatef(0, 0.f, 0.f, 0.f);
	glCallList(blenderModeId[ARMARIO]);
	//glRotatef(angulo, 0.f, 1.f, 0.f);
	//glRotatef(angulo, 0.f, 0.f, 1.f);
	glPopMatrix();
}

void desenhaChao()
{
	glLineWidth(1);
	glBegin(GL_LINES);
	for (float z = -100; z <= 100; z += 10)
	{
		glVertex3f(-100, -0.1f, z);
		glVertex3f(100, -0.1f, z);
	}
	for (float x = -100; x <= 100; x += 10)
	{
		glVertex3f(x, -0.1f, -100);
		glVertex3f(x, -0.1f, 100);
	}
	glEnd();
}

void posicionaObservador()
{
	//// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	//// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	//// Especifica posição do observador e do alvo
	gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void especificaParametrosVisualizacao()
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle, fAspect, 0.5, 100);
	posicionaObservador();
}

void resize(int w, int h) {

	if (h == 0) h = 1;

	glViewport(0, 0, w, h);
	fAspect = (float)w / (float)h;
}

int main(int argc, char** argv)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(740, 580, "cenario", NULL, NULL);
	if (!window)
	{
		glfwTerminate();

		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	//glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);  
	//glfwSetCursorPosCallback(window, mouse_button_callback);

	glutInit(&argc, argv);
	angle = 60;
	fAspect = 1;
	eyex = 0;
	eyey = 0;
	eyez = 60;
	orbitDegreesX = 0;
	orbitDegreesY = 0;
	orbitDegreesZ = 0;
	init();
	
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// glShadeModel(GL_FLAT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		resize(width, height);


		especificaParametrosVisualizacao();
		glTranslatef(0, -5, 0);
		
		desenhaChao();

		desenhaMesa();
		desenhaLixeira();
		desenhaArmario();

		glRotatef(angleBraco, 0.0f, 1.0f, 0.0f);
		desenhaRobo();
		desenhaBloco();

		if (angleBraco != ANGULO_FINAL_BRACO) {
			angleBraco += speedRotacao;
		}
		else {
			if (eixoBloco != ORDENADA_FINAL_BLOCO) {
				eixoBloco += speedQueda;
			}
			else {
				angleBraco = ANGULO_INICIAL_BRACO;
				eixoBloco = ORDENADA_INICIAL_BLOCO;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}