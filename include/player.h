#pragma once
#include "entity.h"
#include "projectile.h"

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
	GLuint currentWeapon;
	Projectile* projectiles;
	GLuint numProjectiles;
	GLuint maxProjectiles;
	GLint shotCounter;
	GLint maxShotCount;
}Player;

Player* createPlayer();
void updatePlayer(Player* player);
void movePlayer(Player* player, Entity* ent);