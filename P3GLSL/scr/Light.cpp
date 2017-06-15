#include "Light.h"

Light::Light() {
	type = POINT_LIGHT;
	intensity = glm::vec3(0.5f);
	lightCoord = glm::vec3(0.0f, 5.0f, 0.0f);
	lightPosShader = glm::vec3(1.0f);
}

Light::Light(TypeLight type) {
	this->type = type;
	if(type == SPOT_LIGHT)
		intensity = glm::vec3(1.0f);
	else
		intensity = glm::vec3(0.5f);
	lightCoord = glm::vec3(0.0f);
	lightPosShader = glm::vec3(1.0f);
}

void Light::MoveLight(unsigned char key, Camera camera) {
	switch (key)
	{
		case 'i':
			lightCoord[1] -= 0.1f;
			break;
		case 'k':
			lightCoord[1] += 0.1f;
			break;
		case 'j':
			lightCoord[0] -= 0.1f;
			break;
		case 'l':
			lightCoord[0] += 0.1f;
			break;
		default:
			break;
	}

	glm::vec4 resultView = (camera.GetView() * glm::vec4(lightCoord[0], 1.0f, lightCoord[1], 1.0f));
	lightPosShader[0] = resultView.x;
	lightPosShader[1] = resultView.y;
	lightPosShader[2] = resultView.z;
}

void Light::ChangeIntensity(unsigned char key) {
	switch (key)
	{
		case '+':
			intensity += 0.1;
			break;
		case '-':
			intensity -= 0.1;
			break;
		default:
			break;
	}
}

void Light::LightController(unsigned char key, Camera camera) {
	MoveLight(key, camera);
	ChangeIntensity(key);
}