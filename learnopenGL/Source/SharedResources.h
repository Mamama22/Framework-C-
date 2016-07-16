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
	Mesh* sphere;
	Mesh* sphere_1;
	Mesh* line_1;
	Mesh* line_2;
	Mtx44 mtx[10];

	/******************** core **********************/
	void Init();
	void Exit();
	
	/******************** shared functions **********************/
	void DrawLine(Mesh* line, Vector3 startPos, float angle, float length, float thickness);
	void DrawMesh(Mesh* mesh, Vector3 pos, float scale);
};


#endif