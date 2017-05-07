#include "player.h"
Player* createPlayer()
{
	Player* player = (Player*)malloc(sizeof(Player));
	player->gravity = 0.01f;
	player->attackBox = createEntity();
	player->attackBox->name = "AttackBox";
	player->attackBox->model = loadModel("C:/Users/sharg_000/Documents/3D_Modeling_Projects/monkey.obj");
	loadTexture(&player->attackBox->model->meshes[0], "C:/Users/sharg_000/Documents/blueTexture.png");
	player->attackBoxActive = false;
	player->attackBoxTime = 0;
	player->hidden = true;
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

	if (keystate[SDL_SCANCODE_H] && !player->attackBoxActive)
	{
		player->attackBoxActive = true;
	}
	
	if (player->attackBoxActive == true)
	{
		glm::vec3 prevPosition = player->attackBox->position;
		glm::vec3 dir = glm::vec3(player->gameObject->forward.x, 0.0f, player->gameObject->forward.z);
		glm::vec3 newPosition = player->gameObject->position + (glm::normalize(dir) * 1.5f);
		moveEntity(player->attackBox, newPosition - prevPosition);

		player->attackBoxTime++;
		if (player->attackBoxTime >= 20)
		{
			player->attackBoxTime = 0;
			player->attackBoxActive = false;
		}
	}

	player->gameObject->velocity.y -= player->gravity;
	if (player->gameObject->velocity.y <= -0.5f)
	{
		player->gameObject->velocity.y = -0.5f;
	}
}

void movePlayer(Player* player, Entity* ent)
{
	
	if (player->gameObject->velocity.y != 0)
	{
		glm::vec3 vel = glm::vec3(0.0f, player->gameObject->velocity.y, 0.0f);
		moveEntity(player->gameObject, vel);
		if (colliding(player->gameObject->model->collider, ent->model->collider))
		{
			GLfloat push = 0.0f;
			if (player->gameObject->velocity.y > 0 && player->gameObject->position.y < ent->position.y)
			{
				push = (ent->model->collider.minY - player->gameObject->model->collider.maxY);
			}
			else if (player->gameObject->velocity.y < 0 && player->gameObject->position.y > ent->position.y)
			{
				
				push = (ent->model->collider.maxY - player->gameObject->model->collider.minY);
				player->gameObject->onGround = true;
			}
			glm::vec3 pushVector = glm::vec3(0.0f, push, 0.0f);
			moveEntity(player->gameObject, pushVector);
			player->gameObject->velocity.y = 0.0f;
		}
	}
	if (player->gameObject->velocity.x != 0 || player->gameObject->velocity.z != 0)
	{
		if (player->gameObject->velocity.x != 0)
		{
			bool xCollision = false;
			glm::vec3 vel = glm::vec3(player->gameObject->right.x * player->gameObject->velocity.x,
				0.0f, player->gameObject->right.z * player->gameObject->velocity.x);
			moveEntity(player->gameObject, vel);


			while (colliding(player->gameObject->model->collider, ent->model->collider))
			{
				xCollision = true;
				glm::vec3 temp = glm::vec3(player->gameObject->position.x, 0.0f, player->gameObject->position.z);
				glm::vec3 temp2 = glm::vec3(ent->position.x, 0.0f, ent->position.z);
				glm::vec3 push = glm::normalize(temp - temp2)*glm::abs(player->gameObject->velocity.x);
				moveEntity(player->gameObject, push);
			}

			if (xCollision)
			{
				player->gameObject->velocity.x = 0;
			}
		}
		if (player->gameObject->velocity.z != 0)
		{
			bool zCollision = false;
			glm::vec3 vel = glm::vec3(player->gameObject->forward.x * player->gameObject->velocity.z,
				0.0f, player->gameObject->forward.z * player->gameObject->velocity.z);
			moveEntity(player->gameObject, vel);

			while (colliding(player->gameObject->model->collider, ent->model->collider))
			{
				zCollision = true;
				glm::vec3 temp = glm::vec3(player->gameObject->position.x, 0.0f, player->gameObject->position.z);
				glm::vec3 temp2 = glm::vec3(ent->position.x, 0.0f, ent->position.z);
				glm::vec3 push = glm::normalize(temp - temp2)*glm::abs(player->gameObject->velocity.z);
				moveEntity(player->gameObject, push);
			}

			if (zCollision)
			{
				player->gameObject->velocity.z = 0;
			}
		}
	}
}