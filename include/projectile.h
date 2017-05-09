#pragma once
#include "entity.h"

typedef struct
{
	Entity* gameObject;
	GLuint timeToLive;
	bool active;
}Projectile;

Projectile* createProjectile(glm::vec3 speed);