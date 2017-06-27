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
	if (manual)
	{
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

		if (key == 'r' || key == 'f') {
			RotateCameraX();
		}
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

	path = new Spline(yCamera, factorScale);

	this->view = glm::translate(this->view, glm::vec3(path->GetInitScalePoints()[0].x, -20.0f, path->GetInitScalePoints()[0].z));

	glm::vec2 viewVector = glm::vec2(view[3].x, view[3].z);
	glm::vec2 nextView = glm::vec2(path->GetInitScalePoints()[0].x - view[3].x, path->GetInitScalePoints()[0].z - view[3].z);

	float dotProduct = viewVector.x * nextView.x + viewVector.y * nextView.y;
	float  determinant = viewVector.x * nextView.y + viewVector.y * nextView.x;
	float angle = atan2(determinant, dotProduct);

	this->view = glm::rotate(this->view, angle, glm::vec3(0.0, 1.0, 0.0));

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
	if (!manual)
	{
		timelapse = (int)timelapse % path->GetNumPathPoints();

		float timelapseAnt = timelapse - time;
		float timelapseSig = timelapse + time;

		timelapseSig = (int)timelapseSig % path->GetNumPathPoints();
		timelapseAnt = (int)timelapseAnt % path->GetNumPathPoints();

		if (timelapseAnt < 0)
			timelapseAnt += path->GetNumPathPoints();

		Vector coordAnt = path->GetScaleCoord((int)(timelapseAnt));
		Vector coord = path->GetScaleCoord((int)timelapse);
		Vector coordSig = path->GetScaleCoord((int)timelapseSig);

		glm::vec2 viewVector = glm::vec2(coord.x - coordAnt.x, coord.z - coordAnt.z);
		glm::vec2 nextView = glm::vec2(coordSig.x - coord.x, coordSig.z - coord.z);

		float dotProduct = viewVector.x * nextView.x + viewVector.y * nextView.y;
		float determinant = viewVector.x * nextView.y - viewVector.y * nextView.x;
		float angle = atan2(determinant, dotProduct);

		//this->view = glm::mat4(1.0f);

		this->view *= glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));

		//this->view *= glm::rotate(glm::mat4(1.0), -angle * 0.5f, glm::vec3(0.0, 1.0, 0.0));

		this->view[3].x = coord.x;
		this->view[3].y = -20.0f;
		this->view[3].z = coord.z;

		//this->view *= glm::translate(glm::mat4(1.0f), glm::vec3(coord.x, -20.0f, coord.z));

		timelapse += time;
	}
}

void Camera::Translate(glm::vec3 cord)
{
	this->view *= glm::translate(glm::mat4(1.0f), cord);
}

glm::vec3 Camera::GetPos()
{
	return glm::vec3(view[3].x, view[3].y, view[3].z);
}

void Camera::ChangeMode()
{
	(manual) ? manual = false : manual = true;
}