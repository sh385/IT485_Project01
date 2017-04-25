#pragma once
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

typedef struct
{
	glm::vec3 position;
	glm::vec2 texel;
	glm::vec3 normal;
}Vertex;

typedef struct
{
	Vertex* vertices;
	GLuint* indices;
	GLuint numIndices;
	GLuint numVertices;
	GLuint polycount;

	SDL_Surface* texture;
	GLuint vao, vbo, ebo, texID;
} Mesh;

void freeMesh(Mesh** mesh);
void drawMesh(Mesh* mesh);
void loadTexture(Mesh* mesh, char* texturePath);
void initMesh(Mesh* mesh);



