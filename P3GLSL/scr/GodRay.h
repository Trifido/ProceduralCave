#ifndef _GODRAY_H
#define _GODRAY_H

#include <glm/glm.hpp>
#include "Texture.h"
#include "Mesh.h"
#include "Scene.h"
#include "GLSLProgram.h"
#include "Light.h"

#include <vector>

class GodRay
{
private:
	std::vector<Mesh> planes;
	GLSLProgram *shadeProg;
	glm::vec3 coordinates;
	int limit = 4;

public:
	GodRay();
	~GodRay();

	void AddToScene(Scene &scene);
	void InitGodRay(glm::vec2 position);
	void AddLightsToShader(Light light, glm::vec3 ambientLight);
	void AddShader(GLSLProgram &prog);
	void Destroy();
};


#endif
