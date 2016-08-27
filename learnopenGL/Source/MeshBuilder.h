#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "SpriteAnimation.h"

/*************************************************************
Mesh builder class

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
class MeshBuilder
{
	static void GenQuadVertices(Color color, float length, std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, int totalX, int totalY, bool start0);

public:

	static Mesh* GenerateLine(Color color, bool start0 = false);
	static Mesh* GenerateAxes(float lengthX = 1.f, float lengthY = 1.f, float lengthZ = 1.f);
	static Mesh* GenerateQuad(Color color, float length = 1.f, float texCoord = 1.f, bool start0 = false);
	static Mesh* GenerateDebugQuad(Color color);
	static Mesh* GenerateRTriangle(Color color);
	static Mesh* GenerateSphere(Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateSpriteAnimation(const std::string& meshName, unsigned numRow, unsigned numCol);

	//custom quad---------------------------------------------------------------------//
	static Mesh* GenerateCustomQuad(std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, int totalX, int totalY, bool start0 = false);
};

#endif