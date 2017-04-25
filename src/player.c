#include "player.h"
Player* createPlayer()
{
	Player* player = (Player*)malloc(sizeof(Player));
	player->gravity = 0.01f;
	return player;
}

void updatePlayer(Player* player)
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_SPACE] && player->gameObject->onGround)
	{
		player->gameObject->velocity.y = 0.2f;
		player->gameObject->onGround = false;
	}
	if (keystate[SDL_SCANCODE_W])
	{
		player->gameObject->velocity.z = 0.05f;
	}
	else if (keystate[SDL_SCANCODE_S])
	{
		player->gameObject->velocity.z = -0.05f;
	}
	else
	{
		player->gameObject->velocity.z = 0.0f;
	}

	if (keystate[SDL_SCANCODE_A])
	{
		player->gameObject->velocity.x = -0.05f;
	}
	else if (keystate[SDL_SCANCODE_D])
	{
		player->gameObject->velocity.x = 0.05f;
	}
	else
	{
		player->gameObject->velocity.x = 0.0f;
	}
	player->gameObject->velocity.y -= player->gravity;
}

void movePlayer(Player* player, Entity* ent)
{

	if (player->gameObject->velocity.y != 0)
	{
		player->gameObject->position.y += player->gameObject->velocity.y;
		updateCollider(player->gameObject, glm::vec3(0.0f, player->gameObject->velocity.y, 0.0f));
		updateMatrix(player->gameObject);
		if (colliding(player->gameObject, ent))
		{
			GLfloat push = 0.0f;
			if (player->gameObject->velocity.y > 0 && player->gameObject->position.y < ent->position.y)
			{
				push = (ent->model->collider.minY - player->gameObject->model->collider.maxY);
				player->gameObject->position.y += push;
			}
			else if (player->gameObject->velocity.y < 0 && player->gameObject->position.y > ent->position.y)
			{
				push = (ent->model->collider.maxY - player->gameObject->model->collider.minY);
				player->gameObject->position.y += push;
				player->gameObject->onGround = true;
			}
			updateCollider(player->gameObject, glm::vec3(0.0f, push, 0.0f));
			updateMatrix(player->gameObject);
			player->gameObject->velocity.y = 0.0f;
		}
	}
	if (player->gameObject->velocity.x != 0 || player->gameObject->velocity.z != 0)
	{
		if (player->gameObject->velocity.x != 0)
		{
			bool xCollision = false;
			player->gameObject->position.x += player->gameObject->right.x * player->gameObject->velocity.x;
			player->gameObject->position.z += player->gameObject->right.z * player->gameObject->velocity.x;
			updateCollider(player->gameObject, glm::vec3(player->gameObject->right.x * player->gameObject->velocity.x,
				0.0f, player->gameObject->right.z * player->gameObject->velocity.x));
			updateMatrix(player->gameObject);


			while (colliding(player->gameObject, ent))
			{
				xCollision = true;
				glm::vec3 temp = glm::vec3(player->gameObject->position.x, 0.0f, player->gameObject->position.z);
				glm::vec3 temp2 = glm::vec3(ent->position.x, 0.0f, ent->position.z);
				glm::vec3 push = glm::normalize(temp - temp2)*glm::abs(player->gameObject->velocity.x);
				player->gameObject->position += push;
				updateCollider(player->gameObject, push);
				updateMatrix(player->gameObject);
			}

			if (xCollision)
			{
				player->gameObject->velocity.x = 0;
			}
		}
		if (player->gameObject->velocity.z != 0)
		{
			bool zCollision = false;
			player->gameObject->position.x += player->gameObject->forward.x * player->gameObject->velocity.z;
			player->gameObject->position.z += player->gameObject->forward.z * player->gameObject->velocity.z;
			updateCollider(player->gameObject, glm::vec3(player->gameObject->forward.x * player->gameObject->velocity.z,
				0.0f, player->gameObject->forward.z * player->gameObject->velocity.z));
			updateMatrix(player->gameObject);

			while (colliding(player->gameObject, ent))
			{
				zCollision = true;
				glm::vec3 temp = glm::vec3(player->gameObject->position.x, 0.0f, player->gameObject->position.z);
				glm::vec3 temp2 = glm::vec3(ent->position.x, 0.0f, ent->position.z);
				glm::vec3 push = glm::normalize(temp - temp2)*glm::abs(player->gameObject->velocity.z);
				player->gameObject->position += push;
				updateCollider(player->gameObject, push);
				updateMatrix(player->gameObject);
			}

			if (zCollision)
			{
				player->gameObject->velocity.z = 0;
			}
		}
	}
}