#include "projectile.h"


Projectile* createProjectile(glm::vec3 speed)
{
	Projectile* proj = (Projectile*)malloc(sizeof(Projectile));
	proj->gameObject = createEntity();
	proj->gameObject->velocity = speed;
	proj->active = false;
	proj->gameObject->name = "Projectile";
	return proj;
}