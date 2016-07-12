#include "BoxCollision.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
BoxCollision::BoxCollision()
{
}

BoxCollision::BoxCollision(BoxCollision& copyMe) : Component(copyMe)
{
}

BoxCollision::~BoxCollision()
{
}

/********************************************************************************
Init
********************************************************************************/
void BoxCollision::Init(const char* name, Vector3 pos, Vector3 scale)
{
	Component::Init(name);
	transform.Set(pos, scale);
}

/********************************************************************************
collision check: SAT
********************************************************************************/
void CheckCollision(BoxCollision& check)
{

}

/********************************************************************************
comp update: post-TRS
********************************************************************************/
void BoxCollision::UpdatingComp()
{
	Vector3 pos = transform.GetPos();

	//check if go over orange line---------------------------------------//
	if (pos.x + transform.scale.x * 0.5f > TEST_COLLISION_X)
	{
		float x = TEST_COLLISION_X - (pos.x + transform.scale.x * 0.5f);

		//Translate TOP parent entity back------------------------------//
		CU::entityMan.GetTopParent_Entity(parentHandle)->Translate(Vector3(x - 0.05f, 0, 0));
		//CU::entityMan.GetEntity(parentHandle)->Translate(Vector3(x - 0.05f, 0, 0));
	}
}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void BoxCollision::DrawBound()
{
	if (active)
	{
		CU::view.SetIdentity();
		CU::view.LoadMatrix(transform.finalTRS);
		CU::view.RenderMesh(*CU::sharedResources.boundingBox_Mesh);
	}
}

/********************************************************************************
Exit
********************************************************************************/
void BoxCollision::Exit()
{

}