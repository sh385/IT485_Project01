#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

typedef struct
{
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 right;
	GLfloat nearPlane;
	GLfloat farPlane;
	GLfloat yaw;
	GLfloat pitch;
} Camera;

Camera* createCamera(glm::vec3 position, GLfloat nearPlane, GLfloat farPlane);
void rotateCamera(Camera* camera, GLfloat yaw, GLfloat pitch);

void moveCameraBackward(Camera* camera, GLfloat speed);
void moveCameraForward(Camera* camera, GLfloat speed);
void moveCameraLeft(Camera* camera, GLfloat speed);
void moveCameraRight(Camera* camera, GLfloat speed);