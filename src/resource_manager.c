#include "resource_manager.h"


ResourceManager* createScene()
{
	ResourceManager* manager = (ResourceManager*)malloc(sizeof(ResourceManager));
	manager->numEntities = 0;
	manager->maxEntities = 50;
	manager->entityList = (Entity**)malloc(sizeof(Entity)*manager->maxEntities);
	memset(manager->entityList, 0, sizeof(Entity)*manager->maxEntities);
	manager->numLights = 0;
	manager->maxLights = 10;
	manager->lights = (Light**)malloc(sizeof(Light)*manager->maxLights);
	memset(manager->lights, 0, sizeof(Light)*manager->maxLights);
	manager->numEnemies = 0;
	return manager;
}

bool lightInUse(Light* light)
{
	if (light)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void createFramebuffer(ResourceManager* manager)
{

	glGenTextures(1, &manager->frameTexture);
	glBindTexture(GL_TEXTURE_2D, manager->frameTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);


	glGenRenderbuffers(1, &manager->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, manager->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &manager->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, manager->fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, manager->frameTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, manager->rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Incomplete Framebuffer!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &manager->quadvao);

	glGenBuffers(1, &manager->quadvbo);
}

void loadScene(ResourceManager* manager, char* path)
{
	manager->file = fopen(path, "r");
	char buf[512];
	GLfloat x = 0.0f;
	GLfloat y = 0.0f;
	GLfloat z = 0.0f;
	char texture[512];

	while (fscanf(manager->file, "%s", buf) != EOF)
	{
		if (strcmp(buf, "Entity:") == 0)
		{
			fscanf(manager->file, "%s", buf);
			Entity* ent = createEntity();
			ent->model = loadModel(buf);
			fscanf(manager->file, "%f/%f/%f", &x, &y, &z);
			fscanf(manager->file, "%f/%f/%f", &ent->color.x, &ent->color.y, &ent->color.z);
			for (GLuint i = 0; i < ent->model->numMeshes; i++)
			{
				fscanf(manager->file, "%s", buf);
				loadTexture(&ent->model->meshes[i], buf);
			}
			fscanf(manager->file, "%s", buf);
			if (strcmp(buf, "Player") == 0)
			{
				manager->player = createPlayer();
				addEntity(manager, manager->player->attackBox);
				manager->player->gameObject = ent;
				manager->player->gameObject->name = "Player";
				manager->player->gameObject->health = 100;
				moveEntity(manager->player->gameObject, glm::vec3(x, y, z));
				moveEntity(manager->player->attackBox, manager->player->gameObject->position);
			}
			else if (strcmp(buf, "Static") == 0)
			{
				ent->name = "Static";
				moveEntity(ent, glm::vec3(x, y, z));
				addEntity(manager, ent);
			}
			else if (strcmp(buf, "Goal") == 0)
			{
				ent->name = "Goal";
				moveEntity(ent, glm::vec3(x, y, z));
				addEntity(manager, ent);
			}
			else
			{
				AI* ai = createAI(10.0f);
				ent->name = "AI";
				ai->gameObject = ent;
				ai->gameObject->health = 50;
				moveEntity(ai->gameObject, glm::vec3(x, y, z));
				addEntity(manager, ai->gameObject);
				manager->numEnemies++;
			}

		}
		else if (strcmp(buf, "Light:") == 0)
		{

			Light* light = createLight();
			light->color = glm::vec3();
			light->direction = glm::vec3();
			fscanf(manager->file, "%f/%f/%f", &light->color.x, &light->color.y, &light->color.z);
			fscanf(manager->file, "%f/%f/%f", &light->direction.x, &light->direction.y, &light->direction.z);
			fscanf(manager->file, "%f", &light->range);
			fscanf(manager->file, "%f", &light->intensity);
			fscanf(manager->file, "%f/%f/%f", &x, &y, &z);
			light->gameObject = createEntity();
			light->gameObject->name = "Light";
			light->gameObject->model = loadModel("C:/Users/sharg_000/Documents/3D_Modeling_Projects/LightBulb.obj");
			loadTexture(&light->gameObject->model->meshes[0], "C:/Users/sharg_000/Documents/yellowTexture.png");
			moveEntity(light->gameObject, glm::vec3(x, y, z));
			addLight(manager, light);
		}
	}
	fclose(manager->file);
}

void addEntity(ResourceManager* manager, Entity* ent)
{
	/*manager->numEntities++;
	if (manager->numEntities == 1)
	{
		manager->entityList = (Entity**)malloc(sizeof(Entity));
	}
	else
	{
		manager->entityList = (Entity**)realloc(manager->entityList, sizeof(Entity)*manager->numEntities);
	}
	manager->entityList[manager->numEntities - 1] = ent;*/
	for (GLuint i = 0; i < manager->maxEntities; i++)
	{
		if (entityInUse(manager->entityList[i]) == false)
		{
			manager->entityList[i] = ent;
			manager->numEntities++;
			break;
		}
	}
}

void addLight(ResourceManager* manager, Light* light)
{
	/*manager->numLights++;
	if (manager->numLights == 1)
	{
		manager->lights = (Light**)malloc(sizeof(Light));
	}
	else
	{
		manager->lights = (Light**)realloc(manager->lights, sizeof(Light)*manager->numLights);
	}
	manager->lights[manager->numLights - 1] = light;*/
	for (GLuint i = 0; i < manager->maxLights; i++)
	{
		if (lightInUse(manager->lights[i]) == false)
		{
			manager->lights[i] = light;
			manager->numLights++;
			break;
		}
	}
}

void initScene(ResourceManager* manager)
{
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		initEntity(manager->entityList[i]);
	}
	for (GLuint i = 0; i < manager->numLights; i++)
	{
		initEntity(manager->lights[i]->gameObject);
	}
	initEntity(manager->player->gameObject);

}

void updateScene(ResourceManager* manager)
{
	updateEntity(manager->player->gameObject);
	updatePlayer(manager->player);
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		if (entityInUse(manager->entityList[i]) == false)
		{
			continue;
		}

		if (strcmp(manager->entityList[i]->name, "Goal") == 0)
		{
			if (colliding(manager->entityList[i]->model->collider, manager->player->gameObject->model->collider))
			{
				printf("You Win!");
			}
		}
		updateEntity(manager->entityList[i]);
		if (manager->entityList[i] != manager->player->attackBox && strcmp(manager->entityList[i]->name, "Projectile") != 0)
		{
			movePlayer(manager->player, manager->entityList[i]);

			if (strcmp(manager->entityList[i]->name, "AI") == 0)
			{
				if (manager->player->attackBoxActive == true && colliding(manager->player->attackBox->model->collider, manager->entityList[i]->model->collider))
				{
					printf("colliding");
					manager->entityList[i]->health -= 10;
					if (manager->entityList[i]->health <= 0)
					{
						freeEntity(&manager->entityList[i]);
						manager->numEntities--;
						manager->numEnemies--;
						continue;
					}
				}
				if (glm::abs(manager->player->gameObject->position.x - manager->entityList[i]->position.x) <= 20
					&& glm::abs(manager->player->gameObject->position.z - manager->entityList[i]->position.z) <= 20
					&& manager->player->hidden == false)
				{
					follow(manager->entityList[i], manager->player->gameObject->position);
				}
				if (glm::abs(manager->player->gameObject->position.x - manager->entityList[i]->position.x) <= 10
					&& glm::abs(manager->player->gameObject->position.z - manager->entityList[i]->position.z) <= 10
					&& manager->player->hidden == false)
				{
					manager->player->gameObject->health -= 15;
				}

			}

			if (manager->player->numProjectiles != 0)
			{
				for (GLuint j = 0; j < manager->player->numProjectiles; j++)
				{
					if (manager->player->projectiles[j].active == true)
					{
						manager->player->projectiles[j].active = false;
						addEntity(manager, manager->player->projectiles[j].gameObject);
						initEntity(manager->player->projectiles[j].gameObject);
					}
					else
					{
						moveEntity(manager->player->projectiles[j].gameObject, manager->player->projectiles[j].gameObject->velocity);
						//manager->player->projectiles[j].gameObject->velocity.y -= 0.001f;
						if (colliding(manager->player->projectiles[j].gameObject->model->collider,
							manager->entityList[i]->model->collider) && strcmp(manager->entityList[i]->name, "AI") == 0)
						{
							freeEntity(&manager->entityList[i]);
							manager->numEntities--;
						}

					}
				}
			}
		}
	}
	manager->player->hidden = true;
	for (GLuint i = 0; i < manager->numLights; i++)
	{
		GLfloat dotProduct = glm::dot(manager->lights[i]->direction, glm::normalize(manager->lights[i]->gameObject->position -
			manager->player->gameObject->position));
		if (dotProduct <= -manager->lights[i]->range)
		{
			manager->player->hidden = false;
		}
	}

	/*for (GLuint i = 0; i < manager->numSpawners; i++)
	{

		updateParticles(&manager->spawner[i]);
	}*/
	if (manager->player->hidden == true)
	{
		//printf("Player is hidden");
	}
}

void renderToonScene(ResourceManager* manager)
{
	/*GLint uniLocation = glGetUniformLocation(get_toon_shader(), "modelMatrix");
	glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->player->gameObject->modelMatrix));
	drawEntity(manager->player->gameObject);*/
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		if (manager->entityList[i] != manager->player->attackBox && entityInUse(manager->entityList[i]))
		{
			GLint uniLocation = glGetUniformLocation(get_toon_shader(), "modelMatrix");
			glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->entityList[i]->modelMatrix));

			GLint uniColor = glGetUniformLocation(get_toon_shader(), "objectColor");
			glUniform4fv(uniColor, 1, (const GLfloat*)&manager->entityList[i]->color);
			drawEntity(manager->entityList[i]);
		}
	}
	renderLights(manager, get_toon_shader());
}

void renderScene(ResourceManager* manager)
{
	//GLint uniLocation = glGetUniformLocation(get_phong_shader(), "modelMatrix");
	//glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->player->gameObject->modelMatrix));
	//drawEntity(manager->player->gameObject);
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		if (manager->entityList[i] != manager->player->attackBox && entityInUse(manager->entityList[i]))
		{
			GLint uniLocation = glGetUniformLocation(get_phong_shader(), "modelMatrix");
			glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->entityList[i]->modelMatrix));

			GLint uniColor = glGetUniformLocation(get_phong_shader(), "objectColor");
			glUniform4fv(uniColor, 1, (const GLfloat*)&manager->entityList[i]->color);

			drawEntity(manager->entityList[i]);
		}
	}
	renderLights(manager, get_phong_shader());
}

void renderLights(ResourceManager* manager, GLuint shader)
{
	for (GLuint i = 0; i < manager->numLights; i++)
	{
		GLint uniLocation = glGetUniformLocation(shader, "modelMatrix");
		glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->lights[i]->gameObject->modelMatrix));

		drawEntity(manager->lights[i]->gameObject);
	}
}

void renderXrayScene(ResourceManager* manager)
{
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		if (entityInUse(manager->entityList[i]) == false)
		{
			continue;
		}
		if (manager->entityList[i]->name == "AI")
		{
			GLint uniLocation = glGetUniformLocation(get_xray_shader(), "modelMatrix");
			glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->entityList[i]->modelMatrix));
			drawEntity(manager->entityList[i]);
		}
	}
}

void renderPostProcessEffects(ResourceManager* manager)
{

	GLfloat verts[24] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	glBindVertexArray(manager->quadvao);
	glBindBuffer(GL_ARRAY_BUFFER, manager->quadvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindTexture(GL_TEXTURE_2D, manager->frameTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

void freeScene(ResourceManager* manager)
{
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		//freeEntity(manager->entityList[i]);
	}
	//freeEntity(&(manager->player->gameObject));
}