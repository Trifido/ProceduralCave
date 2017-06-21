#include "Scene.h"

Scene::Scene()
{
	ambientLight = glm::vec3(0.13f);
}

void Scene::AddObject(Mesh &object) {
	objects.push_back(&object);
}

void Scene::AddLight(Light &light) {
	switch (light.GetType()){
	case 0:
		if (dirPoint.size() < num_max){
			lights.push_back(&light);
			dirPoint.push_back(light.GetPosition());

		}
		break;
	case 1:
		if (dirSpot.size() < num_max){
			lights.push_back(&light);
			dirSpot.push_back(light.GetPosition());

		}
		break;
	case 2:
		if (dirDirect.size() < num_max){
			lights.push_back(&light);
			dirDirect.push_back(light.GetPosition());

		}
	}
	
}

void Scene::AddCamera(Camera &camera) {
	cameras.push_back(&camera);
}

void Scene::Render(){
	for (unsigned int i = 0; i < objects.size(); i++){
		(*objects.at(i)).InitRender((*cameras.at(0)));
		//(*objects.at(i)).DefaultMeshRender();
		(*objects.at(i)).Render();

	}
}

void Scene::Animation(){
	//Cambio la matriz model
	static float angle = 0.0;
	angle += 0.001f;

	for(unsigned int i = 0; i < objects.size(); i++)
		(*objects.at(i)).PathAnimation();
	cameras.at(0)->AnimateCamera();
}

void Scene::Destroy()
{
	
}

Mesh Scene::getObject(int i){
	return (*objects.at(i));
}

Camera Scene::getCamera(int i){
	return (*cameras.at(i));
}

void Scene::ChangePlaneMesh(unsigned char key)
{
	for (unsigned int i = 0; i < objects.size(); i++) {
		if(key == 'm')
			if(objects.at(i)->GetTypeMesh() != SKYBOX_MESH)
				objects.at(i)->ChangeScalingFactor(0.1f); //objects.at(i)->GetScalingFactor() + 0.01f
		if(key == 'n')
			if (objects.at(i)->GetTypeMesh() != SKYBOX_MESH)
				objects.at(i)->ChangeScalingFactor(-0.1f);//objects.at(i)->GetScalingFactor() 
	}
	key = NULL;
}

void Scene::ChangeTypeRender(unsigned char key)
{
	static TypeRender type;
	bool change = false;

	if (key == 'z' && type != TYPE_POINTS)
	{
		type = TYPE_POINTS;
		change = true;
	}
	else if (key == 'x' && type != TYPE_LINES)
	{
		type = TYPE_LINES;
		change = true;
	}
	else if (key == 'c' && type != TYPE_FILLS)
	{
		type = TYPE_FILLS;
		change = true;
	}

	if (change)
	{
		for (unsigned int i = 0; i < objects.size(); i++)
			objects.at(i)->SetTypeRender(type);
	}
}

void Scene::ChangePaths(unsigned char key)
{
	static bool changePath = false;
	static unsigned char antKey;
	static std::string antTipoKey;
	float coordX, coordZ;

	if (key == 'p' || key == 'P')
	{
		changePath = true;
		antKey = key;
	}

	if (changePath)
	{
		if (key == '1')
		{
			tipoKey = "Point1";
			antTipoKey = tipoKey;
		}
		else if (key == '2')
		{
			tipoKey = "Point2";
			antTipoKey = tipoKey;
		}
		else if (key == '3')
		{
			tipoKey = "Point3";
			antTipoKey = tipoKey;
		}
		else if (key == '4')
		{
			tipoKey = "Point4";
			antTipoKey = tipoKey;
		}
		else
		{
			if (key == 'i') {
				coordX = 0.0f;
				coordZ = 0.5f;
				antKey = key;
			}
			else if (key == 'k') {
				coordX =  0.0f;
				coordZ = -0.5f;
				antKey = key;
			}
			else if (key == 'j') {
				coordX = -0.5f;
				coordZ = 0.0f;
				antKey = key;
			}
			else if (key == 'l') {
				coordX = 0.5f;
				coordZ = 0.0f;
				antKey = key;
			}
			else
			{
				tipoKey = "NULL";
				antKey = 'N';
			}
			
			if (antKey != 'N')
			{
				for (unsigned int i = 0; i < objects.size(); i++)
				{
					if (objects.at(i)->isCurvedPlane())
					{
						if (tipoKey == "Point1") {
							(objects.at(i)->path)->TranslationX(0, coordX);
							(objects.at(i)->path)->TranslationZ(0, coordZ);
							objects.at(i)->UpdatePlaneMesh();
						}
						else if (tipoKey == "Point2") {
							(objects.at(i)->path)->TranslationX(1, coordX);
							(objects.at(i)->path)->TranslationZ(1, coordZ);
							objects.at(i)->UpdatePlaneMesh();
						}
						else if (tipoKey == "Point3") {
							(objects.at(i)->path)->TranslationX(2, coordX);
							(objects.at(i)->path)->TranslationZ(2, coordZ);
							objects.at(i)->UpdatePlaneMesh();
						}
						else if (tipoKey == "Point4") {
							(objects.at(i)->path)->TranslationX(3, coordX);
							(objects.at(i)->path)->TranslationZ(3, coordZ);
							objects.at(i)->UpdatePlaneMesh();
						}
					}
				}
			}
		}
	}
	else
	{
		antKey = key;
	}
}