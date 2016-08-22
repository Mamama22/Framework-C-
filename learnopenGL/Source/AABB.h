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
	

public:

	/******************** constructor/destructor **********************/
	AABB();
	AABB(const AABB& copyMe);
	~AABB();

	/******************** Transformation: added on to parent's **********************/


	/******************** Init functions **********************/
	void Init(const char* name, Vector3 pos, Vector3 scale);

	/******************** collision functions **********************/
	void CollisionCheck(AABB& checkMe);

	/******************** CALLED BY PARENT ENTITY ONLY **********************/
	//transformation---------------------------------------------//
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