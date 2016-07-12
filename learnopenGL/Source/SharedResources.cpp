#include "SharedResources.h"
#include "MeshBuilder.h"

/********************************************************************************
Init
********************************************************************************/
void SharedResources::Init()
{
	boundingBox_Mesh = MeshBuilder::GenerateDebugQuad(Color(42, 212, 175));
}

/********************************************************************************
Exit
********************************************************************************/
void SharedResources::Exit()
{
	delete boundingBox_Mesh;
}