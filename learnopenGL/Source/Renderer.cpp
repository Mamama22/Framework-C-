#include "Renderer.h"
#include "CoreUtilities.h"

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
	CU::view.Scale(transform.scale.x, transform.scale.y, transform.scale.z);
	CU::view.RenderMesh(*mesh);
}