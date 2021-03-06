#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>			// holds scene data that importer places values in
#include <assimp/postprocess.h>

// Meshes and textures are stored inside models
#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();

private:
	void LoadNode(aiNode* node, const aiScene* scene);	// node stores specific meshID, scene stores mesh data
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);				

	// simplistic implementation: meshes and textures stored directly inside models
	// as opposed to being stored externally and referenced by the models
	std::vector<Mesh*> meshList;			// meshes may use shared textures
	std::vector<Texture*> textureList;		// only unique textures are stored
	std::vector<unsigned int> meshToTex;	// shared textures are referenced vie meshToTex
};

