#include "Renderer.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
Renderer::Renderer()
{
	mesh = NULL; 
}

Renderer::Renderer(const Renderer& copyMe) : Component(copyMe)
{ 
	mesh = copyMe.mesh; 
	transform = copyMe.transform;
}

Renderer::~Renderer()
{ 
	mesh = NULL; 
}

/********************************************************************************
Init
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
void Render_OnScreen::Draw()
{
}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void Render_InWorld::Draw()
{
	if (active)
	{
		CU::view.SetIdentity();
		CU::view.LoadMatrix(transform.finalTRS);
		CU::view.RenderMesh(*mesh);
	}
}