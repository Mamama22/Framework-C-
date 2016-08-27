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
-Pos and Vel will be updated after calculating final TRS at stage 2

Improvement:
-customisable transformation steps

Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Transformation
{
	//Static---------------------------------//
	static TransformNode shareNode;
	static Mtx44 sharedMtx[5];	//for TRS calculations

	//Custom transform---------------------------------//
	static Mtx44 CustomTrans_Mtx;
	static TransformNode customTrans[20];
	static int totalTrans;
	static bool apply_ToChildren;

	/******************** Utilities **********************/
	void Calculate_transformList();

	//Flags---------------------------------------------------------//
	bool transforming;

public:

	static int TRS_count;
	static int TRS_cal_count;

	//Values---------------------------------------------------------//
	float angle;
	Vector3 axis;
	Vector3 pos;
	Vector3 vel;	//travelling vel
	Vector3 scale;
	Mtx44 TRS, finalTRS;	//overall transformation matrix


	/******************** constructor/destructor **********************/
	Transformation();
	Transformation(const Transformation& copy);
	~Transformation();

	/******************** Core functions **********************/
	void Set(Vector3 pos, Vector3 scale);
	void PreUpdate();

	/******************** Transformation **********************/
	void Translate(Vector3 vel);	//MAKE SURE childrens also called
	void Scale(Vector3 scale);
	void Rotate(float angle, Vector3 axis);

	/******************** Transformation by parent **********************/
	void Translate_byParent(Vector3 vel);	//MAKE SURE childrens also called
	void Rotate_byParent(float angle, Vector3 axis);

	/******************** Custom Transformation **********************/
	void Start_CustomTrans(bool applyToChildren);
	void Custom_Translate(Vector3 vel);
	void Custom_Rotate(float angle, Vector3 axis);
	void Custom_Scale(Vector3 scale);
	void End_CustomTrans();

	/******************** Calculate TRS **********************/
	void Calculate_TRS();
	Mtx44 Calculate_TRS_withParent(const Mtx44& parentRotMat);
	
	/******************** Parent/child **********************/
	void AddedToParent(Transformation& trans);	//to offset child to be based on parent TRS

	/******************** Get functions **********************/
	Vector3 GetPos();
	Vector3 GetVel();

	/******************** Operator overload **********************/
	Transformation& operator= (const Transformation& copyMe);
};

#endif