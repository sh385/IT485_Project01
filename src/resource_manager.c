#include "resource_manager.h"


ResourceManager* createScene()
{
	ResourceManager* manager = (ResourceManager*)malloc(sizeof(ResourceManager));
	manager->numEntities = 0;
	manager->numLights = 0;
	return manager;
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
			printf("%d\n", ent->model->numMeshes);
			for (GLuint i = 0; i < ent->model->numMeshes; i++)
			{
				fscanf(manager->file, "%s", buf);
				loadTexture(&ent->model->meshes[i], buf);
			}
			fscanf(manager->file, "%s", buf);
			if (strcmp(buf, "Player") == 0)
			{
				manager->player = createPlayer();
				manager->player->gameObject = ent;
				manager->player->gameObject->name = "Player";
				moveEntity(manager->player->gameObject, glm::vec3(manager->player->gameObject->position.x + x, 
					manager->player->gameObject->position.y + y, manager->player->gameObject->position.z + z));
			}
			else if (strcmp(buf, "Static") == 0)
			{
				ent->name = "Static";
				moveEntity(ent, glm::vec3(ent->position.x + x, ent->position.y + y, ent->position.z + z));
				addEntity(manager, ent);
			}
			else
			{
				ent->name = "X-Ray";
				moveEntity(ent, glm::vec3(ent->position.x + x, ent->position.y + y, ent->position.z + z));
				addEntity(manager, ent);
			}
		}
		else if (strcmp(buf, "Light:") == 0)
		{

			Light* light = createLight();
			light->color = glm::vec3();
			fscanf(manager->file, "%f/%f/%f", &light->color.x, &light->color.y, &light->color.z);
			printf("%f %f %f\n", light->color.x, light->color.y, light->color.z);
			fscanf(manager->file, "%f", &light->intensity);
			fscanf(manager->file, "%f/%f/%f", &x, &y, &z);
			light->gameObject = createEntity();
			light->gameObject->model = loadModel("C:/Users/sharg_000/Documents/3D_Modeling_Projects/LightBulb.obj");
			loadTexture(&light->gameObject->model->meshes[0], "C:/Users/sharg_000/Documents/yellowTexture.png");
			moveEntity(light->gameObject, glm::vec3(light->gameObject->position.x + x, 
				light->gameObject->position.y + y, light->gameObject->position.z + z));
			addLight(manager, light);
		}
	}
	fclose(manager->file);
}

void addEntity(ResourceManager* manager, Entity* ent)
{
	manager->numEntities++;
	if (manager->numEntities == 1)
	{
		manager->entityList = (Entity**)malloc(sizeof(Entity));
	}
	else
	{
		manager->entityList = (Entity**)realloc(manager->entityList, sizeof(Entity)*manager->numEntities);
	}
	manager->entityList[manager->numEntities - 1] = ent;
}

void addLight(ResourceManager* manager, Light* light)
{
	manager->numLights++;
	if (manager->numLights == 1)
	{
		manager->lights = (Light**)malloc(sizeof(Light));
	}
	else
	{
		manager->lights = (Light**)realloc(manager->lights, sizeof(Light)*manager->numLights);
	}
	manager->lights[manager->numLights - 1] = light;
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
		updateEntity(manager->entityList[i]);
		movePlayer(manager->player, manager->entityList[i]);
	}
}

void renderToonScene(ResourceManager* manager)
{
	//GLint uniLocation = glGetUniformLocation(get_toon_shader(), "modelMatrix");
	//glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->player->gameObject->modelMatrix));
	//drawEntity(manager->player->gameObject);
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		GLint uniLocation = glGetUniformLocation(get_toon_shader(), "modelMatrix");
		glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->entityList[i]->modelMatrix));
		drawEntity(manager->entityList[i]);
	}
	renderLights(manager, get_toon_shader());
}

void renderScene(ResourceManager* manager)
{
	//GLint uniLocation = glGetUniformLocation(get_toon_shader(), "modelMatrix");
	//glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->player->gameObject->modelMatrix));
	//drawEntity(manager->player->gameObject);
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		GLint uniLocation = glGetUniformLocation(get_phong_shader(), "modelMatrix");
		glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->entityList[i]->modelMatrix));
		drawEntity(manager->entityList[i]);
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
		if (manager->entityList[i]->name == "X-Ray")
		{
			GLint uniLocation = glGetUniformLocation(get_xray_shader(), "modelMatrix");
			glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(manager->entityList[i]->modelMatrix));
			drawEntity(manager->entityList[i]);
		}
	}
	//drawEntity(manager->player->gameObject);
}


void freeScene(ResourceManager* manager)
{
	for (GLuint i = 0; i < manager->numEntities; i++)
	{
		//freeEntity(manager->entityList[i]);
	}
	//freeEntity(&(manager->player->gameObject));
}