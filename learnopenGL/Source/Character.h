#ifndef CHARACTER_H
#define CHARACTER_H
#include "Entity.h"

/*************************************************************
Basic character with path-finding

How to use:
-Overload and define your own chracter type
-Scale is the bounds of the character, will be used to determine if is in a path-finding tile

NOTE:
Comps added automatically:
1) AI_Comp

Author: Tan Yie Cher
Date: 18/9/2016
/*************************************************************/
class Character : public Entity
{
public:

	enum STATE
	{
		IDLE,
		MOVE,
	};

protected:

	AI_Comp* AI_comp;	//reference to path-finding grid
	STATE state;
	Vector3 targetPos;

public:

	/******************** Core functions **********************/
	void Init(Vector3 pos, Vector3 scale, int AI_Map_id, bool displayPath);

	void Update_Stage1();
	void Update_Stage3();

	/******************** Testing functions **********************/
	void FindPath(int startX, int startY, int endX, int endY);
};

#endif