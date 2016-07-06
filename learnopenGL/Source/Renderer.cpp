#include "Renderer.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
Renderer::Renderer()
{
	mesh = NULL; 
}

Renderer::Renderer(Renderer& copyMe) : Component(copyMe)
{ 
	mesh = copyMe.mesh; 
}

Renderer::~Renderer()
{ 
	mesh = NULL; 
}

/********************************************************************************
Add component
********************************************************************************/
void Renderer::Init(const char* name, Mesh* mesh, Vector3 pos, Vector3 scale)
{
	Component::Init(name);
	this->mesh = mesh;
	transform.Set(pos, scale);
}

/********************************************************************************
Exit
********************************************************************************/
void Renderer::Exit()
{

}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void Render_OnScreen::UpdatingComp()
{
	CU::view.SetIdentity();
	CU::view.Translate(transform.pos.x, transform.pos.y, transform.pos.z);
	CU::view.Rotate(transform.angle, 0, 0, 1);
	CU::view.Scale(transform.scale.x, transform.scale.y, transform.scale.z);
	CU::view.RenderMesh(*mesh);
}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void Render_InWorld::UpdatingComp()
{
	CU::view.SetIdentity();
	CU::view.Translate(transform.pos.x, transform.pos.y, transform.pos.z);
	CU::view.Rotate(transform.angle, 0, 0, 1);
	CU::view.Scale(transform.scale.x, transform.scale.y, transform.scale.z);
	CU::view.RenderMesh(*mesh);
}