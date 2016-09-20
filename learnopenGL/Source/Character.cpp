#include "Character.h"
#include "AI_Map.h"
#include "CoreUtilities.h"

/********************************************************************************
Init Character
param AI_Map_id: the ID of the AI_Map character will be in
********************************************************************************/
void Character::Init(Vector3 pos, Vector3 scale, int AI_Map_id, bool displayPath)
{
	Entity::Init(pos, scale);

	state = IDLE;

	//AI Comp-----------------------------------------------------//
	AI_comp = new AI_Comp;
	AI_comp->Init("ASD", AI_Map_id, displayPath);
	AddComponent(AI_comp);
}

/********************************************************************************
Update chracter movement and flags: stage 1
********************************************************************************/
void Character::Update_Stage1()
{
	float speed = 250.f;

	switch (state)
	{
	case IDLE:

		break;
	case MOVE:

		//get latest update target pos (in case moved)----------------------------------------//
		targetPos = AI_comp->GetTargetPointPos();

		//translate to target, if reach, get new target----------------------//
		Vector3 dir = (targetPos - transform.pos).Normalized();

		Vector3 vel = dir * speed * CU::dt;
		
		//if gonna overshoot target pos, translate to target pos--------------//
		if (vel.LengthSquared() > (targetPos - transform.pos).LengthSquared())
		{
			vel = targetPos - transform.pos;
		}
		
		Translate(vel);

		break;
	}
}

/********************************************************************************
Update chracter after changes
********************************************************************************/
void Character::Update_Stage3()
{
	switch (state)
	{
	case IDLE:

		break;
	case MOVE:

		float dist = AI_comp->Get_tileScale() * 0.25f;

		//check if in bounds of tile----------------------------------------//
		if ((targetPos - transform.pos).LengthSquared() < dist * dist)
		{
			AI_comp->GetNextPoint();
	
			if (AI_comp->Reached_Dest())
				state = IDLE;
		}

		break;
	}
}

/********************************************************************************
Test find a path and follow it
********************************************************************************/
void Character::Test_FindPath()
{
	if (AI_comp->findPath(2, 2, 24, 22))
	{
		state = MOVE;
	}
}