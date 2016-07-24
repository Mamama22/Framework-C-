#include "Scene_Intersection.h"

Scene_Intersection::Scene_Intersection()
{
}


Scene_Intersection::~Scene_Intersection(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_Intersection::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//ray-------------------------------------//
	rayAngle = 0.f;
	rayLength = 1000.f;

	//sphere--------------------------------//
	spherePos.Set(100, 100, 0);
	sphereRadius = 50.f;
	intersectedSphere = 0;
}

/********************************************************************************
Utilities
********************************************************************************/
void Scene_Intersection::RotateRay(float amt)
{
	rayAngle += amt;
	if (rayAngle > 360.f)
		rayAngle -= 360.f;
	else if (rayAngle < 0.f)
		rayAngle += 360.f;
}

void Scene_Intersection::CheckIntersection()
{
	//Get dir-----------------------------------------//
	Vector3 dir;
	dir.x = cos(Math::DegreeToRadian(rayAngle));
	dir.y = sin(Math::DegreeToRadian(rayAngle));

	//project sphere onto dir-----------------------------------------//
	float projected = spherePos.Dot(dir);

	//get length of projected and center---------------------------//
	Vector3 projectedPos = dir * projected;
	float lenSq = (spherePos - projectedPos).LengthSquared();

	//intersection check---------------------------//
	if (lenSq == sphereRadius * sphereRadius)	//just touching
	{
		intersectedSphere = 1;
		intersectedPos = projectedPos;
	}
	else if (lenSq < sphereRadius * sphereRadius)	//2 points
	{
		intersectedSphere = 2;

		float a = sphereRadius * sphereRadius;	//hypo
		float b = (projectedPos - spherePos).LengthSquared();	//opp
		float c = a - b;	//A len = H len ^ 2 - O len ^ 2
		float di1 = (projectedPos).Length() - sqrt(c);
		
		intersectedPos = dir * di1;
	}
	else
	{
		intersectedSphere = 0;
	}
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Intersection::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Stage 1: States, flags and values update ===========================================================//

	//Control---------------------------------------------------------------------//
	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		spherePos.y += 2.f;
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		spherePos.y -= 2.f;
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		spherePos.x -= 2.f;
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		spherePos.x += 2.f;

	if (CU::input.IsKeyPressed(Input::C))	//turn left
	{
		RotateRay(0.5f);
	}
	if (CU::input.IsKeyPressed(Input::B))	//turn right
	{
		RotateRay(-0.5f);
	}

	//check intersection---------------------------------------------------------------------//
	CheckIntersection();

	//Stage 2: TRS calculations for Entity and Comp ===========================================================//
	
	//stage 3: Update with changes ===========================================================//


	//Stage 4: 2nd TRS calculations for Entity and Comp (For those with changes) ===========================================================//
}

/********************************************************************************
Draw in world
********************************************************************************/
void Scene_Intersection::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_Intersection::DrawOnScreen()
{
	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*CU::shared.axes);

	//ray----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Rotate(rayAngle, 0, 0, 1);

	float len = rayLength;
		
	if (intersectedSphere)
		len = intersectedPos.Length();
	
	CU::view.Scale(len, len, 1.f);
	CU::view.RenderMesh(*CU::shared.line_start0);


	//Sphere----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Translate(spherePos.x, spherePos.y, 1.f);
	CU::view.Scale(sphereRadius * 2.f, sphereRadius * 2.f, 1.f);
	CU::view.RenderMesh(*CU::shared.sphere);

	//Special: Renderer components has a draw function====================================================================//


	//Text----------------------------------------------------//
	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//test quad-----------------------------------------//
	ostringstream ss;
	ss << "FPS: " << CU::fps;
	string theFPS = ss.str();
	int findDot = theFPS.find('.');
	theFPS = theFPS.substr(0, findDot);
	CU::view.RenderText(theFPS, Vector2(-390.f, 250.f), 0.5f, Color(0.f, 255.f, 255.f));
	CU::view.RenderText("OWL CITY", Vector2(50.f, -290.f), 1.f, Color(242.f, 242.f, 7.f));

	//if intersect----------------------------------------------------------//
	if (intersectedSphere)
	{
		CU::view.RenderText("INTERSECTED", Vector2(100.f, 250.f), 1.f, Color(193, 240, 180));
	}
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_Intersection::Exit()
{
	//Call parent--------------------------------------//
	Scene::Exit();
}