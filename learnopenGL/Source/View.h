#ifndef VIEW_H
#define VIEW_H
#include "Header.h"
#include "Camera.h"
#include "Text_Renderer.h"
#include "Mesh.h"
#include "Light.h"

/*********************************************************************************************************
New and improved view.

Author: Tan Yie Cher
Date: 23/6/2016

Notes:
1) ADD LIGHTS BEFORE CALLING VIEW INIT() !!!! If not uniforms not registered

Improvements:
-in RenderMesh(), we use if else to determine uniforms to pass in depending on shaders used, eliminate if else for each render call
-Solve jerkiness
-RenderText(), VAO pass in values at Init(), not Render()
/*********************************************************************************************************/
class View
{
public:

	/******************************************** The type of shader to use ***************************************************/
	enum SHADER_TYPE
	{
		LIGHT_SHADER,
		BASIC_SHADER,
		TEXT_SHADER,
		TOTAL_SHADERS,
	};

private:

	Camera* camera;
	SHADER_TYPE currentShader;

	/******************************************** OpenGL ***************************************************/
	static GLFWwindow* m_window;

	//Uniforms: Add your uniforms here for easier reference-------------------------//
	map<string, unsigned> uniformMap[TOTAL_SHADERS];

	/******************************************** Others ***************************************************/

	//Light---------------------------------//
	LightManager lightMan;

	//Text----------------------------------//
	FreeType_Text textMan;

public:

	/********************** constructor/destructor *****************************/
	View();
	~View();

	/********************** Core functions *****************************/
	void Init();
	void Exit();
	void Reset();	//changing scene etc. needs to reset lights etc.

	/********************** Add Lights *****************************/
	void AddDirLight(Vector3 dir, Color color);
	void AddPointLight(Vector3 pos);
	void AddSpotLight(Vector3 pos, Vector3 target, float cutoff_angle);

	/********************** Call this before/after any render functions *****************************/
	void ClearScreen();
	void Start_WorldRender(Camera& camera);	//render in world
	void Start_ScreenRender(float offsetX, float offsetY);	//render on screen
	void PostRender();

	/********************** The type of shader to use *****************************/
	void UseShader(SHADER_TYPE type);

	/********************** Transformation *****************************/
	void PushMatrix();
	void PopMatrix();
	void SetIdentity();
	void LoadMatrix(Mtx44& mat);

	void Translate(float x, float y, float z);
	void Rotate(float angle, float x, float y, float z);
	void Scale(float x, float y, float z);

	/********************** Render *****************************/
	void RenderMesh(Mesh& renderMe);
	void RenderText(string text, Vector2 pos, float yScale, Color color);

	/********************** Getter/setter *****************************/
	static GLFWwindow* getWindow();

private:

	/**************** set up, call in order ****************/
	void Setup();
	void InitShaders();
	void InitUniforms();
	void InitText();

	/**************** pre-render, call in order ****************/
	void CameraAndProj(Camera& camera);
	void CameraAndProj_OnScreen(float offsetX, float offsetY);

	/************* Transformation *****************/
	//Transform utility var----------------------------//
	Mtx44 mvMatrix;   //model view matrix
	MS modelStack;	//model matrix
	MS viewStack;	//view matrix
	MS projectionStack;	//perspective matrix

	/********************** Flags **********************/
	bool InitAlready;

	/********************** shader *********************************/
	unsigned m_programID;
	unsigned m_basicProgramID;

	//Text shaders, DOES NOT SHARE SAME ATTRIBUTES!!!!
	unsigned m_textVertArrayID;	//for text attributes
	unsigned text_vertexBuffer;	//text data aka mesh
	unsigned m_textProgramID;
};

#endif