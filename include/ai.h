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
}AI;
AI* createAI(NavMesh* navmesh);
NavMesh* createNavmesh(char* filePath);
NavNode* findPath(AI* ai, Entity* target);
void updateAI(AI* player);
void moveAI(AI* player, Entity* ent);
NavNode getNodeAt(NavMesh* navmesh, glm::vec3 position);
bool nodeIsLocatedAt(NavNode node, glm::vec3 position);
NavNode* neighborsOf(NavMesh* navmesh, glm::vec3 nodePosition);
bool insideOf(NavNode value, NavNode* list, GLuint sizeOfList);