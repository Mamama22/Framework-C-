#ifndef BOX_COLLISION_H
#define BOX_COLLISION_H

#include "Component.h"
#include "Mesh.h"

/*************************************************************
Box collison. Once collides, will apply collision to all

Type: abstract class
Usage: just add to the entity

Author: Tan Yie Cher
Date: 12/7/2016
/*************************************************************/
class BoxCollision : public Component
{
protected:

	/******************** var **********************/
	Vector3 vel;
	Vector3 prevPos;

public:

	/******************** constructor/destructor **********************/
	BoxCollision();
	BoxCollision(const BoxCollision& copyMe);
	~BoxCollision();

	/******************** comp functions **********************/
	void Init(const char* name, Vector3 pos, Vector3 scale);

	/******************** check collision **********************/
	void CheckCollision(BoxCollision& check);

	/******************** abstract functions **********************/
	void UpdatingComp();
	void Exit();

	/******************** special functions **********************/
	void DrawBound();	//Call at draw
};


#endif