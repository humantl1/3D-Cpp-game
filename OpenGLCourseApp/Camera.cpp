#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp)
    : position {startPosition},
      worldUp {glm::normalize(startUp)},
      front  {glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f))} {}

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
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
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

//void Camera::update()
//{
//	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front.y = sin(glm::radians(pitch));
//	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front = glm::normalize(front);
//
//	right = glm::normalize(glm::cross(front, worldUp));
//	up = glm::normalize(glm::cross(right, front));
//}

Camera::~Camera()
{

}
