#include "Scene_MapEditor.h"

Scene_MapEditor::Scene_MapEditor()
{
}


Scene_MapEditor::~Scene_MapEditor(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_MapEditor::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//gridmap--------------------------------------------------------//
	InitGridmap();

	//tiles---------------------------------------------//
	x_inter = y_inter = -1;
	inter_tileScale = 0.0f;
}

/********************************************************************************
Init gridmap
********************************************************************************/
void Scene_MapEditor::InitGridmap()
{
	//gridmap---------------------------------------------------------//
	gridmap = new GridMap;
	//pos, mesh, tilescale, total tiles X, total tiles Y, total X grids, total Y grids, tilemap size X, tilemap size Y, total SP X, total SP Y
	gridmap->Init(Vector3(-400, -300, 0), TILEMAP_MC, 23.f, 25, 25, 2, 2);

	////add a layer---------------------------------------------------//
	//gridlayer = new GridLayer;
	//gridlayer->Init("d", TILEMAP_MC, Vector3(-400, -300, 0));
	//gridmap->AddChildren(gridlayer);

	////modify tiles of layer------------------------------------------------//
	//for (int x = 0; x < gridlayer->Get_TotalTilesX(); ++x)
	//{
	//	for (int y = 0; y < gridmap->Get_TotalTilesY(); ++y)
	//	{
	//		int tileType = 0;	//tilemap size is 4
	//		gridlayer->ModifyTile(tileType, x, y);
	//	}
	//}
	//gridlayer->RecalculateMesh();

	//add a layer---------------------------------------------------//
	gridLayer = new GridLayer;
	gridLayer->Init("d", TILEMAP_MC, Vector3(-400, -300, 0));
	gridmap->AddChildren(gridLayer);

	//modify tiles of layer------------------------------------------------//
	for (int x = 0; x < gridLayer->Get_TotalTilesX(); ++x)
	{
		for (int y = 0; y < gridmap->Get_TotalTilesY(); ++y)
		{
			int tileType = rand() % CU::shared.tilemapList[gridLayer->Get_TilemapEnum()].total_tiles();	//tilemap size is 4
			gridLayer->ModifyTile(tileType, x, y);
		}
	}

	//set middle row empty
	for (int x = 0; x < gridLayer->Get_TotalTilesX(); ++x)
		gridLayer->SetTileEmpty(x, 10);

	gridLayer->RecalculateMesh();
}

/********************************************************************************
Update player input
********************************************************************************/
void Scene_MapEditor::UpdatePlayerInput()
{
	if (CU::input.IsKeyPressed(Input::W))
		gridmap->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::S))
		gridmap->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::A))
		gridmap->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::D))
		gridmap->Translate(Vector3(2.f, 0, 0));
}

/********************************************************************************
Run
********************************************************************************/
void Scene_MapEditor::Run_Stage1()
{
	//Call parent--------------------------------------//
	Scene::Run_Stage1();

	UpdatePlayerInput();
}

/********************************************************************************
Run
********************************************************************************/
void Scene_MapEditor::Run_Stage3()
{
	Scene::Run_Stage3();

	//check cursor intersected active layer-----------------------------//
	gridLayer->GetIntersected_Tile(CU::input.GetCursorPos_World2D(), x_inter, y_inter, inter_tilePos);
	inter_tileScale = gridLayer->Get_TileScale();
}

/********************************************************************************
Draw in world (3D)
********************************************************************************/
void Scene_MapEditor::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen (2D)
********************************************************************************/
void Scene_MapEditor::DrawOnScreen()
{
	//Axes----------------------------------------------------//
	/*CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::shared.axes->Render();*/

	////cursor-----------------------------------------------//
	//CU::view.SetIdentity();
	//Vector3 asdsa = CU::input.GetCursorPos();
	//asdsa.x -= Screen::CAMERA_WIDTH * 0.5f;
	//asdsa.y -= Screen::CAMERA_HEIGHT * 0.5f;
	//CU::view.Translate(asdsa.x, asdsa.y, 0.f);
	//CU::view.Scale(45.f, 45.f, 1.f);
	//CU::shared.quad->SetTexture(TEX_CURSOR);
	//CU::shared.quad->Render();

	//highlighted tile----------------------------------------//
	if (x_inter != -1)
	{
		CU::view.Draw_HollowQuad(inter_tilePos, inter_tileScale);
	}
}

/********************************************************************************
Draw GUI
********************************************************************************/
void Scene_MapEditor::DrawGUI()
{
	//Text----------------------------------------------------//
	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//test quad-----------------------------------------//
	ostringstream ss;
	ss << "FPS: " << CU::fps;
	string theFPS = ss.str();
	int findDot = theFPS.find('.');
	theFPS = theFPS.substr(0, findDot);
	CU::view.RenderText(theFPS, Vector2(-390.f, 250.f), 0.5f, Color(0.f, 255.f, 255.f));

	ss.str("");
	ss << "TRS cal count: " << Transformation::TRS_cal_count;
	CU::view.RenderText(ss.str(), Vector2(-390.f, 190.f), 0.55f, Color(24.f, 196.f, 87.f));
	CU::view.RenderText("OWL CITY", Vector2(50.f, -290.f), 1.f, Color(237, 132, 26));
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_MapEditor::Exit()
{
	//Call parent--------------------------------------//
	Scene::Exit();
}