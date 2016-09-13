#ifndef AABB_H
#define AABB_H

#include "Renderer.h"
#include "Line.h"


/*************************************************************
AABB quick collision

Author: Tan Yie Cher
Date: 22/8/2016
/*************************************************************/
class AABB : public Renderer
{
	bool collided;
	Vector3 vel;
	
public:


	/******************** constructor/destructor **********************/
	AABB();
	AABB(const AABB& copyMe);
	~AABB();

	/******************** Transformation: added on to parent's **********************/


	/******************** Init functions **********************/
	void Init(const char* name, Mesh* debugMesh, Vector3 pos, Vector3 scale);

	/******************** collision functions **********************/
	bool CollisionCheck(AABB& checkMe);

	/******************** CALLED BY PARENT ENTITY ONLY **********************/
	//transformation---------------------------------------------//
	void ByParent_Rotate(float angle, Vector3 axis);

	void CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform);

	/******************** Core functions **********************/
	void PreUpdate();
	void Update();
	void Draw();

	/******************** Get set functions **********************/

	/************************************* Line intersection ***************************************/
	bool CheckLineIntersection(const Vector3& lineOrigin, float lineAngle, Vector3& intersectedPos);
};

#endif