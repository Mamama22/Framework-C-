#ifndef SHARED_RESOURCES_H
#define SHARED_RESOURCES_H

#include "Mesh.h"

/*************************************************************
Some commonly used resoures, like certain meshes.

Author: Tan Yie Cher
Date: 12/7/2016
/*************************************************************/
class SharedResources
{
public:

	/******************** shared resources **********************/
	Mesh* boundingBox_Mesh;
	Mesh* quad;
	Mesh* quad_1;
	Mesh* quad_2;
	Mesh* sphere;
	Mesh* sphere_1;
	Mesh* line_1;
	Mesh* line_2;
	Mesh* line_start0;
	Mesh* axes;
	Mtx44 mtx[10];

	/******************** core **********************/
	void Init();
	void Exit();
	
	/******************** shared functions **********************/
	void DrawLine_Offset(Mesh* line, Vector3 startPos, float angle, float length, float thickness, Vector3 offset);
	void DrawLine(Mesh* line, Vector3 startPos, float angle, float length, float thickness);
	void DrawMesh(Mesh* mesh, Vector3 pos, float scale);
	void CalculateOffset(Vector3& offset, Vector3& dir, float dist);

	//Math--------------------------------------------//
	Vector3 vectorProjection(Vector3& projected, Vector3& dir);
};


#endif