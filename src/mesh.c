#include <stdlib.h>
#include <stdio.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "simple_logger.h"
#include "mesh.h"

void loadTexture(Mesh* mesh, char* texturePath)
{
	if (!texturePath)
	{
		slog("No path provided");
		return;
	}
	SDL_Surface* texture;
	texture = IMG_Load(texturePath);
	mesh->texture = texture;
	if (!texture)
	{
		printf("Error loading texture");
	}
}


void initMesh(Mesh* mesh)
{
	glGenTextures(1, &mesh->texID);
	glBindTexture(GL_TEXTURE_2D, mesh->texID);
 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh->texture->w, mesh->texture->h, 0, GL_RGB, GL_UNSIGNED_BYTE, mesh->texture->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);
}

void freeMesh(Mesh** mesh)
{
	if (!mesh)
	{
		slog("freeMesh: No mesh pointer provided to deallocate");
		return;
	}
	Mesh* m;
	m = *mesh;

	if (!m)
	{
		slog("freeMesh: No mesh data provided");
		return;
	}

	if (m->vertices)
	{
		free(m->vertices);
	}

	if (m->indices)
	{
		free(m->indices);
	}

	*mesh = NULL;
}

void drawMesh(Mesh* mesh)
{	
	glBindTexture(GL_TEXTURE_2D, mesh->texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindVertexArray(mesh->vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mesh->numVertices, &mesh->vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->numIndices, &mesh->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texel));

	glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
