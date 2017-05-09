#include "entity.h"

void touch(Entity* ent, Entity* ent2)
{
	/*if (colliding(ent, ent2))
	{
		glm::vec3 position = glm::vec3(ent->position.x, ent->position.y, ent->position.z);
		glm::vec3 position2 = glm::vec3(ent2->position.x, ent2->position.y, ent2->position.z);

		GLfloat pushAmount = 0.2f;
		glm::vec3 pushVector = glm::vec3();
		if (ent->velocity.y > 0 && ent->position.y < ent2->position.y)
		{
			pushVector.y = ent2->model->collider.minY - ent->model->collider.maxY;
			ent->velocity.y = 0.0f;
		}
		else if (ent->velocity.y < 0 && ent->position.y > ent2->position.y)
		{
			pushVector.y = ent2->model->collider.maxY - ent->model->collider.minY;
			ent->velocity.y = 0.0f;
			ent->onGround = true;
		}
		if (ent->velocity.x > 0 && ent->position.x < ent2->position.x)
		{
			pushVector.x = ent2->model->collider.minX - ent->model->collider.maxX;
			ent->velocity.x = 0;
		}
		else if (ent->velocity.x < 0 && ent->position.x > ent2->position.x)
		{
			pushVector.x = ent2->model->collider.maxX - ent->model->collider.minX;
			ent->velocity.x = 0.0f;
		}
		if (ent->velocity.z > 0 && ent->position.z < ent2->position.z)
		{
			pushVector.z = ent2->model->collider.minZ - ent->model->collider.maxZ;
			ent->velocity.z = 0.0f;
		}
		else if (ent->velocity.z < 0 && ent->position.z > ent2->position.z)
		{
			pushVector.z = ent2->model->collider.maxZ - ent->model->collider.minZ;
			ent->velocity.z = 0.0f;
		}

		ent->model->collider.minX += pushVector.x;
		ent->model->collider.maxX += pushVector.x;

		ent->model->collider.minY += pushVector.y;
		ent->model->collider.maxY += pushVector.y;

		ent->model->collider.minZ += pushVector.z;
		ent->model->collider.maxZ += pushVector.z;

		ent->position += glm::vec4(pushVector, 1.0f);

		ent->model->collider.x = ent->position.x;
		ent->model->collider.y = ent->position.y;
		ent->model->collider.z = ent->position.z;

		for (GLuint m = 0; m < ent->model->numMeshes; m++)
		{
			for (GLuint v = 0; v < ent->model->meshes[m].numVertices; v++)
			{
				ent->model->meshes[m].vertices[v].position += glm::vec4(pushVector, 0.0f);
			}
		}
	}*/
}

bool entityInUse(Entity* entity)
{
	if (entity)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool colliding(Collider col, Collider col2)
{
	return (col.minX < col2.maxX && col.maxX > col2.minX &&
		col.minY < col2.maxY && col.maxY > col2.minY &&
		col.minZ < col2.maxZ && col.maxZ > col2.minZ);
}

void updateCollider(Entity* ent, glm::vec3 velocity)
{
	ent->model->collider.minX += velocity.x;
	ent->model->collider.minY += velocity.y;
	ent->model->collider.minZ += velocity.z;
	ent->model->collider.maxX += velocity.x;
	ent->model->collider.maxY += velocity.y;
	ent->model->collider.maxZ += velocity.z;

	ent->model->collider.x = ent->position.x;
	ent->model->collider.y = ent->position.y;
	ent->model->collider.z = ent->position.z;
}

void updateMatrix(Entity* ent)
{
	ent->modelMatrix = glm::translate(glm::mat4(), ent->position);
}

void moveEntity(Entity* entity, glm::vec3 offset)
{
	entity->position.x += offset.x;
	entity->position.y += offset.y;
	entity->position.z += offset.z;

	entity->model->collider.x = entity->position.x;
	entity->model->collider.y = entity->position.y;
	entity->model->collider.z = entity->position.z;

	entity->model->collider.minX += offset.x;
	entity->model->collider.minY += offset.y;
	entity->model->collider.minZ += offset.z;
	entity->model->collider.maxX += offset.x;
	entity->model->collider.maxY += offset.y;
	entity->model->collider.maxZ += offset.z;

	entity->modelMatrix = glm::translate(glm::mat4(), entity->position);
}

void setVelocity(Entity* entity, glm::vec3 velocity)
{
	entity->velocity = velocity;
}

Entity* createEntity()
{
	Entity* entity = (Entity*)malloc(sizeof(Entity));
	entity->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	entity->position = glm::vec3(0.0f, 0.0f, 0.0f);
	entity->onGround = false;
	entity->forward = glm::vec3(0.0f, 0.0f, 1.0f);
	entity->right = glm::vec3(1.0f, 0.0f, 0.0f);
	entity->up = glm::vec3(0.0f, 1.0f, 0.0f);
	entity->modelMatrix = glm::mat4();
	entity->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return entity;
}

void initEntity(Entity* entity)
{
	initModel(entity->model);
	entity->position.x = entity->model->collider.x;
	entity->position.y = entity->model->collider.y;
	entity->position.z = entity->model->collider.z;
	entity->modelMatrix = glm::translate(glm::mat4(), entity->position);
}

void freeEntity(Entity** entity)
{
	Entity* ent;
	ent = *entity;

	if (!ent)
	{
		slog("freeEntity: No entity provided");
		return;
	}

	if (ent->model)
	{
		free(ent->model);
	}

	*entity = NULL;
}

void updateEntity(Entity* entity)
{
	
}

void drawEntity(Entity* entity)
{
	drawModel(entity->model);
}