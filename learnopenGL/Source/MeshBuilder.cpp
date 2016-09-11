#include "MeshBuilder.h"

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

	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_LINES);

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

	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_LINES);

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

	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_LINES);

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

	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_TRIANGLES);

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

	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_TRIANGLE_STRIP);

	return mesh;
}

/******************************************************************************
Generate quad vertices
******************************************************************************/
void MeshBuilder::GenQuadVertices(Color color, float length, std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, int totalX, int totalY, bool start0)
{
	Vertex v;

	const float xUnit = 1.f / (float)totalX;
	const float yUnit = 1.f / (float)totalY;
	float x = 0.f;
	float y = 0.f;

	if (start0)
	{
		int count = 0;
		for (float X = 0; X < totalX; ++X)
		{
			for (float Y = 0; Y < totalY; ++Y)
			{
				count++;
				v.pos.Set(x, y, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x, y);//
				vertex_buffer_data.push_back(v);
				//======================================================================//
				v.pos.Set(x + xUnit, y, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x + xUnit, y);//
				vertex_buffer_data.push_back(v);
				//======================================================================//
				v.pos.Set(x + xUnit, y + yUnit, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x + xUnit, y + yUnit);//
				vertex_buffer_data.push_back(v);
				//======================================================================//
				v.pos.Set(x, y + yUnit, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x, y + yUnit);//
				vertex_buffer_data.push_back(v);

				y += yUnit;

				//index-------------------------------------//
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 3);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 0);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 2);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 1);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 2);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 0);
			}
			y = 0.f;
			x += xUnit;
		}
	}
	else
	{
		float start = length * 0.5f;

		for (float X = 0; X < totalX; ++X)
		{
			for (float Y = 0; Y < totalY; ++Y)
			{
				v.pos.Set(x - start, y - start, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x, y);//
				vertex_buffer_data.push_back(v);
				//======================================================================//
				v.pos.Set(x + xUnit - start, y - start, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x + xUnit, y);//
				vertex_buffer_data.push_back(v);
				//======================================================================//
				v.pos.Set(x + xUnit - start, y + yUnit - start, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x + xUnit, y + yUnit);//
				vertex_buffer_data.push_back(v);
				//======================================================================//
				v.pos.Set(x - start, y + yUnit - start, 0);//
				v.color = color;
				v.normal.Set(0, 0, 1);
				v.texcoord.Set(x, y + yUnit);//
				vertex_buffer_data.push_back(v);

				y += yUnit;

				//index-------------------------------------//
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 3);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 0);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 2);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 1);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 2);
				index_buffer_data.push_back((Y * 4) + (X * (totalY * 4)) + 0);
			}
			y = 0.f;
			x += xUnit;
		}
	}
}

/******************************************************************************
Generate a quad
******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(Color color, float length, float texCoord, bool start0)
{
	Mesh *mesh = new Mesh();
	
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	GenQuadVertices(color, length, vertex_buffer_data, index_buffer_data, 1, 1, start0);
	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_TRIANGLES);

	return mesh;
}

/******************************************************************************
Generate a custom quad for tilemapping etc. 

1) Create a customisable mesh, pass in own vertex vector, which you can modify later on
2) call setup function after every modifcation
******************************************************************************/
Mesh_Tilemap* MeshBuilder::GenerateTilemapQuad(std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, int totalX, int totalY, bool start0)
{
	Mesh_Tilemap *mesh = new Mesh_Tilemap();

	GenQuadVertices(Color(1, 0, 0), 1.f, vertex_buffer_data, index_buffer_data, totalX, totalY, start0);

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


	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_TRIANGLES);

	return mesh;
}