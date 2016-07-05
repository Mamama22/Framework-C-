#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Vector3.h"

/*************************************************************
Transformation component
Core component type

Every entity/comp to have it

Usage: 
-default 

Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Transformation
{
public:
	Vector3 pos;
	Vector3 scale;

	void Set(Vector3 pos, Vector3 scale){ this->pos = pos; this->scale = scale; }
};

#endif