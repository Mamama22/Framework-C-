#include "Scene_PlaneInter.h"

Scene_PlaneInter::Scene_PlaneInter()
{
}


Scene_PlaneInter::~Scene_PlaneInter(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_PlaneInter::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//ray-------------------------------------//
	rayAngle = 0.f;
	rayLength = 1000.f;
	rayPos.Set(0, 0, 0);

	//plane--------------------------------//
	planePos.Set(100, 100, 0);
	planeAngle = 210;
	intersectedPlane = 0;
	planeLength = 200.f;

	planeNormal.x = cos(Math::DegreeToRadian(planeAngle));
	planeNormal.y = sin(Math::DegreeToRadian(planeAngle));
}

/********************************************************************************
Utilities
********************************************************************************/
void Scene_PlaneInter::RotateRay(float amt)
{
	rayAngle += amt;
	if (rayAngle > 360.f)
		rayAngle -= 360.f;
	else if (rayAngle < 0.f)
		rayAngle += 360.f;
}

/********************************************************************************
n: ray normal
po: plane center pos
l0: ray origin pos

t: the dist from l0 to p0
********************************************************************************/
bool intersectPlane(const Vector3 &n, const Vector3 &p0, const Vector3 &l0, const Vector3 &l, float &t)
{
	// assuming vectors are all normalized, so dot product order does not matter
	float denom = n.Dot(l);

	//cout << "Denom: " << denom << endl;

	if (denom > 1e-6) 
	{
		Vector3 p0l0 = p0 - l0;
		t = p0l0.Dot(n) / denom;
		return (t >= 0);
	}

	return false;
}


/********************************************************************************
check intersection with plane
********************************************************************************/
void Scene_PlaneInter::CheckIntersection()
{
	Vector3 rayNormal;

	rayNormal.x = cos(Math::DegreeToRadian(rayAngle));
	rayNormal.y = sin(Math::DegreeToRadian(rayAngle));
	float t = 0.f;

	//cout << "Ray normal: " << rayNormal << endl;

	intersectedPlane = intersectPlane(-planeNormal, planePos, rayPos, rayNormal, t);

	if (intersectedPlane)
	{
		intersectedPos = rayPos + rayNormal * t;

		//check if intersected pos is "out of range"
		float lensq = (intersectedPos - planePos).LengthSquared();
		float halfLenPlane = planeLength * 0.5f;
		
		//out of range of the plane width
		if (lensq > halfLenPlane * halfLenPlane)
			intersectedPlane = false;
	}
}

/********************************************************************************
Run
********************************************************************************/
void Scene_PlaneInter::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Stage 1: States, flags and values update ===========================================================//

	//Control---------------------------------------------------------------------//
	/*if (CU::input.IsKeyPressed(Input::ARROW_UP))
		spherePos.y += 2.f;
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		spherePos.y -= 2.f;
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		spherePos.x -= 2.f;
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		spherePos.x += 2.f;*/

	if (CU::input.IsKeyPressed(Input::C))	//turn left
	{
		RotateRay(1.f);
	}
	if (CU::input.IsKeyPressed(Input::B))	//turn right
	{
		RotateRay(-1.f);
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
void Scene_PlaneInter::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_PlaneInter::DrawOnScreen()
{
	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*CU::shared.axes);

	//plane----------------------------------------------------//
	glLineWidth(3.5f);
	CU::view.SetIdentity();
	CU::view.Translate(planePos.x, planePos.y, 1.f);
	CU::view.Rotate(planeAngle + 90.f, 0.f, 0.f, 1.f);
	CU::view.Scale(planeLength, 1.f, 1.f);
	CU::view.RenderMesh(*CU::shared.line_3);
	glLineWidth(1.f);

	//normal------------//
	glLineWidth(2.5f);
	CU::view.SetIdentity();
	CU::view.Translate(planePos.x, planePos.y, 1.f);
	CU::view.Rotate(planeAngle, 0.f, 0.f, 1.f);
	CU::view.Scale(100.f, 1.f, 1.f);
	CU::view.RenderMesh(*CU::shared.line_start0_1);
	glLineWidth(1.f);

	//ray----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Translate(rayPos.x, rayPos.y, rayPos.z);
	CU::view.Rotate(rayAngle, 0, 0, 1);

	float len = rayLength;

	if (intersectedPlane)
		len = intersectedPos.Length();

	CU::view.Scale(len, len, 1.f);
	CU::view.RenderMesh(*CU::shared.line_start0);


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
	if (intersectedPlane)
	{
		CU::view.RenderText("INTERSECTED", Vector2(100.f, 250.f), 1.f, Color(193, 240, 180));
	}
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_PlaneInter::Exit()
{
	//Call parent--------------------------------------//
	Scene::Exit();
}