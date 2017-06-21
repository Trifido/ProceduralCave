#include "Camera.h"

Camera::Camera() 
{
	this->proj = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
	this->rotation = 0.0f;
	this->rotationX = 0.0f;
	timelapse = 0.0f;
}

void Camera::MoveCamera(unsigned char key) {
	switch (key) {
		case 'w':
			maxZView();
			break;
		case 's':
			minusZView();
			break;
		case 'a':
			maxXView();
			break;
		case 'd':
			minusXView();
			break;
		case 'e':
			IncrementRot();
			break;
		case 'q':
			DecrementRot();
			break;
		case 'r':
			IncrementRotX();
			break;
		case 'f':
			DecrementRotX();
			break;
	}

	if (key == 'e' || key == 'q') {
		RotateCamera();
	}
	
	if(key == 'r' || key == 'f') {
		RotateCameraX();
	}
}

void Camera::RotateCamera()
{
	this->view = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)) * this->view;
}

void Camera::RotateCameraX()
{
	this->view = glm::rotate(glm::mat4(1.0f), rotationX, glm::vec3(1, 0, 0)) * this->view;
}

bool Camera::InitCamera(float yCamera, float factorScale)
{
	this->proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, farp);
	this->view = glm::mat4(1.0f);
	this->view[3].z = -6.0f;

	path = new Spline(yCamera, factorScale);

	this->view[3].y = -20.0f;
	this->view[3].x = path->GetInitScalePoints()[0].x;
	this->view[3].z = path->GetInitScalePoints()[0].z;
	//path->ScaleZPoints(offsetInside);
	//path->ScaleXPoints(offsetInside);

	return true;
}

void Camera::ResizeAspectRatio(float width, float height)
{ 
	this->proj = glm::perspective(glm::radians(60.0f), float(width) / float(height), this->nearp, this->farp);
}

void Camera::SetModelView(glm::mat4 mv)
{
	modelView = mv;
}

void Camera::SetModelViewProj(glm::mat4 mvp)
{
	modelViewProj = mvp;
}

void Camera::SetNormal(glm::mat4 norm)
{
	normal = norm;
}

Camera::~Camera() {}

void Camera::AnimateCamera()
{
	Vector coord = path->GetScaleCoord((int)timelapse);
	//printf("%f %f %f\n", coord.x, coord.y, coord.z);
	view[3].z = coord.z;
	view[3].x = coord.x;
	timelapse += 0.1f;
	//REPARAR GIRO DE CÁMARA CON LA SPLINE
}

void Camera::Translate(glm::vec3 cord)
{
	this->view *= glm::translate(glm::mat4(1.0f), cord);
}

glm::vec3 Camera::GetPos()
{
	return glm::vec3(view[3].x, view[3].y, view[3].z);
}