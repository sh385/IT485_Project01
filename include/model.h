#pragma once
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "simple_logger.h"
#include "mesh.h"

#include "mesh.h"

struct Collider
{
	GLuint width;
	GLuint height;
	GLuint depth;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat minX;
	GLfloat minY;
	GLfloat minZ;
	GLfloat maxX;
	GLfloat maxY;
	GLfloat maxZ;
};

typedef struct
{
	Mesh* meshes;
	GLuint numMeshes;
	Collider collider;
}Model;

void drawModel(Model* model);
void initModel(Model* model);
Model* loadModel(char* filePath);