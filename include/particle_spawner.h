#pragma once
#include "entity.h"
typedef struct
{
	glm::vec3 position;
	GLint timeToLive;
	glm::vec3 velocity;
	GLfloat size;
	glm::mat4 modelMatrix;
}Particle;

typedef struct
{
	glm::vec3 position;
	Particle* particles;
	GLuint numParticles;
	GLuint numIndices;
	GLuint vao, vbo, ebo;
	glm::vec3* vertices;
	GLuint* indices;
}ParticleSpawner;

ParticleSpawner* createParticles();
void initParticles(ParticleSpawner* spawner);
void updateParticles(ParticleSpawner* spawner);
void drawParticles(ParticleSpawner* spawner);
void follow(ParticleSpawner* spawner, glm::vec3 target);