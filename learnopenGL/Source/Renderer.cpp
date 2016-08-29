#include "Renderer.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
Renderer::Renderer() : Component()
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
	CU::rendererMan.RegisterRenderer(this);
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
		CU::view.SetIdentity();
		CU::view.LoadMatrix(transform.finalTRS);
		CU::view.RenderMesh(*mesh);
}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void Render_InWorld::Draw()
{
	CU::view.SetIdentity();
	CU::view.LoadMatrix(transform.finalTRS);
	CU::view.RenderMesh(*mesh);
}


Render_GridMap::Render_GridMap() : Renderer(){}
Render_GridMap::Render_GridMap(const Render_GridMap& copyMe) : Renderer(copyMe){}
Render_GridMap::~Render_GridMap()
{
	for (int i = 0; i < this->totalX_tiles; ++i)
		delete[] gridmap[i];
	delete[] gridmap;

	delete mesh;
}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void Render_GridMap::Init(const char* name, TEXTURE_ENUM tileMap_tex, Vector3 pos, float tileScale, int totalX_tiles, int totalY_tiles)
{
	Renderer::Init(name, NULL, pos, Vector3(1,1,1));

	this->tileScale = tileScale;
	this->totalX_tiles = totalX_tiles;
	this->totalY_tiles = totalY_tiles;

	//Create gridmap: X over Y-------------------------------------------------------------//
	gridmap = new int*[this->totalX_tiles];
	for (int i = 0; i < this->totalX_tiles; ++i)
		gridmap[i] = new int[this->totalY_tiles];

	//populate with 0---------------------------------------------------------------------//
	for (int x = 0; x < this->totalX_tiles; ++x)
	{
		for (int y = 0; y < this->totalY_tiles; ++y)
		{
			gridmap[x][y] = 0;
		}
	}

	//Set the texture---------------------------------------------------------------//
	this->tileMap_tex = tileMap_tex;

	//Create draw quad--------------------------------------------------------------//
	mesh = MeshBuilder::GenerateCustomQuad(vertex_buffer_data, index_buffer_data, totalX_tiles, totalY_tiles, true);

	//tmp: call setup so at least can render complete tilemap
	mesh->SetTexture(tileMap_tex);
	transform.Scale(Vector3(tileScale * totalX_tiles, tileScale * totalY_tiles, 1.f));
	mesh->Setup(vertex_buffer_data, index_buffer_data, GL_TRIANGLES);
}

/********************************************************************************
Modify tile
********************************************************************************/
void Render_GridMap::ModifyTile(int x, int y, int tileType, int tilemap_sizeX, int tilemap_sizeY)
{
	//current tile to set from tilemap-------------------------------------//
	int xCur = tileType / tilemap_sizeX;
	int yCur = tileType % tilemap_sizeY;

	//percentage of size of a tile in tilemap-----------------------------//
	const float xUnit = 1.f / (float)tilemap_sizeX;
	const float yUnit = 1.f / (float)tilemap_sizeY;

	//modify vertex_buffer_data, the tile at (X, Y)'s texcoord
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 0].texcoord.Set(xCur * xUnit, yCur * yUnit);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 1].texcoord.Set(xCur * xUnit + xUnit, yCur * yUnit);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 2].texcoord.Set(xCur * xUnit + xUnit, yCur * yUnit + yUnit);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 3].texcoord.Set(xCur * xUnit, yCur * yUnit + yUnit);
}

void Render_GridMap::RecalculateMesh()
{
	//call setup and pass in our vertex_buffer_data
	mesh->SetupVertexOnly(vertex_buffer_data, index_buffer_data, GL_TRIANGLES);
}

/********************************************************************************
Draw mesh on screen
********************************************************************************/
void Render_GridMap::Draw()
{
	CU::view.SetIdentity();
	CU::view.LoadMatrix(transform.finalTRS);
	CU::view.RenderMesh(*mesh);
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
		CU::view.RenderMesh(*mesh);
		CU::view.Translate(offset.x, offset.y, offset.z);
	}
}