#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "entity.h"
#include "player.h"
#include "light.h"

typedef struct
{
	Player* player;
	Entity** entityList;
	Light** lights;
	GLuint numEntities;
	GLuint numLights;
	FILE* file;
	GLuint fbo;
	GLuint rbo;
	GLuint quadvao;
	GLuint quadvbo;
	GLuint frameTexture;
}ResourceManager;

ResourceManager* createScene();
void createFramebuffer(ResourceManager* manager);
void initScene(ResourceManager* manager);
void updateScene(ResourceManager* manager);
void renderLights(ResourceManager* manager, GLuint shader);
void renderScene(ResourceManager* manager);
void renderToonScene(ResourceManager* manager);
void renderPostProcessEffects(ResourceManager* manager);
void freeScene(ResourceManager* manager);
void loadScene(ResourceManager* manager, char* path);
void addEntity(ResourceManager* manager, Entity* ent);
void addLight(ResourceManager* manager, Light* light);
void renderXrayScene(ResourceManager* manager);

