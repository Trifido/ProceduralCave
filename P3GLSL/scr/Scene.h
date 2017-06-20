#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"

class Scene 
{
public:
	static Scene& getInstance()
	{
		static Scene    instance;
		return instance;
	}

private:
	Scene(Scene const&);
	void operator=(Scene const&);

	const unsigned int num_max = 8;

	std::vector <Mesh*> objects;
	std::vector <Light*> lights;
	std::vector <Camera*> cameras;
	glm::vec3 ambientLight;

	//std::vector <>;
	glm::vec3 ia;

	std::vector <glm::vec3> dirDirect;
	std::vector <glm::vec3> iDirect;
	std::vector <glm::vec3> dirPoint;
	std::vector <glm::vec3> iPoint;
	std::vector <glm::vec3> dirSpot;
	std::vector <glm::vec3> iSpot;

	int  numDir = 0;
	int numPoint = 0;
	int numFocal = 0;

	std::string tipoKey;

public:
	Scene();
	~Scene() { Destroy(); }
	void AddObject(Mesh &object);
	void AddLight(Light &light);
	void AddCamera(Camera &camera);
	void Render();
	void Animation();
	void Destroy();
	inline glm::vec3 getAmbientLight() { return ambientLight; }

	Mesh getObject(int i);
	Camera getCamera(int i);

	//Change Meshes
	void ChangePlaneMesh(unsigned char key);
	//Change type render
	void ChangeTypeRender(unsigned char key);
	//Change Paths
	void ChangePaths(unsigned char key);
};

#endif