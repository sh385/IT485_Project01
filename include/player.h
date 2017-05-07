#pragma once
#include "entity.h"

typedef struct
{
	Entity* gameObject;
	GLfloat gravity;
	GLfloat x;
	GLfloat z;
	Entity* attackBox;
	bool attackBoxActive;
	GLint attackBoxTime;
	bool hidden;
}Player;

Player* createPlayer();
void updatePlayer(Player* player);
void movePlayer(Player* player, Entity* ent);