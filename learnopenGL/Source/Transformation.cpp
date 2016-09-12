#include "Transformation.h"
Transformation::Transformation(){ transforming = false; angle = 0.f; axis.Set(0, 1, 0); }
Transformation::Transformation(const Transformation& copy){ *this = copy; }
Transformation::~Transformation(){}
TransformNode Transformation::shareNode;
Mtx44 Transformation::sharedMtx[5];
Mtx44 Transformation::CustomTrans_Mtx;
bool Transformation::apply_ToChildren = true;
TransformNode Transformation::customTrans[20];
int Transformation::totalTrans = 0;
int Transformation::TRS_count = 0;
int Transformation::TRS_cal_count = 0;

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
Set transformation
********************************************************************************/
void Transformation::PreUpdate()
{
	vel.SetZero();
	transforming = false;
}

/********************************************************************************
Translate
********************************************************************************/
void Transformation::Translate(Vector3 vel)
{
	this->pos += vel;

	sharedMtx[0].SetToTranslation(vel.x, vel.y, vel.z);

	TRS = TRS * sharedMtx[0];
	this->vel += vel;
	transforming = true;
	TRS_count++;
}

/********************************************************************************
Scale
********************************************************************************/
void Transformation::Scale(Vector3 scale)
{
	this->scale += scale;
	transforming = true;
	TRS_count++;
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
	transforming = true;
	TRS_count++;
}

/********************************************************************************
transform by parent
********************************************************************************/
void Transformation::Translate_byParent(Vector3 vel)
{
	this->pos += vel;
	this->vel += vel;
	transforming = true;
	TRS_count++;
}

void Transformation::Rotate_byParent(float angle, Vector3 axis)
{
	this->axis = axis;
	this->angle += angle;
	if (this->angle < 0.f)this->angle += 360.f;
	else if (this->angle > 360.f)this->angle -= 360.f;
	transforming = true;
	TRS_count++;
}

/********************************************************************************
Offset TRS to parent TRS
Assume list is TRS order
Scale not counted
********************************************************************************/
void Transformation::AddedToParent(Transformation& trans)
{
	TRS = trans.TRS.GetInverse() * TRS;	//offset back by this TRS

	//both parent and child needs to update their final TRS
	transforming = true;
	trans.transforming = true;
	TRS_count++;
	TRS_count++;
}

void Transformation::RemovedFromParent(Transformation& trans)
{
	TRS = trans.TRS * TRS;	//offset out by this TRS

	//both parent and child needs to update their final TRS
	transforming = true;
	trans.transforming = true;
	TRS_count++;
	TRS_count++;
}

/********************************************************************************
Calculate TRS based on transformation list
Scale not counted in TRS but at Final TRS
********************************************************************************/
void Transformation::Calculate_transformList()
{
	sharedMtx[0].SetToTranslation(pos.x, pos.y, pos.z);
	sharedMtx[1].SetToRotation(angle, axis.x, axis.y, axis.z);
	sharedMtx[2].SetToScale(scale.x, scale.y, scale.z);

	TRS = TRS * sharedMtx[0] * sharedMtx[1];
	finalTRS = TRS;
	finalTRS = finalTRS * sharedMtx[2];
}

/********************************************************************************
Start custom transform
********************************************************************************/
void Transformation::Start_CustomTrans(bool applyToChildren)
{
	totalTrans = 0;
	apply_ToChildren = applyToChildren;
	transforming = true;
	TRS_count++;
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
void Transformation::Calculate_TRS()
{
	//if never transform or calculated alr, no need to update TRS and final TRS
	if (!transforming)
		return;

	//Final TRS + scale--------------------------------------------------//
	sharedMtx[2].SetToScale(scale.x, scale.y, scale.z);
	finalTRS = TRS * sharedMtx[2];

	//get updateed pos aand vel-------------------------//
	this->pos = GetPos();
	this->vel = GetVel();

	//preparation for next stage-------------------------//
	TRS_cal_count++;
	transforming = false;
}

/********************************************************************************
Calculate final TRS + applied with parent transformations
parentRotMat: the transformation intended for children
********************************************************************************/
Mtx44 returnRot;
int counter = 0;
Mtx44 Transformation::Calculate_TRS_withParent(const Mtx44& parentRotMat)
{
	//if never transform or calculated alr, no need to update TRS and final TRS, just return parent TRS * this TRS to children in case they transforms
	if (!transforming)
		return parentRotMat * TRS;

	//Final TRS + scale--------------------------------------------------//
	sharedMtx[2].SetToScale(scale.x, scale.y, scale.z);

	//apply parent TRS, then local TRS, then scaling
	returnRot = parentRotMat * TRS;
	finalTRS = returnRot * sharedMtx[2];

	//get updateed pos aand vel-------------------------//
	this->pos = GetPos();
	this->vel = GetVel();

	//preparation for next stage-------------------------//
	TRS_cal_count++;
	transforming = false;

	return returnRot;
}

/********************************************************************************
Get pos from TRS
********************************************************************************/
Vector3 Transformation::GetPos()
{
	Vector3 pos;
	pos = finalTRS * pos;
	return pos;
}

/********************************************************************************
Get vel raking into account angle
********************************************************************************/
Vector3 xVel, yVel;
float sinAngle, cosAngle;
Vector3 Transformation::GetVel()
{
	sinAngle = sin(Math::DegreeToRadian(angle));
	cosAngle = cos(Math::DegreeToRadian(angle));

	//X vel----------------------------------------------//
	xVel.x = vel.x * cosAngle;
	xVel.y = vel.x * sinAngle;

	//X vel----------------------------------------------//
	yVel.x = vel.y * -sinAngle;
	yVel.y = vel.y * cosAngle;
	return xVel + yVel;
}

bool Transformation::isTransforming(){ return transforming; }

/********************************************************************************
operator overloads
********************************************************************************/
Transformation& Transformation::operator= (const Transformation& copyMe)
{
	angle = copyMe.angle;
	axis = copyMe.axis;
	pos = copyMe.pos;
	scale = copyMe.scale;
	TRS = copyMe.TRS;
	finalTRS = copyMe.finalTRS;	//overall transformation matrix
	return *this;
}