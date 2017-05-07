#pragma once
#include "entity.h"

typedef struct
{
	Entity* gameObject;
	GLfloat intensity;
	glm::vec3 direction;
	glm::vec3 color;
	GLfloat range;
}Light;

Light* createLight();