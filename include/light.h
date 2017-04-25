#pragma once
#include "entity.h"

typedef struct
{
	Entity* gameObject;
	GLfloat intensity;
	glm::vec3 color;
}Light;

Light* createLight();