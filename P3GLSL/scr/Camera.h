#ifndef _CAMERA_H
#define _CAMERA_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Spline.h"

class Camera
{
	private:
		glm::mat4	proj;
		glm::mat4	view;
		glm::mat4 modelView;
		glm::mat4 modelViewProj;
		glm::mat4 normal;

		//PINHOLE CAMERA
		float rotation, rotationX;
		float nearp = 1.0f;
		float farp = 1000.0f;
		float pesoRGB = 0.5f;
		float pesoAlpha = 0.5f;
		float focalDistance = -25.0f;
		float maxDistanceFactor = 0.2f;

		//OTHER CAMERA
		//glm::vec3 postion = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 direction = glm::normalize(postion - target);
		//glm::vec3 camRight = glm::normalize(glm::cross(up, direction));
		//glm::vec3 camUp = glm::cross(direction, camRight);

		//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		//				   glm::vec3(0.0f, 0.0f, 0.0f),
		//				   glm::vec3(0.0f, 1.0f, 0.0f));

		//OTHER PARAMS
		bool manual = true;
		float timelapse = 0.0f;
		float time = 1.0f;
		Spline *path;

	public:
		Camera();
		~Camera();

		inline void minusZView() {view[3].z -= 2;}	//0.5
		inline void maxZView() { view[3].z += 2; }
		inline void minusXView() { view[3].x -= 2; }
		inline void maxXView() { view[3].x += 2; }

		inline glm::mat4 GetView() { return view; }
		inline glm::mat4 GetProj() { return proj; }
		inline glm::mat4 GetModelView() { return modelView; }
		inline glm::mat4 GetModelViewProj() { return modelViewProj; }
		inline glm::mat4 GetNormal() { return normal; }

		inline void IncrementRot() { rotation = 0.0523599f; }
		inline void DecrementRot() { rotation = -0.0523599f; }
		inline void IncrementRotX() { rotationX = 0.0523599f; }
		inline void DecrementRotX() { rotationX = -0.0523599f; }

		void SetModelView(glm::mat4 mv);
		void SetModelViewProj(glm::mat4 mvp);
		void SetNormal(glm::mat4 norm);

		void MoveCamera(unsigned char key);
		void RotateCamera();
		void RotateCameraX();
		void ResizeAspectRatio(float width, float height);
		bool InitCamera(float yCamera, float factorScale);
		void AnimateCamera();
		void Translate(glm::vec3 cord);
		void ChangeMode();
		glm::vec3 GetPos();
};

#endif