#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Vector3.h"

/*************************************************************
Transformation component
Core component type

Every entity/comp to have it

Usage: 
-To get velocity, pos - prevPos

Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Transformation
{
public:
	float angle;
	Vector3 pos;
	Vector3 scale;
	Vector3 prevPos;	//previous pos
	Vector3 vel;	//velocity

	Transformation(){ angle = 0.f; }
	~Transformation(){}

	void Set(Vector3 pos, Vector3 scale){ this->pos = pos; this->scale = scale; this->angle = 0.f; }
};

#endif