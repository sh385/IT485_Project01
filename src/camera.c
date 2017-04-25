#include <stdio.h>
#include "camera.h"

Camera* createCamera(glm::vec3 position, GLfloat nearPlane, GLfloat farPlane)
{
	Camera* camera = (Camera*)malloc(sizeof(Camera));
	camera->yaw = 0.0f;
	camera->pitch = 0.0f;
	camera->position = position;
	camera->worldUp = glm::vec3(0.0, 1.0, 0.0);
	camera->forward.x = glm::cos(glm::radians(camera->pitch)) * glm::cos(glm::radians(camera->yaw));
	camera->forward.y = glm::sin(glm::radians(camera->pitch));
	camera->forward.z = glm::cos(glm::radians(camera->pitch)) * glm::sin(glm::radians(camera->yaw));
	camera->forward = glm::normalize(camera->forward);
	camera->right = glm::normalize(glm::cross(camera->worldUp, camera->forward));
	camera->up = glm::cross(camera->right, camera->forward);
	camera->nearPlane = nearPlane;
	camera->farPlane = farPlane;
	return camera;
}

void rotateCamera(Camera* camera, GLfloat yaw, GLfloat pitch)
{
	camera->yaw += yaw;
	camera->pitch += pitch;

	camera->forward.x = glm::cos(glm::radians(camera->pitch)) * glm::cos(glm::radians(camera->yaw));
	camera->forward.y = glm::sin(glm::radians(camera->pitch));
	camera->forward.z = glm::cos(glm::radians(camera->pitch)) * glm::sin(glm::radians(camera->yaw));
	camera->forward = glm::normalize(camera->forward);
	
	camera->right = glm::normalize(glm::cross(camera->forward, camera->worldUp));
	camera->up = glm::normalize(glm::cross(camera->right, camera->forward));
}

void moveCameraBackward(Camera* camera, GLfloat speed)
{
	camera->position.x -= camera->forward.x * speed;
	camera->position.y -= camera->forward.y * speed;
	camera->position.z -= camera->forward.z * speed;
}

void moveCameraForward(Camera* camera, GLfloat speed)
{
	camera->position.x += camera->forward.x * speed;
	camera->position.y += camera->forward.y * speed;
	camera->position.z += camera->forward.z * speed;
}

void moveCameraLeft(Camera* camera, GLfloat speed)
{
	camera->position.x -= camera->right.x * speed;
	camera->position.y -= camera->right.y * speed;
	camera->position.z -= camera->right.z * speed;
}

void moveCameraRight(Camera* camera, GLfloat speed)
{
	camera->position.x += camera->right.x * speed;
	camera->position.y += camera->right.y * speed;
	camera->position.z += camera->right.z * speed;
}