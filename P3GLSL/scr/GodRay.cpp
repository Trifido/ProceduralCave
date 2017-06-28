#include "GodRay.h"

GodRay::GodRay()
{
	shadeProg = new GLSLProgram();
	for (int i = 0; i < limit; i++)
	{
		planes.push_back(Mesh("../img/godRay1.jpg"));
	}
}

GodRay::~GodRay()
{
	this->Destroy();
}

void GodRay::Destroy()
{
	shadeProg->Destroy();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (int i = 0; i < limit; i++)
	{
		planes.at(i).Destroy(*shadeProg);
	}
	//shadeProg = new GLSLProgram();
}

void GodRay::AddToScene(Scene &scene)
{
	for (int i = 0; i < limit; i++)
	{
		scene.AddObject(planes.at(i));
	}
}

void GodRay::AddLightsToShader(Light light, glm::vec3 ambientLight)
{
	this->shadeProg->AddLight(light);
	this->shadeProg->AddAmbientLight(ambientLight);
}

//El inicializador, configura el shader y, genera y posiciona la geometría.
//la posicion que se establece son los puntos de control de la spline.
void GodRay::InitGodRay(glm::vec2 position)
{
	int i = 0;
	for (int i = 0; i < limit; i++)
	{
		planes.at(i).AddDisplacementShader(*shadeProg);

		planes.at(i).GenerateGodRay("../img/godRay1.jpg", "../img/perlin.jpg", 40.0f);
		planes.at(i).InitPlaneGodRay(40, 40, 1.0f, 1.0f, false);
		planes.at(i).Traslation(glm::vec3(0.0, 0.0f, 0.0));

	}

	for (int i = 0; i < limit; i++)
		planes.at(i).Traslation(glm::vec3(position.x, 25.0f, position.y));


	for (int i = 0; i < limit; i++)
		planes.at(i).Rotation(-1.5708f, glm::vec3(1, 0, 0));

	planes.at(1).Rotation(0.785398f, glm::vec3(0, 0, 1));
	planes.at(2).Rotation(1.5708f, glm::vec3(0, 0, 1));
	planes.at(3).Rotation(2.35619f, glm::vec3(0, 0, 1));
}

void GodRay::AddShader(GLSLProgram &prog)
{
	this->shadeProg = &prog;
}