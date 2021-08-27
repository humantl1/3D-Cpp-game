#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp)
    : position {startPosition},
      worldUp {startUp},
      front  {glm::vec3(0.0f, 0.0f, -1.0f)} {}

void Camera::Initialize() {
	transform_ = owner_->GetComponent<TransformComponent>();
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::normalize(glm::cross(right, front));
}

void Camera::Update(float delta_time)
{
	position = transform_->position_;
	front = transform_->rotation_;
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

//void Camera::keyControl(bool* keys, GLfloat deltaTime)
//{	
//	GLfloat velocity = moveSpeed * deltaTime;
//
//	//printf("%d",keys[GLFW_KEY_W]);
//	if (keys[GLFW_KEY_W])
//	{
//		position += front * velocity;
//	}
//	if (keys[GLFW_KEY_S])
//	{
//		position -= front * velocity;
//	}
//	if (keys[GLFW_KEY_A])
//	{
//		position -= right * velocity;
//	}
//	if (keys[GLFW_KEY_D])
//	{
//		position += right * velocity;
//	}
//}

//void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
//{
//	xChange *= turnSpeed;
//	yChange *= turnSpeed;
//
//	yaw += xChange;
//	pitch += yChange;
//
//	if (pitch > 89.0f)
//	{
//		pitch = 89.0f;
//	}
//
//	if (pitch < -89.0f)
//	{
//		pitch = -89.0f;
//	}
//
//	update();
//}

glm::mat4 Camera::calculateViewMatrix()
{
	return  glm::lookAt(position, position + front, up);
}


glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}
