#include "ai.h"
#include "math.h"

NavMesh* createNavmesh(char* filePath)
{
	NavMesh* navmesh = (NavMesh*)malloc(sizeof(NavMesh));
	navmesh->numNodes = 0;
	//ai->navmesh = navmesh;
	if (!filePath)
	{
		slog("No file path provided");
		return NULL;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	navmesh->nodes = (NavNode*)malloc(sizeof(NavNode)*(scene->mMeshes[0]->mNumVertices/4));

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ASSIMP ERROR: %s", importer.GetErrorString());
		return NULL;
	}
	GLuint counter = 0;
	GLfloat sumX = 0.0f;
	GLfloat sumZ = 0.0f;
	for (GLuint i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
	{
		sumX += scene->mMeshes[0]->mVertices[i].x;
		sumZ += scene->mMeshes[0]->mVertices[i].z;
		counter++;
		if (counter == 4)
		{
			sumX = sumX / 4.0f;
			sumZ = sumZ / 4.0f;
			navmesh->nodes[((i + 1) / 4) - 1].position.x = round(sumX);
			navmesh->nodes[((i + 1) / 4) - 1].position.y = 0.0f;
			navmesh->nodes[((i + 1) / 4) - 1].position.z = round(sumZ);
			counter = 0;
			navmesh->numNodes++;
			sumX = 0.0f;
			sumZ = 0.0f;
		}
	}

	return navmesh;
}

AI* createAI(NavMesh* navmesh)
{
	AI* ai = (AI*)malloc(sizeof(AI));
	ai->navmesh = navmesh;
	ai->pathSize = 0;
	return ai;
}

NavNode* findPath(AI* ai, Entity* target)
{
	NavNode currentNode = getNodeAt(ai->navmesh, glm::vec3(0.0f, 0.0f, -10.0f));
	GLuint openListSize = 1;
	NavNode* openList = (NavNode*)malloc(sizeof(NavNode)*ai->navmesh->numNodes*8);

	GLuint closedListSize = 0;
	NavNode* closedList = (NavNode*)malloc(sizeof(NavNode)*ai->navmesh->numNodes*8);

	NavNode* cameFrom = (NavNode*)malloc(sizeof(NavNode)*ai->navmesh->numNodes*8);
	GLuint cameFromSize = 1;
	cameFrom[0] = currentNode;
	cameFrom[0].parent = glm::vec3();
	cameFrom[0].hScore = glm::length(currentNode.position - target->position);
	cameFrom[0].fScore = cameFrom[0].hScore;
	cameFrom[0].gScore = 0.0f;
	GLfloat tempGscore = 0.0f;
	openList[openListSize-1] = currentNode;
	while (openListSize != 0)
	{
		currentNode = openList[openListSize - 1];
		if (nodeIsLocatedAt(currentNode, target->position))
		{
			ai->pathSize = closedListSize;
			return closedList;
		}
		openListSize--;
		closedListSize++;
		closedList[closedListSize-1] = currentNode;
		NavNode* neighbors = neighborsOf(ai->navmesh, currentNode.position);
		for (GLuint i = 0; i < 8; i++)
		{
			printf("%f\n", neighbors[i].position.x);
			if (neighbors[i].position == glm::vec3())
			{
				continue;
			}
			else
			{
				tempGscore = cameFrom[0].gScore + glm::length(currentNode.position - neighbors[i].position);
				if (!insideOf(neighbors[i], openList, openListSize))
				{
					openListSize++;
					openList[openListSize - 1] = neighbors[i];
				}
				else if (tempGscore >= glm::length(currentNode.position - neighbors[i].position))
				{
					continue;
				}
				cameFromSize++;
				neighbors[i].parent = currentNode.position;
				neighbors[i].gScore = tempGscore;
				neighbors[i].fScore = tempGscore + glm::length(neighbors[i].position - target->position);
				cameFrom[cameFromSize - 1] = neighbors[i];
			}
		}
	}
	return closedList;
}

NavNode getNodeAt(NavMesh* navmesh, glm::vec3 position)
{
	NavNode node;
	GLuint counter = 0;
	GLfloat amount = 0.0f;
	glm::vec3 temp = glm::vec3(position.x, 0.0f, position.z);
	for (GLuint i = 0; i < navmesh->numNodes; i++)
	{
		if (glm::length(navmesh->nodes[i].position - temp) <= 1.0f && counter == 0)
		{
			node = navmesh->nodes[i];
			amount = glm::length(navmesh->nodes[i].position - temp);
			counter++;
		}
		else if (glm::length(navmesh->nodes[i].position - temp) <= amount)
		{
			node = navmesh->nodes[i];
		}
	}
	return node;
}

bool nodeIsLocatedAt(NavNode node, glm::vec3 position)
{
	glm::vec3 temp = glm::vec3(position.x, 0.0f, position.z);
	glm::vec3 temp2 = glm::vec3(node.position.x, 0.0f, node.position.z);

	if (glm::length(temp2 - temp) <= 5.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

NavNode* neighborsOf(NavMesh* navmesh, glm::vec3 nodePosition)
{
	NavNode* nodes = (NavNode*)malloc(sizeof(NavNode)*8);
	GLuint counter = 0;
	for (GLuint i = 0; i < navmesh->numNodes; i++)
	{
		if (counter < 7)
		{
			if (glm::length(navmesh->nodes[i].position - nodePosition) <= 1.0f)
			{
				nodes[i].position = navmesh->nodes[i].position;
			}
		}
		counter++;
	}
	return nodes;
}

bool insideOf(NavNode value, NavNode* list, GLuint sizeOfList)
{
	for (GLuint i = 0; i < sizeOfList; i++)
	{
		if (value.position == list[sizeOfList].position)
		{
			return true;
		}
	}
	return false;
}