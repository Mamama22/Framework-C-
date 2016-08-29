#include "SP_Comp.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
SP_Comp::SP_Comp() : Renderer()
{
}

SP_Comp::SP_Comp(const SP_Comp& copyMe) : Renderer(copyMe)
{
}

SP_Comp::~SP_Comp()
{
}


/********************************************************************************
Init
********************************************************************************/
void SP_Comp::Init(const char* name, Vector3 pos, Vector3 scale)
{
	Renderer::Init(name, CU::shared.debug_quad, pos, scale);
}

/********************************************************************************
Update spartial partition
********************************************************************************/
void SP_Comp::UpdateSP(int totalGrids_X, int totalGrids_Y)
{
	//get local pos, entity.pos - gridmap.pos

	//use localized pos to find the current grid/s it intersected

	
}