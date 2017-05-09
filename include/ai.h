#pragma once
#include "entity.h"

typedef struct
{
	glm::vec3 position;
	glm::vec3 parent;
	glm::vec3 neighbors;
	GLfloat gScore;
	GLfloat hScore;
	GLfloat fScore;
}NavNode;

typedef struct
{
	NavNode* nodes;
	GLuint numNodes;
}NavMesh;

typedef struct
{
	Entity* gameObject;
	GLfloat gravity;
	//(1) Patrol, (2) Chase, (3) Attack, (4) Investigate, (5) Search
	GLuint states[5];
	GLuint state;
	NavMesh* navmesh;
	GLuint pathSize;
	GLfloat lookDistance;
}AI;
AI* createAI(GLfloat lookDistance);
NavMesh* createNavmesh(char* filePath);
void updateAI(AI* player);
void moveAI(AI* player, Entity* ent);
void follow(Entity* ai, glm::vec3 target);
NavNode getNodeAt(NavMesh* navmesh, glm::vec3 position);
bool nodeIsLocatedAt(NavNode node, glm::vec3 position);
NavNode* neighborsOf(NavMesh* navmesh, glm::vec3 nodePosition);
bool insideOf(NavNode value, NavNode* list, GLuint sizeOfList);