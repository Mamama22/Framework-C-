#include "Transformation.h"
Transformation::Transformation(){ angle = 0.f; axis.Set(0, 1, 0); }
Transformation::~Transformation(){}
TransformNode Transformation::shareNode;
Mtx44 Transformation::sharedMtx;

/********************************************************************************
Set transformation
********************************************************************************/
void Transformation::Set(Vector3 pos, Vector3 scale)
{
	this->scale = scale;
	this->pos = pos;

	//Add default transform: T, R and S-----------------------------//
	Add_Translate(pos);
	Add_Rotate(angle, axis);
	Add_Scale(scale);

	//Set the TRS to initial value---------------------------------//
	Calculate_TRS();
}

/********************************************************************************
Translate
********************************************************************************/
void Transformation::Translate(Vector3 vel)
{
	this->pos += vel;
	Modify_Translate(0, this->pos);	//update transformList

	sharedMtx.SetToTranslation(vel.x, vel.y, vel.z);
	TRS = TRS * sharedMtx;
}

/********************************************************************************
Scale
********************************************************************************/
void Transformation::Scale(Vector3 scale)
{
	this->scale += scale;
	//Modify_Scale(2, this->scale);
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
	Modify_Rotate(1, this->angle, this->axis);	//update transformList

	sharedMtx.SetToRotation(angle, axis.x, axis.y, axis.z);
	TRS = TRS * sharedMtx;
}

/********************************************************************************
Add transformation step
********************************************************************************/
void Transformation::Add_Translate(Vector3 translate)
{
	shareNode.SetTranslate(translate.x, translate.y, translate.z);
	transformList.push_back(shareNode);
}

void Transformation::Add_Rotate(float angle, Vector3 axis)
{
	shareNode.SetRotate(angle, axis.x, axis.y, axis.z);
	transformList.push_back(shareNode);
}

void Transformation::Add_Scale(Vector3 scale)
{
	shareNode.SetScale(scale.x, scale.y, scale.z);
	transformList.push_back(shareNode);
}

/********************************************************************************
Modify transformation: make sure you keep track of the indexes
********************************************************************************/
void Transformation::Modify_Translate(int index, Vector3 translate)
{
	transformList[index].SetTranslate(translate.x, translate.y, translate.z);
}

void Transformation::Modify_Rotate(int index, float angle, Vector3 axis)
{
	transformList[index].SetRotate(angle, axis.x, axis.y, axis.z);
}

void Transformation::Modify_Scale(int index, Vector3 scale)
{
	transformList[index].SetScale(scale.x, scale.y, scale.z);
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
********************************************************************************/
void Transformation::Calculate_transformList()
{
	for (int i = 0; i < transformList.size(); ++i)
	{
		if (transformList[i].type == 0)	//translate
			sharedMtx.SetToTranslation(transformList[i].v1, transformList[i].v2, transformList[i].v3);
		else if (transformList[i].type == 1)	//rotate
			sharedMtx.SetToRotation(transformList[i].v1, transformList[i].v2, transformList[i].v3, transformList[i].v4);
		else	//scale
			sharedMtx.SetToScale(transformList[i].v1, transformList[i].v2, transformList[i].v3);

		TRS *= sharedMtx;
	}
}

/********************************************************************************
Calculate TRS based on transformation list

Returns Mtx44: the matrix all children of this entity will transform with (Excludes scale if is last)
********************************************************************************/
void Transformation::Calculate_TRS()
{
	TRS.SetToIdentity();
	Calculate_transformList();	//Calculate TRS and pass on mat
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