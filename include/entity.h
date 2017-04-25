#pragma once
#include "graphics3d.h"
#include "model.h"
typedef struct
{
	Model* model;
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec4 lastPosition;
	bool onGround;
	char* name;
}Entity;

Entity* createEntity();
void touch(Entity* ent, Entity* ent2);
void drawEntity(Entity* entity);
void updateEntity(Entity* entity);
void initEntity(Entity* entity);
void setVelocity(Entity* entity, glm::vec3 velocity);
void moveEntity(Entity* entity, glm::vec3 offset);
void updateMatrix(Entity* ent);
void updateCollider(Entity* ent, glm::vec3 velocity);
bool colliding(Entity* ent, Entity* ent2);
void freeEntity(Entity** entity);

