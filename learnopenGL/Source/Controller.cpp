#include "Controller.h"
const unsigned char Controller::FPS = 1000; // FPS of this game
const unsigned int Controller::frameTime = 1000 / FPS; // time for each frame
ostringstream ss;

/********************************************************************************
Init stuff
********************************************************************************/
void Controller::Init()
{
	//Timers: set both timers to 0-----------------------------//
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_thread1 = 0.0;
	m_dAccumulatedTime_thread2 = 0.0;

	//camera------------------------------------------------------//
	camera.Init(Vector3(0.f, 0.f, 10.f), Vector3(0.f, 0.f, 5.f), Vector3(0.f, 1.f, 0.f));

	//Lighting: Call before View init() !!!-------------------------------------//
	CU::view.AddDirLight(Vector3(0.f, -1.f, 0.f), Color(255.f, 255.f, 255.f));
	CU::view.AddPointLight(Vector3(-4.f, 4.f, 5.f));
	CU::view.AddSpotLight(Vector3(-12.f, 1.f, 7.f), Vector3(-12.5f, 0.f, 7.f), 30.f);
	CU::view.AddSpotLight(Vector3(13.f, 1.f, 6.f), Vector3(15.f, 0.f, 6.f), 35.f);
	
	//Core utils init------------------------------//
	CU::Init();
	CU::input.SetMouse(true);

	//Meshes---------------------------------------//
	sphere = MeshBuilder::GenerateSphere(36, 36, 0.5f);
	quad = MeshBuilder::GenerateQuad(1.f, 1.f, false);
	axes = MeshBuilder::GenerateAxes();
}

/********************************************************************************
Run stuff
********************************************************************************/
void Controller::Run()
{
	//Main Loop
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	double aTime = 0;
	bool b = false;

	//quit owhen previous frame initiates
	while (!glfwWindowShouldClose(View::getWindow()) && !CU::input.IsKeyPressed(Input::Q))
	{
		//=============================================================================================================================================//
		//get the elapsed time--------------------------------------//
		m_dElapsedTime = m_timer.getElapsedTime();
		aTime += m_dElapsedTime;
		m_dAccumulatedTime_thread1 += m_dElapsedTime;
		m_dAccumulatedTime_thread2 += m_dElapsedTime;
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		//fps------------------------------------------------//
		fps = (float)(1.f / m_dElapsedTime);
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		/* Update */
		if (m_dAccumulatedTime_thread1 > 0.01)	//update: update fps is _dAccumulatedTime_thread1 > fps
		{
			m_dAccumulatedTime_thread1 = 0.0;
	
			//call before all-----------------------------------------//
			CU::input.CheckForKeyPresses();

			//cam/Mouse update----------------------------------------------//
			camera.Update(m_dElapsedTime);
			camera.UpdateInput(m_dElapsedTime);

			//check if any input pressed------------------------------//
			if (fps <= 40.f)
				cout << fps << endl;
		}
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		/* Render */
		if (m_dAccumulatedTime_thread2 > 0.005)	//render: render fps is _dAccumulatedTime_thread1 > fps
		{
			CU::view.ClearScreen();

			///** View update(rendering) **/
			CU::view.Start_WorldRender(camera);

			CU::view.UseShader(View::BASIC_SHADER);	//use basic shader

			CU::view.SetIdentity();
			CU::view.Scale(2000.f, 2000.f, 2000.f);
			CU::view.RenderMesh(*axes);

			//Point light----------------------------------//
			CU::view.SetIdentity();
			CU::view.Translate(-2.f, 2.f, 2.5f);
			CU::view.Scale(0.3f, 0.3f, 0.3f);
			CU::view.RenderMesh(*sphere);

			//spot light------------------------------------//
			CU::view.SetIdentity();
			CU::view.Translate(-12.f, 1.f, 7.f);
			CU::view.Scale(0.2f, 0.2f, 0.2f);
			CU::view.RenderMesh(*sphere);

			CU::view.SetIdentity();
			CU::view.Translate(13.f, 1.f, 6.f);
			CU::view.Scale(0.2f, 0.2f, 0.2f);
			CU::view.RenderMesh(*sphere);

			CU::view.UseShader(View::LIGHT_SHADER);	//use light shader

			CU::view.SetIdentity();
			CU::view.Translate(-2.f, 2.f, 0.f);
			CU::view.Scale(2.f, 2.f, 2.f);
			CU::view.RenderMesh(*sphere);

			CU::view.SetIdentity();
			CU::view.Translate(-2.f, -2.5f, 0.f);
			CU::view.Rotate(-90.f, 1.f, 0.f, 0.f);
			CU::view.Scale(100.f, 100.f, 1.f);
			CU::view.RenderMesh(*quad);

			//text-----------------------------------//
			CU::view.Start_ScreenRender();
			CU::view.UseShader(View::TEXT_SHADER);	//use light shader

			//test quad-----------------------------------------//
			ss.str("");
			ss << "FPS: " << fps;
			string theFPS = ss.str();
			int findDot = theFPS.find('.');
			theFPS = theFPS.substr(0, findDot);
			CU::view.RenderText(theFPS, Vector2(10.f, 550.f), 0.5f, Color(0.f, 255.f, 255.f));
			CU::view.RenderText("OWL CITY", Vector2(500.f, 10.f), 1.f, Color(242.f, 215.f, 104.f));

			CU::view.PostRender();

			m_dAccumulatedTime_thread2 = 0.0;
		}
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		//Swap buffers
		glfwSwapBuffers(View::getWindow());
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		//=============================================================================================================================================//

		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
	}
}

/********************************************************************************
Exit stuff
********************************************************************************/
void Controller::Exit()
{
	delete sphere;
	delete quad;
	delete axes;

	//Core utils exit------------------------------//
	CU::Exit();
}