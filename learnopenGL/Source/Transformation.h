#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Vector3.h"
#include "Mtx44.h"

/*************************************************************
Add a transformation node to the list of transformations to carry out

Author: Tan Yie Cher
Date: 7/7/2016
/*************************************************************/
class TransformNode
{
public:
	unsigned type;
	float v1;
	float v2;
	float v3;
	float v4;

	TransformNode(){ v1 = v2 = v3 = v4 = 0.f; type = -1; }
	TransformNode(const TransformNode& c){ type = c.type; v1 = c.v1; v2 = c.v2; v3 = c.v3; v4 = c.v4; }
	~TransformNode(){}

	void SetTranslate(float v1, float v2, float v3)
	{
		type = 0;
		this->v1 = v1;this->v2 = v2;this->v3 = v3;
	}

	void SetRotate(float angle, float ax, float ay, float az)
	{
		type = 1;
		this->v1 = angle; this->v2 = ax; this->v3 = ay; this->v4 = az;
	}

	void SetScale(float v1, float v2, float v3)
	{
		type = 2;
		this->v1 = v1; this->v2 = v2; this->v3 = v3;
	}
};

/*************************************************************
Transformation component
Core component type

Every entity/comp to have it

Usage: 
-Entity must call children's Translate if translate being called!!!
-Add/remove transformations to apply to entity (Note: If scale is final transformation, it will not be passed on)

Improvement:
-customisable transformation steps

Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Transformation
{
	//Static---------------------------------//
	static TransformNode shareNode;
	static Mtx44 sharedMtx;	//for TRS calculations

	/******************** Utilities **********************/
	void Calculate_transformList();

public:

	//Default--------------------------------//
	float angle;
	Vector3 axis;
	Vector3 pos;
	Vector3 scale;
	Mtx44 TRS, finalTRS;	//overall transformation matrix

	vector<TransformNode> transformList;

	/******************** constructor/destructor **********************/
	Transformation();
	~Transformation();

	/******************** Init **********************/
	void Set(Vector3 pos, Vector3 scale);

	/******************** Transformation **********************/
	void Translate(Vector3 vel);	//MAKE SURE childrens also called
	void Scale(Vector3 scale);
	void Rotate(float angle, Vector3 axis);

	/******************** Add/modify transformation **********************/
	void Add_Translate(Vector3 translate);
	void Add_Rotate(float angle, Vector3 axis);
	void Add_Scale(Vector3 scale);

	void Modify_Translate(int index, Vector3 translate);
	void Modify_Rotate(int index, float angle, Vector3 axis);
	void Modify_Scale(int index, Vector3 scale);

	/******************** Calculate TRS **********************/
	void AddedToParent(Transformation& trans);	//to offset child to be based on parent TRS
	void Calculate_TRS();
	
	/******************** Get functions **********************/
	Vector3 GetPos();
};

#endif