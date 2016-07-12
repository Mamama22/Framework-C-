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

	/******************** core **********************/
	void Init();
	void Exit();
};


#endif