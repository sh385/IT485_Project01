#include "particle_spawner.h"

ParticleSpawner* createParticles(glm::vec3 position)
{
	ParticleSpawner* spawner = (ParticleSpawner*)malloc(sizeof(ParticleSpawner));
	spawner->position = position;
	spawner->numParticles = 100;
	spawner->numIndices = 6;
	spawner->indices = (GLuint*)malloc(sizeof(GLuint)*6);
	spawner->indices[0] = 0;
	spawner->indices[1] = 1;
	spawner->indices[2] = 2;
	spawner->indices[3] = 2;
	spawner->indices[4] = 1;
	spawner->indices[5] = 3;

	spawner->particles = (Particle*)malloc(sizeof(Particle)*spawner->numParticles);
	spawner->vertices = (glm::vec3*)malloc(sizeof(glm::vec3)*spawner->numParticles*4);
	return spawner;
}

void initParticles(ParticleSpawner* spawner)
{
	GLuint temp = 0;
	for (GLuint i = 0; i < spawner->numParticles; i++)
	{
		GLfloat size = 0.05f;
		spawner->particles[i].size = size;
		spawner->particles[i].timeToLive = (rand() % 60) + 1;
		spawner->particles[i].velocity = glm::vec3((rand()*-0.000001f) + (rand()*0.000001f), (rand()*-0.000001f) + (rand()*0.000001f), (rand()*-0.000001f) + (rand()*0.000001f));
		spawner->particles[i].position = spawner->position;
		spawner->vertices[temp] = glm::vec3(spawner->particles[i].position.x - size, spawner->particles[i].position.y + size, 0.0f);
		spawner->vertices[temp + 1] = glm::vec3(spawner->particles[i].position.x + size, spawner->particles[i].position.y + size, 0.0f);
		spawner->vertices[temp + 2] = glm::vec3(spawner->particles[i].position.x - size, spawner->particles[i].position.y - size, 0.0f);
		spawner->vertices[temp + 3] = glm::vec3(spawner->particles[i].position.x + size, spawner->particles[i].position.y - size, 0.0f);
		spawner->particles[i].modelMatrix = glm::mat4();
		temp += 4;
	}
	glGenVertexArrays(1, &spawner->vao);
	glGenBuffers(1, &spawner->vbo);
	glGenBuffers(1, &spawner->ebo);
}

void follow(ParticleSpawner* spawner, glm::vec3 target)
{
	spawner->position += glm::normalize(target - spawner->position) * 0.05f;
	for (GLuint i = 0; i < spawner->numParticles; i++)
	{
		if (spawner->particles[i].velocity == glm::vec3(0.0f, 0.0f, 0.0f))
		{
			spawner->particles[i].velocity = glm::vec3(rand()*-0.00001f, (rand()*-0.000001f) + (rand()*0.000001f), 
				(rand()*-0.000001f) + (rand()*0.000001f));
		}
		spawner->particles[i].position += spawner->particles[i].velocity;
		spawner->particles[i].timeToLive--;
		if (spawner->particles[i].timeToLive <= 0)
		{
			spawner->particles[i].timeToLive = (rand() % 60) + 1;
			spawner->particles[i].position = spawner->position;
		}
		spawner->particles[i].modelMatrix = glm::translate(glm::mat4(), spawner->particles[i].position);
	}
}

void updateParticles(ParticleSpawner* spawner)
{
	for (GLuint i = 0; i < spawner->numParticles; i++)
	{
		spawner->particles[i].modelMatrix = glm::translate(spawner->particles[i].modelMatrix, spawner->particles[i].position + spawner->particles[i].velocity);
	}
}

void drawParticles(ParticleSpawner* spawner)
{
	glBindVertexArray(spawner->vao);
	glBindBuffer(GL_ARRAY_BUFFER, spawner->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spawner->ebo);
	glEnableVertexAttribArray(0);
	for (GLuint i = 0; i < spawner->numParticles; i++)
	{
		GLint uniLocation = glGetUniformLocation(get_particle_shader(), "modelMatrix");
		glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(spawner->particles[i].modelMatrix));

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, &spawner->vertices[i*4], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*spawner->numIndices, &spawner->indices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawElements(GL_TRIANGLES, spawner->numIndices, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}