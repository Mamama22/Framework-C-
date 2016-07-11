#include "Transformation.h"
Transformation::Transformation(){ angle = 0.f; axis.Set(0, 1, 0); }
Transformation::~Transformation(){}
TransformNode Transformation::shareNode;
Mtx44 Transformation::sharedMtx[5];
Mtx44 Transformation::CustomTrans_Mtx;
bool Transformation::apply_ToChildren = true;
TransformNode Transformation::customTrans[20];
int Transformation::totalTrans = 0;

/********************************************************************************
Set transformation
********************************************************************************/
void Transformation::Set(Vector3 pos, Vector3 scale)
{
	this->scale = scale;
	this->pos = pos;

	//Add default transform: T, R and S-----------------------------//

	//Set the TRS to initial value---------------------------------//
	TRS.SetToIdentity();
	Calculate_transformList();	//Calculate TRS and pass on mat
}

/********************************************************************************
Translate
********************************************************************************/
void Transformation::Translate(Vector3 vel)
{
	this->pos += vel;

	sharedMtx[0].SetToTranslation(vel.x, vel.y, vel.z);
	TRS = TRS * sharedMtx[0];
}

/********************************************************************************
Scale
********************************************************************************/
void Transformation::Scale(Vector3 scale)
{
	this->scale += scale;
}

/********************************************************************************
Rotates entity and children + components
********************************************************************************/
void Transformation::Rotate(float angle, Vector3 axis)
{
	this->axis = axis;
	this->angle += angle;
	if (this->angle < 0.f)this->angle += 360.f;
	else if (this->angle > 360.f)this->angle -= 360.f;

	sharedMtx[0].SetToRotation(angle, axis.x, axis.y, axis.z);
	TRS = TRS * sharedMtx[0];
}

/********************************************************************************
Offset TRS to parent TRS
Assume list is TRS order
Scale not counted
********************************************************************************/
void Transformation::AddedToParent(Transformation& trans)
{
	TRS = trans.TRS.GetInverse() * TRS;
}

/********************************************************************************
Calculate TRS based on transformation list
Scale not counted in TRS but at Final TRS
********************************************************************************/
void Transformation::Calculate_transformList()
{
	sharedMtx[0].SetToTranslation(pos.x, pos.y, pos.z);
	sharedMtx[1].SetToRotation(angle, axis.x, axis.y, axis.z);

	TRS = TRS * sharedMtx[0] * sharedMtx[1];
}

/********************************************************************************
Start custom transform
********************************************************************************/
void Transformation::Start_CustomTrans(bool applyToChildren)
{
	totalTrans = 0;
	apply_ToChildren = applyToChildren;
}

void Transformation::Custom_Translate(Vector3 vel)
{
	customTrans[totalTrans++].SetTranslate(vel.x, vel.y, vel.z);
}

void Transformation::Custom_Rotate(float angle, Vector3 axis)
{
	customTrans[totalTrans++].SetRotate(angle, axis.x, axis.y, axis.z);
}
	
void Transformation::Custom_Scale(Vector3 scale)
{
	customTrans[totalTrans++].SetScale(scale.x, scale.y, scale.z);
}

void Transformation::End_CustomTrans()
{
	Mtx44 trans;
	Mtx44 jaja;
	jaja.SetToIdentity();

	for (int i = 0; i < totalTrans; ++i)
	{
		if (customTrans[i].type == 0)	//translate
		{
			trans.SetToTranslation(customTrans[i].v1, customTrans[i].v2, customTrans[i].v3);
			jaja *= trans;
		}
		else if (customTrans[i].type == 1)	//rotate
		{
			trans.SetToRotation(customTrans[i].v1, customTrans[i].v2, customTrans[i].v3, customTrans[i].v4);
			jaja *= trans;
		}
		else if (customTrans[i].type == 2)	//scale
		{
			trans.SetToScale(customTrans[i].v1, customTrans[i].v2, customTrans[i].v3);
			jaja *= trans;
		}
	}

	TRS = TRS * jaja;
}

/********************************************************************************
Calculate final TRS
********************************************************************************/
Mtx44 Transformation::Calculate_TRS()
{
	//Final TRS + scale--------------------------------------------------//
	sharedMtx[2].SetToScale(scale.x, scale.y, scale.z);
	finalTRS = TRS * sharedMtx[2];
	return TRS;
}

/********************************************************************************
Calculate final TRS + applied with parent transformations
parentRotMat: the transformation intended for children
********************************************************************************/
Mtx44 returnRot;
Mtx44 Transformation::Calculate_TRS_withParent(const Mtx44& parentRotMat)
{
	//Final TRS + scale--------------------------------------------------//
	sharedMtx[2].SetToScale(scale.x, scale.y, scale.z);

	//apply parent TRS, then local TRS, then scaling
	returnRot = parentRotMat * TRS;
	finalTRS = returnRot * sharedMtx[2];
	return returnRot;
}

/********************************************************************************
Get pos from TRS
********************************************************************************/
Vector3 Transformation::GetPos()
{
	Vector3 pos;
	pos = TRS * pos;
	return pos;
}