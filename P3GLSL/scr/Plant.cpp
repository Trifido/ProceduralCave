#include "Plant.h"

Plant::Plant()
{
	shadeProg = new GLSLProgram();
	for (int i = 0; i < limit; i++)
	{
		planes.push_back(Mesh("../img/plant.png"));
	}
}

Plant::~Plant()
{
	this->Destroy();
}

void Plant::Destroy()
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

void Plant::AddToScene(Scene &scene)
{
	for (int i = 0; i < limit; i++)
	{
		scene.AddObject(planes.at(i));
	}
}

void Plant::AddLightsToShader(Light light, glm::vec3 ambientLight)
{
	this->shadeProg->AddLight(light);
	this->shadeProg->AddAmbientLight(ambientLight);
}

void Plant::InitPlant(glm::vec2 position)
{
	int i = 0;
	for (int i = 0; i < limit; i++)
	{
		planes.at(i).AddDisplacementShader(*shadeProg);

		planes.at(i).GeneratePlant("../img/plant.png", "../img/emitPlant.png", 40.0f);
		planes.at(i).InitPlanePlant(20, 10, 1.0f, 1.0f, false);
		planes.at(i).Traslation(glm::vec3(0.0, 0.0f, 0.0));

	}

	for (int i = 0; i < limit; i++)
		planes.at(i).Traslation(glm::vec3(position.x - 15.0f, 12.0f, position.y + 15.0f));


	for (int i = 0; i < limit; i++)
		planes.at(i).Rotation(-1.5708f, glm::vec3(1, 0, 0));

	planes.at(1).Rotation(0.785398f, glm::vec3(0, 0, 1));
	planes.at(2).Rotation(1.5708f, glm::vec3(0, 0, 1));
	planes.at(3).Rotation(2.35619f, glm::vec3(0, 0, 1));
}

void Plant::AddShader(GLSLProgram &prog)
{
	this->shadeProg = &prog;
}