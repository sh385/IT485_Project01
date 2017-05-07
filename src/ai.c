#include "ai.h"
#include "math.h"

NavMesh* createNavmesh(char* filePath)
{
	NavMesh* navmesh = (NavMesh*)malloc(sizeof(NavMesh));
	navmesh->numNodes = 0;

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
			navmesh->nodes[((i + 1) / 4) - 1].position.x = sumX;
			navmesh->nodes[((i + 1) / 4) - 1].position.y = 0.0f;
			navmesh->nodes[((i + 1) / 4) - 1].position.z = sumZ;
			counter = 0;
			navmesh->numNodes++;
			sumX = 0.0f;
			sumZ = 0.0f;
		}
	}

	return navmesh;
}

AI* createAI(NavMesh* navmesh, GLfloat lookDistance)
{
	AI* ai = (AI*)malloc(sizeof(AI));
	ai->navmesh = navmesh;
	ai->pathSize = 0;
	ai->lookDistance = lookDistance;
	return ai;
}

void follow(AI* ai, glm::vec3 target)
{
	if (ai->gameObject->position.x > target.x)
	{
		moveEntity(ai->gameObject, glm::vec3(-0.06f, 0.0f, 0.0f));
	}
	else if (ai->gameObject->position.x < target.x)
	{
		moveEntity(ai->gameObject, glm::vec3(0.06, 0.0f, 0.0f));
	}

	if (ai->gameObject->position.z > target.z)
	{
		moveEntity(ai->gameObject, glm::vec3(0.0f, 0.0f, -0.06f));
	}
	else if (ai->gameObject->position.z < target.z)
	{
		moveEntity(ai->gameObject, glm::vec3(0.0f, 0.0f, 0.06f));
	}
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

	if (glm::length(temp2 - temp) <= 1.0f)
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
		if (counter < 8)
		{
			if (glm::length(navmesh->nodes[i].position - nodePosition) <= 2.7f && navmesh->nodes[i].position != nodePosition)
			{
				nodes[counter].position = navmesh->nodes[i].position;
				counter++;
			}
		}
		
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