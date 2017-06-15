#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "GLSLProgram.h"
#include "BOX.h"
#include "auxiliar.h"
#include "Scene.h"
#include "Spline.h"
#include "GodRay.h"

#include <windows.h>

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Camera
Camera camera;

//Spline
Spline spline(0.0f, 65);

//GLSLPrograms
GLSLProgram programa, programa1, programa2, programa3, programaCube, programaSky, programaGodRay;

//Meshes
Mesh paredExterior("../img/planet.png", "../img/bump.png", "../img/noise_Voronoi.png");
Mesh paredInterior("../img/planet.png", "../img/bump.png", "../img/noise_Voronoi.png");
Mesh techo("../img/textureCave.jpg", "../img/bump.png", "../img/noise_Voronoi.png");
Mesh suelo("../img/textureCave.jpg", "../img/bump.png", "../img/noise_Voronoi.png");
Mesh water;
Mesh skyBox(true);
//Mesh sky("../img/skybox/top.jpg", "../img/bump.png", "../img/noise_Voronoi.png");
//
//Mesh cube;
//
GodRay *godRay;

//Variable cambio intensidad
Light light1;
Light light2(DIRECTIONAL_LIGHT);
//Light light3(SPOT_LIGHT);

float valorIntensidad = 0.5f;
glm::vec2 lightCoord = glm::vec2(0.0f);
glm::vec3 lightPos = glm::vec3(1.0f);

Scene scene1;

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initObj();
void destroy();

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	programa.InitShader("../shaders_P3/shaderHeightMap.vert", "../shaders_P3/shaderHeightMap.frag");
	programa1.InitShader("../shaders_P3/shaderHeightMapCeiling.vert", "../shaders_P3/shaderHeightMapCeiling.frag");
	programa2.InitShader("../shaders_P3/waterShader.vert", "../shaders_P3/waterShader.frag");
	programa3.InitShader("../shaders_P3/skyboxShader.vert", "../shaders_P3/skyboxShader.frag");
	programaCube.InitShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
	programaSky.InitShader("../shaders_P3/shaderHeightMap.vert", "../shaders_P3/shaderHeightMap.frag");
	programaGodRay.InitShader("../shaders_P3/shaderGodRay.vert", "../shaders_P3/shaderGodRay.frag");

	initObj();

	glutMainLoop();

	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){
	//Crea un contexto básico
	glutInit(&argc, argv);
	//Genera un contexto de opengl 3.3
	glutInitContextVersion(3, 3);
	//Que no tenga recompatibilidad hacia atrás
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//ELEMENTOS DEL FRAMEBUFFER
	//Crea el framebuffer con profundidad
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("P2-RA");

	//Se pone esto porque glew es muy antigua, para poner funcionalidades experimentales
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	// Preguntamos con que versión de opengl estamos trabajando
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
}

void initOGL(){
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.9f, 0.0f, 0.0f, 0.0f);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	//Activar Culling, pintar cara front y back rellenas.
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_CULL_FACE);

	camera.InitCamera(20.0f, 75.0f);
}

void destroy(){
	programa.Destroy();
	programa1.Destroy();
	programa2.Destroy();
	programa3.Destroy();
	programaCube.Destroy();
	programaSky.Destroy();
	programaGodRay.Destroy();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	paredExterior.Destroy(programa);
	paredInterior.Destroy(programa);
	techo.Destroy(programa1);
	suelo.Destroy(programa);
	water.Destroy(programa2);
	skyBox.Destroy(programa3);
	//sky.Destroy(programaSky);

	for (int i = 0; i < spline.GetNumInitPoints(); i++)
		godRay[i].Destroy();

	//cube.Destroy(programaGodRay);
}

void initObj()
{
	light2.SetIntensity(glm::vec3(1.0, 1.0, 1.0));
	//light3.SetIntensity(glm::vec3(0.0, 0.0, 1.0));

	programa.AddLight(light1);
	programa.AddAmbientLight(scene1.getAmbientLight());

	programa1.AddLight(light1);
	programa1.AddAmbientLight(scene1.getAmbientLight());

	programa2.AddLight(light2);
	programa2.AddAmbientLight(scene1.getAmbientLight());

	programaCube.AddLight(light1);
	programaCube.AddAmbientLight(scene1.getAmbientLight());

	programaSky.AddLight(light1);
	programaSky.AddAmbientLight(glm::vec3(1.0f));

	programaGodRay.AddLight(light1);
	programaGodRay.AddAmbientLight(scene1.getAmbientLight());

	paredExterior.AddDisplacementShader(programa);
	paredInterior.AddDisplacementShader(programa);
//	sky.AddDisplacementShader(programaSky);
	techo.AddDisplacementHoleShader(programa1);
	suelo.AddDisplacementShader(programa);
	water.AddDisplacementShader(programa2);

	int numPointSpline = spline.GetNumInitPoints();
	godRay = new GodRay[numPointSpline];
	Vector *initsPoints = spline.GetInitScalePoints();

	for (int i = 0; i < numPointSpline; i++)
		godRay[i].AddShader(programaGodRay);

	for (int i = 0; i < numPointSpline; i++)
	{
		godRay[i].InitGodRay(glm::vec2(initsPoints[i].x, initsPoints[i].z));
	}

	skyBox.AddShader(programa3);

	////cube.AddShader(programaGodRay);

	////cube.InitDefaultMesh();
	////cube.Traslation(glm::vec3(0.0f, 27.0f, 0.0f));
		
	paredExterior.InitPlaneMesh(1000, 500, 0.40f, 0.5f, 0.1f, 100.0f, false, true, 1.0f);
	paredInterior.InitPlaneMesh(500, 500, 0.40f, 0.5f, 0.1f, 50.0f, true, true, 1.0f);
	suelo.InitPlane(500, 500, 1.0f, 1.0f, false, 2.0f);
	//sky.InitPlane(500, 500, 1.0f, 0.0f, true, 1.0f);
	//sky.Traslation(glm::vec3(0.0, 10.0f, 0.0));
	techo.InitPlane(500, 500, 1.0f, 1.0f, true, 6.0f);

	water.GenerateWater("../img/blueWater.jpg", "../img/bump.png", "../img/perlin.jpg", 40.0f);//blueWater
	water.InitPlane(500, 500, 1.0f, 1.0f, false);
	water.Traslation(glm::vec3(0.0, 2.0f, 0.0));

	skyBox.InitSky();
	skyBox.Scalation(glm::vec3(400.0f, 400.0f, 400.0f));
	

	////scene1.AddObject(cube);
	scene1.AddObject(skyBox);
	scene1.AddObject(paredExterior);
	scene1.AddObject(paredInterior);
	scene1.AddObject(suelo);
	//scene1.AddObject(sky);
	scene1.AddObject(techo);
	scene1.AddObject(water);

	for (int i = 0; i < spline.GetNumInitPoints(); i++)
		godRay[i].AddToScene(scene1);
	
	

	scene1.AddLight(light1);
	scene1.AddLight(light2);
	//scene1.AddLight(light3);
	
	scene1.AddCamera(camera);

	
}

void renderFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene1.Render();

	glutSwapBuffers();
}

void resizeFunc(int width, int height)
{
	camera.ResizeAspectRatio((float)width, (float)height);
	glViewport(0, 0, width, height);
}

void idleFunc()
{
	scene1.Animation();

	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){
	camera.MoveCamera(key);
	light1.LightController(key, camera);
	scene1.ChangePlaneMesh(key);
	scene1.ChangeTypeRender(key);
}

void mouseFunc(int button, int state, int x, int y){}