#include "Renderer_GridMap.h"
#include "CoreUtilities.h"

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
void Render_GridMap::Init(const char* name, TILEMAP_ENUM tileMap_tex, Vector3 pos, float tileScale, int totalX_tiles, int totalY_tiles)
{
	Renderer::Init(name, NULL, pos, Vector3(1, 1, 1));

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
	Mesh_Tilemap* mesh_tilemap = MeshBuilder::GenerateTilemapQuad(vertex_buffer_data, index_buffer_data, totalX_tiles, totalY_tiles, true);

	//tmp: call setup so at least can render complete tilemap----------------------------------------------------//
	mesh_tilemap->SetTilemapTex(tileMap_tex);
	transform.Scale(Vector3(tileScale * totalX_tiles, tileScale * totalY_tiles, 1.f));
	mesh_tilemap->Setup(vertex_buffer_data, index_buffer_data, GL_TRIANGLES);

	//set the main mesh to tilemap mesh
	mesh = mesh_tilemap;
}

/********************************************************************************
Modify tile
********************************************************************************/
void Render_GridMap::SetTile(int x, int y, int tileType)
{
	float mesh_TS = 1.f / tileScale;

	//current tile to set from tilemap-------------------------------------//
	int xCur = tileType / CU::shared.tilemapList[tileMap_tex].total_tilesX();
	int yCur = tileType % CU::shared.tilemapList[tileMap_tex].total_tilesY();

	//percentage of size of a tile in tilemap-----------------------------//
	const float xUnit = 1.f / (float)CU::shared.tilemapList[tileMap_tex].total_tilesX();
	const float yUnit = 1.f / (float)CU::shared.tilemapList[tileMap_tex].total_tilesY();

	//size of a tile in mesh---------------------------------------------//
	const float xT = 1.f / (float)totalX_tiles;
	const float yT = 1.f / (float)totalY_tiles;

	//Set the (X, -Y) and  (X, Y) vertex pos in case it's been modified-----------------------------------------------//
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 1].pos.Set(x * xT + xT, y * yT, 0);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 2].pos.Set(x * xT + xT, y * yT + yT, 0);

	//modify vertex_buffer_data, the tile at (X, Y)'s texcoord-----------------------------------------------//
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 0].texcoord.Set(xCur * xUnit, yCur * yUnit);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 1].texcoord.Set(xCur * xUnit + xUnit, yCur * yUnit);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 2].texcoord.Set(xCur * xUnit + xUnit, yCur * yUnit + yUnit);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 3].texcoord.Set(xCur * xUnit, yCur * yUnit + yUnit);
}

void Render_GridMap::SetAllTiles(int tileType)
{
	//current tile to set from tilemap-------------------------------------//
	int xCur = tileType / CU::shared.tilemapList[tileMap_tex].total_tilesX();
	int yCur = tileType % CU::shared.tilemapList[tileMap_tex].total_tilesY();

	//percentage of size of a tile in tilemap-----------------------------//
	const float xUnit = 1.f / (float)CU::shared.tilemapList[tileMap_tex].total_tilesX();
	const float yUnit = 1.f / (float)CU::shared.tilemapList[tileMap_tex].total_tilesY();

	//size of a tile in mesh---------------------------------------------//
	const float xT = 1.f / (float)totalX_tiles;
	const float yT = 1.f / (float)totalY_tiles;

	for (int x = 0; x < totalX_tiles; x += 4)
	{
		for (int y = 0; y < totalY_tiles; y += 4)
		{
			//Set the (X, -Y) and  (X, Y) vertex pos in case it's been modified-----------------------------------------------//
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 1].pos.Set(x * xT + xT, y * yT, 0);
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 2].pos.Set(x * xT + xT, y * yT + yT, 0);

			//modify vertex_buffer_data, the tile at (X, Y)'s texcoord-----------------------------------------------//
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 0].texcoord.Set(xCur * xUnit, yCur * yUnit);
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 1].texcoord.Set(xCur * xUnit + xUnit, yCur * yUnit);
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 2].texcoord.Set(xCur * xUnit + xUnit, yCur * yUnit + yUnit);
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 3].texcoord.Set(xCur * xUnit, yCur * yUnit + yUnit);
		}
	}
}

void Render_GridMap::SetTileEmpty(int x, int y)
{
	//size of a tile in mesh---------------------------------------------//
	const float xT = 1.f / (float)totalX_tiles;
	const float yT = 1.f / (float)totalY_tiles;

	//modify vertex_buffer_data, the tile at (X, Y)'s texcoord
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 1].pos.Set(x * xT, y * yT, 0);
	vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 2].pos.Set(x * xT, y * yT + yT, 0);
}

void Render_GridMap::SetAllTilesEmpty()
{
	//size of a tile in mesh---------------------------------------------//
	const float xT = 1.f / (float)totalX_tiles;
	const float yT = 1.f / (float)totalY_tiles;

	for (int x = 0; x < totalX_tiles; x++)
	{
		for (int y = 0; y < totalY_tiles; y++)
		{
			//modify vertex_buffer_data, the tile at (X, Y)'s texcoord
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 1].pos.Set(x * xT, y * yT, 0);
			vertex_buffer_data[(y * 4) + (x * (totalY_tiles * 4)) + 2].pos.Set(x * xT, y * yT + yT, 0);
		}
	}
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
	mesh->Render(alpha);
}
