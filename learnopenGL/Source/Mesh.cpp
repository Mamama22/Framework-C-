#include "Mesh.h"
#include "CoreUtilities.h"
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
Setup mesh
********************************************************************************/
void Mesh::Setup(std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, unsigned drawMode)
{
	//Bind VAO------------------------------------------------------------------//
	glBindVertexArray(m_vertexArrayID);

	//vertices---------------------------------------------------------------------//
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	//indexe---------------------------------------------------------------------//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//Attributes------------------------------------------------------------------------------------//
	//All mesh can share the same VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);	//Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));	//Color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position)+sizeof(Color)));	//normal
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position)+sizeof(Color)+sizeof(Vector3)));	//texcoord


	indexSize = index_buffer_data.size();
	mode = drawMode;

	//Unbind-----------------------------------------------------------------//
	glBindVertexArray(0);	//VAO stores unbind calls too, UNBIND BEFORE UNBINDING BUFFERS
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Mesh::SetupVertexOnly(std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, unsigned drawMode)
{
	//Bind VAO------------------------------------------------------------------//
	glBindVertexArray(m_vertexArrayID);

	//vertices---------------------------------------------------------------------//
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
}

/********************************************************************************
Render
********************************************************************************/
void Mesh::Render(float alpha)
{
	CU::view.RenderMesh(*this, alpha);

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
void Mesh::Render(unsigned offset, unsigned count, float alpha)
{
	CU::view.RenderMesh(*this, alpha);

	//If rendering this mesh for first time this frame, bind it's VAO
	if (currentMesh != meshID)
	{
		glBindVertexArray(m_vertexArrayID);
	}

	currentMesh = meshID;

	//Bind VAO---------------------------------------------//
	glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
}

/********************************************************************************
Tilemap mesh
********************************************************************************/
void Mesh_Tilemap::SetTilemapTex(TILEMAP_ENUM id){ textureID = id; }
int Mesh_Tilemap::GetTilemapTex(){ return textureID; }

//Core------------------------------//
void Mesh_Tilemap::Render(float alpha)
{
	CU::view.RenderTilemap(*this, alpha);

	//If rendering this mesh for first time this frame, bind it's VAO
	if (currentMesh != meshID)
	{
		glBindVertexArray(m_vertexArrayID);
	}

	currentMesh = meshID;

	//Bind VAO---------------------------------------------//
	glDrawElements(mode, indexSize, GL_UNSIGNED_INT, 0);
}

void Mesh_Tilemap::Render(unsigned offset, unsigned count, float alpha)
{
	CU::view.RenderTilemap(*this, alpha);

	//If rendering this mesh for first time this frame, bind it's VAO
	if (currentMesh != meshID)
	{
		glBindVertexArray(m_vertexArrayID);
	}

	currentMesh = meshID;

	//Bind VAO---------------------------------------------//
	glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
}