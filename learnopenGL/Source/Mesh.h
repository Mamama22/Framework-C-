#ifndef MESH_H
#define MESH_H

#include "Header.h"
#include "Vertex.h"

/*************************************************************
New and improved mesh

Author: Tan Yie Cher
Date: 23/6/2016

Note:
-All mesh can share the same VAO
/*************************************************************/
class Mesh
{
	static int currentMesh;	//Keeps track of last rendered mesh ID
	static int totalMeshes;
	int meshID;	

public:

	//Data-------------------------------------//
	unsigned vertexBuffer;
	unsigned m_vertexArrayID;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned textureID;
	unsigned mode;

	//CD--------------------------------//
	Mesh();
	~Mesh();

	//Core------------------------------//
	void Render();
};

#endif