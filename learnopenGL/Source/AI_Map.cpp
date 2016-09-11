#include "AI_Map.h"
#include "CoreUtilities.h"

AI_Map::AI_Map(){}
AI_Map::~AI_Map(){}

void AI_Map::Init(Vector3 pos, vector< vector<int> >& pathMap, float tileScale, int total_tiles_X, int totalTiles_Y)
{
	Entity::Init(Vector3(0,0,0), Vector3(1, 1, 1));

	//Add renderer------------------------------------------------//
	pathMap_renderer = new Render_GridMap;
	//intersectedGrid->Init("Asd", CU::shared.quad_start0, Vector3(0, 0, 0), Vector3(mapScaleX / (float)total_SP_X, mapScaleY / (float)total_SP_Y, 1));
	pathMap_renderer->Init("d", TILEMAP_AI, pos, tileScale, total_tiles_X, totalTiles_Y);

	for (int x = 0; x < total_tiles_X; ++x)
	{
		for (int y = 0; y < totalTiles_Y; ++y)
		{
			if (pathMap[x][y] >= 0)
				pathMap_renderer->SetTile(x, y, 1);
			else
				pathMap_renderer->SetTileEmpty(x, y);
		}
	}

	pathMap_renderer->RecalculateMesh();
	AddComponent(pathMap_renderer);
}