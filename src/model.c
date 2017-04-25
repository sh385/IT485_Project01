#include "model.h"

Model* loadModel(char* filePath)
{
	if (!filePath)
	{
		slog("No file path provided");
		return NULL;
	}
	Model* model;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ASSIMP ERROR: %s", importer.GetErrorString());
		return NULL;
	}

	GLuint numIndices = 0;
	model = (Model*)malloc(sizeof(Model));
	model->meshes = (Mesh*)malloc(sizeof(Mesh) * scene->mNumMeshes);
	model->numMeshes = scene->mNumMeshes;
	GLfloat minX = 0;
	GLfloat minY = 0;
	GLfloat minZ = 0;
	GLfloat maxX = 0;
	GLfloat maxY = 0;
	GLfloat maxZ = 0;
	
	aiMesh* currentMesh;

	//Meshes
	for (GLuint m = 0; m < model->numMeshes; m++)
	{
		numIndices = 0;
		model->meshes[m].numVertices = scene->mMeshes[m]->mNumVertices;
		model->meshes[m].polycount = scene->mMeshes[m]->mNumFaces;
		model->meshes[m].vertices = (Vertex*)malloc(sizeof(Vertex) * model->meshes[m].numVertices);
		model->meshes[m].indices = (GLuint*)malloc(sizeof(GLuint) * model->meshes[m].polycount * 3);
		model->meshes[m].numIndices = model->meshes[m].polycount * 3;
		aiString name;
		aiGetMaterialString(scene->mMaterials[scene->mMeshes[m]->mMaterialIndex], AI_MATKEY_NAME, &name);

		//Vertices, texels, and normals
		for (GLuint v = 0; v < model->meshes[m].numVertices; v++)
		{
			model->meshes[m].vertices[v].position.x = scene->mMeshes[m]->mVertices[v].x;
			model->meshes[m].vertices[v].position.y = scene->mMeshes[m]->mVertices[v].y;
			model->meshes[m].vertices[v].position.z = scene->mMeshes[m]->mVertices[v].z;
			if (model->meshes[m].vertices[v].position.x < minX)
			{
				minX = model->meshes[m].vertices[v].position.x;
			}
			if (model->meshes[m].vertices[v].position.x > maxX)
			{
				maxX = model->meshes[m].vertices[v].position.x;
			}
			if (model->meshes[m].vertices[v].position.y < minY)
			{
				minY = model->meshes[m].vertices[v].position.y;
			}
			if (model->meshes[m].vertices[v].position.y > maxY)
			{
				maxY = model->meshes[m].vertices[v].position.y;
			}
			if (model->meshes[m].vertices[v].position.z < minZ)
			{
				minZ = model->meshes[m].vertices[v].position.z;
			}
			if (model->meshes[m].vertices[v].position.z > maxZ)
			{
				maxZ = model->meshes[m].vertices[v].position.z;
			}
			

			if (scene->mMeshes[m]->mTextureCoords[0])
			{
				model->meshes[m].vertices[v].texel.x = scene->mMeshes[m]->mTextureCoords[0][v].x;
				model->meshes[m].vertices[v].texel.y = scene->mMeshes[m]->mTextureCoords[0][v].y;
			}

			model->meshes[m].vertices[v].normal.x = scene->mMeshes[m]->mNormals[v].x;
			model->meshes[m].vertices[v].normal.y = scene->mMeshes[m]->mNormals[v].y;
			model->meshes[m].vertices[v].normal.z = scene->mMeshes[m]->mNormals[v].z;
		}
		//Faces, and indices
		for (GLuint f = 0; f < scene->mMeshes[m]->mNumFaces; f++)
		{
			for (GLuint i = 0; i < scene->mMeshes[m]->mFaces[f].mNumIndices; i++)
			{
				model->meshes[m].indices[numIndices] = scene->mMeshes[m]->mFaces[f].mIndices[i];
				numIndices++;
			}
		}
	}
	model->collider.minX = minX;
	model->collider.minY = minY;
	model->collider.minZ = minZ;
	model->collider.maxX = maxX;
	model->collider.maxY = maxY;
	model->collider.maxZ = maxZ;
	model->collider.x = minX + ((maxX - minX) / 2);
	model->collider.y = minY + ((maxY - minY) / 2);
	model->collider.z = minZ + ((maxZ - minZ) / 2);
	return model;
}

void initModel(Model* model)
{
	for (GLuint i = 0; i < model->numMeshes; i++)
	{
		initMesh(&model->meshes[i]);
	}
}

void drawModel(Model* model)
{
	for (GLuint i = 0; i < model->numMeshes; i++)
	{
		drawMesh(&model->meshes[i]);
	}
}

void freeModel(Model** model)
{
	Model* m;
	m = *model;

	if (!m)
	{
		slog("freeModel: No model provided");
		return;
	}

	if (m->meshes)
	{
		free(m->meshes);
	}

	*model = NULL;
}