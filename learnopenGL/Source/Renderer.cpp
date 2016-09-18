#include "Renderer.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
Renderer::Renderer() : Component()
{
	mesh = NULL; 
	CU::rendererMan.RegisterRenderer(this);
	alpha = 1.f;
}

Renderer::Renderer(const Renderer& copyMe) : Component(copyMe)
{ 
	mesh = copyMe.mesh; 
	transform = copyMe.transform;
	alpha = copyMe.alpha;
	CU::rendererMan.RegisterRenderer(this);
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
	Component::Init(name, pos, scale);
	this->mesh = mesh;
}

/********************************************************************************
Get/set
********************************************************************************/
void Renderer::SetAlpha(float p){ alpha = p; }
float Renderer::GetAlpha(){ return alpha; }

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
	CU::view.SetIdentity();
	CU::view.LoadMatrix(transform.finalTRS);
	mesh->Render(alpha);
}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void Render_InWorld::Draw()
{
	CU::view.SetIdentity();
	CU::view.LoadMatrix(transform.finalTRS);
	mesh->Render(alpha);
}

/********************************************************************************
Repetitive mesh
********************************************************************************/
void Render_Repetitive::Init(const char* name, Mesh* mesh, Vector3 pos, Vector3 scale, Vector3 offset, int count)
{
	Renderer::Init(name, mesh, pos, scale);
	this->offset = offset.divideWithVector(scale);
	this->count = count;
}

/********************************************************************************
Repetitive mesh
********************************************************************************/
void Render_Repetitive::Draw()
{
	CU::view.SetIdentity();
	CU::view.LoadMatrix(transform.finalTRS);

	for (int i = 0; i < count; ++i)
	{
		mesh->Render(alpha);
		CU::view.Translate(offset.x, offset.y, offset.z);
	}
}