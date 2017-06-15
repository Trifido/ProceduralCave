#ifndef _LIGHT_H
#define _LIGHT_H

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include <iostream>

enum TypeLight
{
		POINT_LIGHT = 0,
		SPOT_LIGHT = 1,
		DIRECTIONAL_LIGHT = 2
};

class Light
{
	private:
		glm::vec3 intensity;
		glm::vec3 lightCoord;
		glm::vec3 lightPosShader;
		TypeLight type;

	private:
		void MoveLight(unsigned char key, Camera camera);
		void ChangeIntensity(unsigned char key);

	public:
		Light();
		Light(TypeLight type);
		void LightController(unsigned char key, Camera camera);

		inline void SetPosition(glm::vec3 newPos) { this->lightCoord = newPos; }
		inline glm::vec3 GetPosition() { return this->lightPosShader; }
		inline glm::vec3 GetIntensity() { return this->intensity; }
		inline void SetIntensity(glm::vec3 newColor) { this->intensity = newColor; }
		inline int GetType() { return this->type; }
};

#endif