#ifndef MESH_H
#define MESH_H

#include "Header.h"
#include "Vertex.h"

/*************************************************************
New and improved mesh

Author: Tan Yie Cher
Date: 23/6/2016

Note:
-IMPORTANT: render mesh of this type without binding to any other external VAO, ie. text VAO!!!
/*************************************************************/
class Mesh
{
protected:

	static int currentMesh;	//Keeps track of last rendered mesh ID
	static int totalMeshes;
	int meshID;
	int textureID;	//can be used for normal textures or tilemap

public:

	//Data-------------------------------------//
	unsigned vertexBuffer;
	unsigned m_vertexArrayID;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned mode;

	//CD--------------------------------//
	Mesh();
	~Mesh();

	//Setup-------------------------------//
	void Setup(std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, unsigned drawMode);
	void SetupVertexOnly(std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, unsigned drawMode);

	//Core------------------------------//
	virtual void Render(float alpha = 1.f);
	virtual void Render(unsigned offset, unsigned count, float alpha = 1.f);

	static void ResetCurrentMesh();	//call

	void SetTexture(TEXTURE_ENUM id);
	int GetTextureID();
};

class Mesh_Tilemap : public Mesh
{
public:
	
	//Core------------------------------//
	virtual void Render(float alpha = 1.f);
	virtual void Render(unsigned offset, unsigned count, float alpha = 1.f);

	void SetTilemapTex(TILEMAP_ENUM id);
	int GetTilemapTex();
};

#endif