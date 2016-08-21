#include "Mesh.h"
int Mesh::totalMeshes = 0;
int Mesh::currentMesh = -1;

void Mesh::ResetCurrentMesh()
{
	//reset, as Text and other renders will bind incompatible VAO, so even if currentMesh is same is this mesh ID, VAO will not be the same
	currentMesh = -1;
}

/********************************************************************************
CD
********************************************************************************/
Mesh::Mesh()
{
	//generate buffers------------------------//
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	indexSize = 0;
	textureID = (TEXTURE_ENUM)-1;

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

void Mesh::SetTexture(TEXTURE_ENUM id)
{
	this->textureID = id;
}

int Mesh::GetTextureID(){ return this->textureID; }

/********************************************************************************
Render
********************************************************************************/
void Mesh::Render()
{
	//If rendering this mesh for first time this frame, bind it's VAO
	if (currentMesh != meshID)
	{
		glBindVertexArray(m_vertexArrayID);
	}

	currentMesh = meshID;

	//Bind VAO---------------------------------------------//
	glDrawElements(mode, indexSize, GL_UNSIGNED_INT, 0);
}

/********************************************************************************
Render
********************************************************************************/
void Mesh::Render(unsigned offset, unsigned count)
{
	//If rendering this mesh for first time this frame, bind it's VAO
	if (currentMesh != meshID)
	{
		glBindVertexArray(m_vertexArrayID);
	}

	currentMesh = meshID;

	//Bind VAO---------------------------------------------//
	glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
}