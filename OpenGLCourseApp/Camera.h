#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

#include "entity_manager.h"
#include "component.h"
#include "transform_component.h"

class Camera : public Component {
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp);
	void Initialize() override;
  void Update(float delta_time) override;
	void Render() override {}

	//void keyControl(bool* keys, GLfloat deltaTime);
	//void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

	glm::mat4 calculateViewMatrix();

	TransformComponent* transform_;
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	//GLfloat yaw;
	//GLfloat pitch;

	//GLfloat moveSpeed;
	//GLfloat turnSpeed;

	//void update();
};

