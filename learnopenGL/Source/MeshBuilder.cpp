#include "MeshBuilder.h"

void MeshBuilder::BindBuffers(Mesh& mesh, std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, unsigned drawMode)
{
	//Bind VAO------------------------------------------------------------------//
	glBindVertexArray(mesh.m_vertexArrayID);

	//vertices---------------------------------------------------------------------//
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	//indexe---------------------------------------------------------------------//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
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


	mesh.indexSize = index_buffer_data.size();
	mesh.mode = drawMode;
	
	//Unbind-----------------------------------------------------------------//
	glBindVertexArray(0);	//VAO stores unbind calls too, UNBIND BEFORE UNBINDING BUFFERS
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

/******************************************************************************
Axes
******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	Mesh *mesh = new Mesh();

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(255, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(255, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 255, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 255, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 255);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 255);
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	BindBuffers(*mesh, vertex_buffer_data, index_buffer_data, GL_LINES);

	return mesh;
}

/******************************************************************************
Generate a line
******************************************************************************/
Mesh* MeshBuilder::GenerateLine(Color color, bool start0)
{
	Vertex v;
	Mesh *mesh = new Mesh();

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	vertex_buffer_data;
	
	if (start0)
		v.pos.Set(0, 0, 0);
	else
		v.pos.Set(-0.5f, 0, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	if (start0)
		v.pos.Set(1, 0, 0);
	else
		v.pos.Set(0.5f, 0, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);

	BindBuffers(*mesh, vertex_buffer_data, index_buffer_data, GL_LINES);

	return mesh;
}

/******************************************************************************
Generate a debug quad
******************************************************************************/
Mesh* MeshBuilder::GenerateDebugQuad(Color color)
{
	Mesh *mesh = new Mesh();
	Vertex v;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(-0.5f, -0.5f, 0.f);//0
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.f);	//1
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.f);	//2
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.f);//3
	v.color = color;
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);

	BindBuffers(*mesh, vertex_buffer_data, index_buffer_data, GL_LINES);

	return mesh;
}

/******************************************************************************
Generate a triangle
******************************************************************************/
Mesh* MeshBuilder::GenerateRTriangle(Color color)
{
	Mesh *mesh = new Mesh();
	Vertex v;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(-0.5f , -0.5f , 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f , -0.5f , 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f , 0.5f , 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f , 0.5f , 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(0);

	BindBuffers(*mesh, vertex_buffer_data, index_buffer_data, GL_TRIANGLES);

	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

/******************************************************************************
Generate a sphere
******************************************************************************/
Mesh* MeshBuilder::GenerateSphere(Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Mesh *mesh = new Mesh();

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;

	for (unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			Vertex v;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	BindBuffers(*mesh, vertex_buffer_data, index_buffer_data, GL_TRIANGLE_STRIP);

	return mesh;
}

/******************************************************************************
Generate a quad
******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(Color color, float length, float texCoord, bool start0)
{
	Mesh *mesh = new Mesh();
	Vertex v;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	if (start0)
	{
		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(0, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(length, 0, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(length, length, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(1, 1);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, length, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(0, 1);
		vertex_buffer_data.push_back(v);
	}
	else
	{
		v.pos.Set(-0.5f * length, -0.5f * length, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(0, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * length, -0.5f * length, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * length, 0.5f * length, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(1, 1);
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * length, 0.5f * length, 0);
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.texcoord.Set(0, 1);
		vertex_buffer_data.push_back(v);
	}

	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);

	BindBuffers(*mesh, vertex_buffer_data, index_buffer_data, GL_TRIANGLES);

	return mesh;
}

/******************************************************************************
Generate a sprite animation
******************************************************************************/
Mesh* MeshBuilder::GenerateSpriteAnimation(const std::string&, unsigned numRow, unsigned numCol)
{
	SpriteAnimation *mesh = new SpriteAnimation(numRow, numCol);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	Vertex v;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texcoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texcoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texcoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texcoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}


	BindBuffers(*mesh, vertex_buffer_data, index_buffer_data, GL_TRIANGLES);

	return mesh;
}