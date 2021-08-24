#include <iostream>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "stbimage.h"
#include "Textura.h"
#include<gl/GL.h>
#include<gl/GLU.h>
#include<vector>
#include"cg.h"


using namespace std;

#define FONTE GLUT_BITMAP_9_BY_15

float ANGULO_INICIAL_BRACO = 0.0f;
float ANGULO_FINAL_BRACO = -33.0f;
float ORDENADA_INICIAL_BLOCO = -9.0f;
float ORDENADA_FINAL_BLOCO = -20.0f;

double angle, fAspect;
double eyex, eyey, eyez;
float orbitDegreesX, orbitDegreesY, orbitDegreesZ;

float angleBraco = ANGULO_INICIAL_BRACO;
float speedRotacao = -0.5f;
float eixoBloco = ORDENADA_INICIAL_BLOCO;
float speedQueda = -0.5f;

GLuint obj[5];
GLuint texID[5];

unsigned int esferaID;


enum OBJETOS { VERMELHO, LIXEIRA, MESA };
enum PISO {PISO, GRAMA};
enum TERRA { TERRA };

float angulo = 0.0;

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

void desenhaEsfera(unsigned int& id, Textura* tex, GLfloat raio, GLuint nStacks, GLuint nSectors)
{
	std::vector<std::vector<vec3> > pontos;
	std::vector<std::vector<vec2> > texCoords;

	const GLfloat PI = 3.14159265359;
	const GLfloat TWO_PI = 2 * PI;
	const GLfloat HALF_PI = PI / 2.0;

	GLfloat deltaPhi = PI / nStacks;
	GLfloat deltaTheta = TWO_PI / nSectors;

	for (GLuint i = 0; i <= nStacks; i++)
	{
		GLfloat phi = -HALF_PI + i * deltaPhi;
		GLfloat temp = raio * cos(phi);
		GLfloat y = raio * sin(phi);

		std::vector<vec3> pts;
		std::vector<vec2> texs;

		for (GLuint j = 0; j <= nSectors; j++)
		{
			GLfloat theta = j * deltaTheta;
			GLfloat x = temp * sin(theta);
			GLfloat z = temp * cos(theta);

			GLfloat s = theta / TWO_PI;
			GLfloat t = phi / PI + 0.5;

			pts.push_back(vec3(x, y, z));
			texs.push_back(vec2(s, t));
		}// next j

		pontos.push_back(pts);
		texCoords.push_back(texs);
	}// next i



	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	tex->Bind();

	for (GLuint i = 0; i < nStacks; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		for (GLuint j = 0; j <= nSectors; j++)
		{
			glTexCoord2fv(&texCoords[i][j].x); glVertex3fv(&pontos[i][j].x);
			glTexCoord2fv(&texCoords[(static_cast<std::vector<std::vector<vec2, std::allocator<vec2>>, std::allocator<std::vector<vec2, std::allocator<vec2>>>>::size_type>(i)) + 1][j].x); glVertex3fv(&pontos[static_cast<std::vector<std::vector<vec3, std::allocator<vec3>>, std::allocator<std::vector<vec3, std::allocator<vec3>>>>::size_type>(i) + 1][j].x);
		}
		glEnd();
	}
	glDisable(GL_CULL_FACE);
	tex->UnBind();
	glEndList();
}

void desenha(float dt)
{
	float velocAng = 10 * dt;
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -30.0);
	glRotatef(angulo, 1.0, 0.0, 0.0);
	glRotatef(angulo, 0.0, 1.0, 0.0);
	glRotatef(angulo, 0.0, 0.0, 1.0);
	glCallList(esferaID);

	angulo += velocAng;
	if (angulo >= 360.0)
		angulo = 0.0;
}

void init()
{
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	
	// glEnable(GL_NORMALIZE);
	// glEnable(GL_DEPTH_TEST);
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

void carregaImage(GLuint tex_id,std::string filePath) {
	unsigned char* imgData;
	int larg, alt, canais;
	imgData = stbi_load(filePath.c_str(), &larg, &alt, &canais,4 );

	if (imgData)
	{
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, larg, alt, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		stbi_image_free(imgData);
	}
	else {
		std::cout << "ERRO:: Nao foi possivel carregar a textura!" << filePath.c_str() << std::endl;
	}

}

void quadrado(GLuint id, GLuint texid, float SIZE) {
	
	
	//glTranslated(25.0f, eixoBloco, 40.0f);

	//glNewList(id, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, texid);
	glColor3f(1.0, 1.0, 1.0);
	glTranslated(0.0f, 22.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(SIZE, SIZE, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-SIZE, SIZE, 0.0);
	glTranslated(-26, 10, 7);
	glEnd();
	//glEndList();

}

void desenhaBloco(GLuint id, GLuint texid, float SIZE)
{
	glBindTexture(GL_TEXTURE_2D, texid);
	glPushMatrix();
	glTranslated(25.0f, eixoBloco, 28.0f);
	glRotatef(180.0, 0, 0, 1);
	glTexCoord2f(0.0, 2.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(SIZE, SIZE, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-SIZE, SIZE, 0.0);
	glTranslated(-26, 10, 7);
	glEnd();
	glPopMatrix();


}
void desenhaBloco2(float SIZE)
{
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(25.0f, eixoBloco, 40.0f);
	glutSolidCube(2.0f);
	glPopMatrix();


}

void desenhaCubo()
{
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(25.0f, eixoBloco, 40.0f);
	glutSolidCube(2.0f);
	glPopMatrix();
}

void desenhaBraco()
{
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(25.0f, 0.0f, 20.0f);
	glEnd();

	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(1, 128 / 255, 128 / 255);
	glVertex3f(25.0f, 0, 20.0f);
	glVertex3f(25.0f, -7, 20.0f);

	glEnd();
}

void desenhaCilindro(GLuint id, GLuint texid, float SIZE)
{
	glBindTexture(GL_TEXTURE_2D, texid);
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslated(-26, 10, 7);
	glRotatef(180.0, 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(SIZE, SIZE, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-SIZE, SIZE, 0.0);
	glTranslated(-26, 10, 7);
	glEnd();
	//glutSolidCylinder(2.0f, 10, 32, 12);
	glPopMatrix();
}

void desenhaArmario(GLuint id, GLuint texid, float SIZE)
{
	glBindTexture(GL_TEXTURE_2D, texid);
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslated(-26, 10, 7);
	glRotatef(180.0, 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(SIZE, -SIZE, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(SIZE, SIZE, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-SIZE, SIZE, 0.0);
	glTranslated(-26, 10, 7);
	glEnd();
	//glutSolidCylinder(2.0f, 10, 32, 12);
	glPopMatrix();
}


void desenhaMesa(GLuint id, GLuint texid, float SIZEa, float SIZEb)
{
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
	glColor3f(0, 0, 1);
	glTexCoord3f(0.0, 1.0, 0.0);
	glVertex3f(-SIZEa, SIZEb, -SIZEa);//p1
	glTexCoord3f(1.0, 1.0, 0.0);
	glVertex3f(SIZEa, SIZEb, -SIZEa);//p2
	glTexCoord3f(1.0, 1.0, 1.0);
	glVertex3f(SIZEa, SIZEb, SIZEa);//p3
	glTexCoord3f(0.0, 1.0, 1.0);
	glVertex3f(-SIZEa, SIZEb, SIZEa);//p4
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(-SIZEa, SIZEb, -SIZEa);
	glVertex3f(-SIZEa, 0, -SIZEa);
	glVertex3f(SIZEa, SIZEb, -SIZEa);
	glVertex3f(SIZEa, 0, -SIZEa);
	glVertex3f(SIZEa, SIZEb, SIZEa);
	glVertex3f(SIZEa, 0, SIZEa);
	glVertex3f(-SIZEa, SIZEb, SIZEa);
	glVertex3f(-SIZEa, 0, SIZEa);
	glEnd();
}

void posicionaObservador()
{
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	// Especifica posição do observador e do alvo
	gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void especificaParametrosVisualizacao()
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle, fAspect, 0.5, 500);
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
	// window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	window = glfwCreateWindow(640, 480, "cenario", NULL, NULL);
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
	angle = 45;
	fAspect = 1;
	eyex = 0;
	eyey = 10;
	eyez = 200;
	orbitDegreesX = 0;
	orbitDegreesY = 0;
	orbitDegreesZ = 0;
	init();
	//float lastTime = (float)glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		//float currentTime = (float)glfwGetTime();
		//float dt = currentTime - lastTime;
		//lastTime = currentTime;


		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// glShadeModel(GL_FLAT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		resize(width, height);


		especificaParametrosVisualizacao();
		glTranslatef(0, -28, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glGenTextures(5, texID);
		carregaImage(texID[OBJETOS(VERMELHO)], "Images/cube/sacola.png");
		carregaImage(texID[OBJETOS(MESA)], "Images/cube/mesa.png");
		carregaImage(texID[OBJETOS(LIXEIRA)], "Images/cube/lixeira2.png");

		obj[OBJETOS(VERMELHO)] = glGenLists(5);
		obj[OBJETOS(MESA)] = glGenLists(5);
		obj[OBJETOS(LIXEIRA)] = glGenLists(5);

				
		//desenha(dt);
		//PlanetTex[TERRA].load("Images/esfera/earth.jpg");
		//esferaID = glGenLists(1);
		//desenhaEsfera(esferaID, &PlanetTex[TERRA], 8, 30, 30);



		desenhaChao();
		desenhaMesa(obj[OBJETOS(MESA)], texID[OBJETOS(MESA)], 10.0f, 20.0f);
		//quadrado(obj[VERMELHO], texID[VERMELHO], 2.0);

		desenhaCilindro(obj[LIXEIRA], texID[LIXEIRA], 4.0f);
		//desenhaArmario(obj[LIXEIRA], texID[LIXEIRA], 4.0f);
		
		glTranslatef(-25.0f, 30.0f, -40.0f);

		glRotated(angleBraco, 0.0f, 1.0f, 0.0f);
		///desenhaBloco(2.0f);
		desenhaBraco();
		desenhaBloco(obj[VERMELHO], texID[VERMELHO], 2.0f);


		/*if (angleBraco != ANGULO_FINAL_BRACO) {
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
		}*/
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);

	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texID[0]);
	glDeleteTextures(2, &texID[1]);
	glDeleteTextures(3, &texID[2]);
	glDeleteTextures(4, &texID[4]);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}