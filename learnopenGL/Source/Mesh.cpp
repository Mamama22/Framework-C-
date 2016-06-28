#include "Mesh.h"
int Mesh::totalMeshes = 0;
int Mesh::currentMesh = -1;

/********************************************************************************
CD
********************************************************************************/
Mesh::Mesh()
{
	//generate buffers------------------------//
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	textureID = indexSize = 0;

	//Vertex Attrib obj : shader normal and basic--------------------------------------------------------//
	glGenVertexArrays(1, &m_vertexArrayID);	//normal

	//ID----------------------------------------//
	meshID = totalMeshes++;
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

/********************************************************************************
Render
********************************************************************************/
void Mesh::Render()
{
	//If rendering this mesh for first time this frame, bind it's VAO
	if (currentMesh != meshID)
		glBindVertexArray(m_vertexArrayID);

	currentMesh = meshID;

	//Bind VAO---------------------------------------------//
	glDrawElements(mode, indexSize, GL_UNSIGNED_INT, 0);
}