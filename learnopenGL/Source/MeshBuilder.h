#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"

/*************************************************************
Mesh builder class

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
class MeshBuilder
{
private:
	static void BindBuffers(Mesh& mesh, std::vector<Vertex>& vertex_buffer_data, std::vector<GLuint>& index_buffer_data, unsigned drawMode);

public:
	static Mesh* GenerateAxes(float lengthX = 1.f, float lengthY = 1.f, float lengthZ = 1.f);
	static Mesh* GenerateQuad(float length = 1.f, float texCoord = 1.f, bool start0 = false);
	static Mesh* GenerateSphere(unsigned numStack, unsigned numSlice, float radius = 1.f);
};

#endif