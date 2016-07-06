#include "View.h"

//screen dimensions----------------------------------//
//very small: 600, 450
//small screen size: 800, 600
//bigger byt still small: 900, 675
//big screen size: 1000, 750
//Extra big: 1150, 862.5
float Screen::SCREEN_WIDTH = 1000;
float Screen::SCREEN_HEIGHT = 750;
unsigned short Screen::CAMERA_WIDTH = 800.f;	//camera view size X 
unsigned short Screen::CAMERA_HEIGHT = 600.f;	//camera view size Y
float Screen::zoom_percent = 1.f;

const float Screen::CAM_X_TO_Y_RATIO = (float)Screen::CAMERA_WIDTH / (float)Screen::CAMERA_HEIGHT;

//OpenGL---------------------//
GLFWwindow* View::m_window;

/********************************************************************************
Callbacks
********************************************************************************/
//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

/********************************************************************************
CD
********************************************************************************/
View::View(){}
View::~View(){}

/********************************************************************************
Init wrapper
********************************************************************************/
void View::Init()
{
	//basic setup------------------------------------------//
	Setup();
	
	//shaders----------------------------------------------//
	InitShaders();

	//Uniforms---------------------------------------------//
	InitUniforms();

	//Text---------------------------------------------//
	InitText();

	//Misc-------------------------------------------------//
	camera = NULL;

	// Use our shader
	UseShader(LIGHT_SHADER);
}

/********************************************************************************
Setup basics
********************************************************************************/
void View::Setup()
{
	/** No need init so many times */
	if (InitAlready)
		return;

	InitAlready = true;

	/************ OpenGL ************/
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	
	//openGL 3.3
	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(Screen::SCREEN_WIDTH, Screen::SCREEN_HEIGHT, "Computer Graphics", NULL, NULL);
	
	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	//GLEW manages function pointers for OpenGL so initialize GLEW
	//before calling any openGL function
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/********************************************************************************
Setup shaders
********************************************************************************/
void View::InitShaders()
{
	//glGenVertexArrays(1, &m_vertexArrayID);	//normal

	//currently is normal vertex array-----------------------------------------//
	//glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Vertex.h", "Shader//Fragment.h");	//for everything else
	m_basicProgramID = LoadShaders("Shader//Vertex_basic.h", "Shader//Fragment_basic.h");	//for light
	m_textProgramID = LoadShaders("Shader//Vertex_text.h", "Shader//Fragment_text.h");	//for text
}

/********************************************************************************
Init Uniforms
********************************************************************************/
void View::InitUniforms()
{
	//Light shader---------------------------------------------------------------------------//
	uniformMap[LIGHT_SHADER]["uM_Matrix"] = glGetUniformLocation(m_programID, "uM_Matrix");
	uniformMap[LIGHT_SHADER]["uMV_Matrix"] = glGetUniformLocation(m_programID, "uMV_Matrix");
	uniformMap[LIGHT_SHADER]["uP_Matrix"] = glGetUniformLocation(m_programID, "uP_Matrix");
	uniformMap[LIGHT_SHADER]["u_EyePos"] = glGetUniformLocation(m_programID, "u_EyePos");

	//basic shader------------------------------------------------------------------------//
	uniformMap[BASIC_SHADER]["uMV_Matrix"] = glGetUniformLocation(m_basicProgramID, "uMV_Matrix");
	uniformMap[BASIC_SHADER]["uP_Matrix"] = glGetUniformLocation(m_basicProgramID, "uP_Matrix");

	//Dir Light uniforms------------------------------------------------//
	uniformMap[LIGHT_SHADER]["u_DirLight_Dir"] = glGetUniformLocation(m_programID, "u_DirLight_Dir");
	uniformMap[LIGHT_SHADER]["u_DirLight_Color"] = glGetUniformLocation(m_programID, "u_DirLight_Color");
	uniformMap[LIGHT_SHADER]["u_TotalDirLight"] = glGetUniformLocation(m_programID, "u_TotalDirLight");

	//Point Light uniforms------------------------------------------------//
	uniformMap[LIGHT_SHADER]["u_PointLight_Pos"] = glGetUniformLocation(m_programID, "u_PointLight_Pos");
	uniformMap[LIGHT_SHADER]["u_TotalPointLight"] = glGetUniformLocation(m_programID, "u_TotalPointLight");

	//Spot Light uniforms------------------------------------------------//
	uniformMap[LIGHT_SHADER]["u_SpotLight_Pos"] = glGetUniformLocation(m_programID, "u_SpotLight_Pos");
	uniformMap[LIGHT_SHADER]["u_SpotLight_Dir"] = glGetUniformLocation(m_programID, "u_SpotLight_Dir");
	uniformMap[LIGHT_SHADER]["u_SpotLight_Cutoff"] = glGetUniformLocation(m_programID, "u_SpotLight_Cutoff");
	uniformMap[LIGHT_SHADER]["u_TotalSpotLight"] = glGetUniformLocation(m_programID, "u_TotalSpotLight");

	//text shader--------------------------------------------------------//
	uniformMap[TEXT_SHADER]["uP_Matrix"] = glGetUniformLocation(m_textProgramID, "uP_Matrix");
	uniformMap[TEXT_SHADER]["textColor"] = glGetUniformLocation(m_textProgramID, "textColor");
}

/********************************************************************************
Init text
********************************************************************************/
void View::InitText()
{
	textMan.Init();

	//Gen attributes and buffer object----------------------------------------------//
	glGenVertexArrays(1, &m_textVertArrayID);	//attributes
	glGenBuffers(1, &text_vertexBuffer);	//buffer data

	//Buffer-----------------------------------------------//
	//the 2D quad requires 6 vertices of 4 floats each, 
	//6 vertices per quad
	//4 floats as per shader, 2 for pos 2 for texcoord
	glBindBuffer(GL_ARRAY_BUFFER, text_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 6 * 4, NULL, GL_DYNAMIC_DRAW);	//constantly update VBO when drawing, so GL_DYNAMIC_DRAW
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/********************************************************************************
Render text: SET text shader and on screen mode
********************************************************************************/
void View::RenderText(string text, Vector2 pos, float yScale, Color color)
{
	//Uniforms--------------------------------------------------------//
	glUniformMatrix4fv(uniformMap[TEXT_SHADER]["uP_Matrix"], 1, GL_FALSE, &projectionStack.Top().a[0]);
	glUniform3fv(uniformMap[TEXT_SHADER]["textColor"], 1, &color.r);

	//Bind Texture and VAO-------------------------------------------------------//
	glActiveTexture(GL_TEXTURE0);
	//glBindVertexArray(m_textVertArrayID);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = textMan.characters[*c];

		GLfloat xpos = pos.x + ch.bearing.x * yScale;
		GLfloat ypos = pos.y - (ch.size.y - ch.bearing.y) * yScale;

		GLfloat w = ch.size.x * yScale;
		GLfloat h = ch.size.y * yScale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		//Pass in texture-----------------------------------------------------//
		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		//Attrib object---------------------------------------------------------//
		glEnableVertexAttribArray(0);

		// Update content of VBO memory-----------------------------------------------------//
		glBindBuffer(GL_ARRAY_BUFFER, text_vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	//update buffer with new data
		
		//Attributes----------------------------------------------------------------------//
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

		// Render quad-----------------------------------------------------//
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Unbind----------------------------------------------------------//
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)-----------------------------------------------------//
		pos.x += (ch.advance >> 6) * yScale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	
	}
	
	//Unbind texture----------------------------------------------------//
	glBindTexture(GL_TEXTURE_2D, 0);
}

/********************************************************************************
Add lights:
********************************************************************************/
void View::AddDirLight(Vector3 dir, Color color)
{
	lightMan.Add_DirLight(dir, color);
}

void View::AddPointLight(Vector3 pos)
{
	lightMan.Add_PointLight(pos);
}

void View::AddSpotLight(Vector3 pos, Vector3 target, float cutoff_angle)
{
	lightMan.Add_SpotLight(pos, target, cutoff_angle);
}

/********************************************************************************
Pre render setup
********************************************************************************/
void View::Start_WorldRender(Camera& camera)
{
	this->camera = &camera;
	CameraAndProj(camera);
}

void View::Start_ScreenRender(float offsetX, float offsetY)
{
	CameraAndProj_OnScreen(offsetX, offsetY);
}

/********************************************************************************
Post render cleanup
********************************************************************************/
void View::PostRender()
{
	Mesh::ResetCurrentMesh();
	modelStack.Clear();
	projectionStack.Clear();
	viewStack.Clear();
}

/********************************************************************************
Choose the shader to use
********************************************************************************/
void View::UseShader(SHADER_TYPE type)
{
	currentShader = type;

	if (type == LIGHT_SHADER)
		glUseProgram(m_programID);
	else if (type == BASIC_SHADER)
		glUseProgram(m_basicProgramID);
	else if (type == TEXT_SHADER)
		glUseProgram(m_textProgramID);

	if (type == TEXT_SHADER)
		glBindVertexArray(m_textVertArrayID);
}

/********************************************************************************
Clear screen
********************************************************************************/
void View::ClearScreen()
{
	//clear screen after each render
	glClearColor(0.2f, 0.3f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/********************************************************************************
Setup camera and projection for world
********************************************************************************/
Mtx44 perspective;
void View::CameraAndProj(Camera& camera)
{
	glEnable(GL_DEPTH_TEST);

	//perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(0.f, Screen::CAMERA_WIDTH, 0.f, Screen::CAMERA_HEIGHT, -1000, 1000);	//2D
	perspective.SetToPerspective(45.f, Screen::CAM_X_TO_Y_RATIO, 0.1f, 500.f);	//3D
	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

/********************************************************************************
Setup camera and projection for on screen:
-camera position does not matter
********************************************************************************/
Mtx44 ortho;
void View::CameraAndProj_OnScreen(float offsetX, float offsetY)
{
	glDisable(GL_DEPTH_TEST);

	ortho.SetToOrtho(offsetX, Screen::CAMERA_WIDTH + offsetX, offsetY, Screen::CAMERA_HEIGHT + offsetY, -10.f, 10.f);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
}

/********************************************************************************
Transformation
********************************************************************************/
void View::PushMatrix()
{
	modelStack.PushMatrix();
}

void View::PopMatrix()
{
	modelStack.PopMatrix();
}

void View::SetIdentity()
{
	modelStack.LoadIdentity();
}

void View::Translate(float x, float y, float z)
{
	modelStack.Translate(x, y, z);
}

void View::Rotate(float angle, float x, float y, float z)
{
	modelStack.Rotate(angle, x, y, z);
}

void View::Scale(float x, float y, float z)
{
	modelStack.Scale(x, y, z);
}

/********************************************************************************
Render
********************************************************************************/
void View::RenderMesh(Mesh& renderMe)
{
	//Get MV matrix---------------------------------------------------------//
	mvMatrix = viewStack.Top() * modelStack.Top();
	
	//Pass in uniforms------------------------------------------------------//
	if (currentShader == LIGHT_SHADER)
	{
		glUniformMatrix4fv(uniformMap[LIGHT_SHADER]["uM_Matrix"], 1, GL_FALSE, &modelStack.Top().a[0]);
		glUniformMatrix4fv(uniformMap[LIGHT_SHADER]["uMV_Matrix"], 1, GL_FALSE, &mvMatrix.a[0]);
		glUniformMatrix4fv(uniformMap[LIGHT_SHADER]["uP_Matrix"], 1, GL_FALSE, &projectionStack.Top().a[0]);
		glUniform3fv(uniformMap[LIGHT_SHADER]["u_EyePos"], 1, &camera->position.x);

		//Dir light uniform----------------------------------------------------//
		glUniform3fv(uniformMap[LIGHT_SHADER]["u_DirLight_Dir"], lightMan.total_dir, &lightMan.dir_direction[0].x);
		glUniform3fv(uniformMap[LIGHT_SHADER]["u_DirLight_Color"], lightMan.total_dir, &lightMan.dir_color[0].r);
		glUniform1i(uniformMap[LIGHT_SHADER]["u_TotalDirLight"], lightMan.total_dir);

		//Point light uniform----------------------------------------------------//
		glUniform3fv(uniformMap[LIGHT_SHADER]["u_PointLight_Pos"], lightMan.total_point, &lightMan.point_pos[0].x);
		glUniform1i(uniformMap[LIGHT_SHADER]["u_TotalPointLight"], lightMan.total_point);

		//spot light uniform----------------------------------------------------//
		glUniform3fv(uniformMap[LIGHT_SHADER]["u_SpotLight_Pos"], lightMan.total_spot, &lightMan.spot_pos[0].x);
		glUniform3fv(uniformMap[LIGHT_SHADER]["u_SpotLight_Dir"], lightMan.total_spot, &lightMan.spot_direction[0].x);
		glUniform1fv(uniformMap[LIGHT_SHADER]["u_SpotLight_Cutoff"], lightMan.total_spot, &lightMan.spot_cutoff[0]);
		glUniform1i(uniformMap[LIGHT_SHADER]["u_TotalSpotLight"], lightMan.total_spot);
	}
	else
	{
		glUniformMatrix4fv(uniformMap[BASIC_SHADER]["uMV_Matrix"], 1, GL_FALSE, &mvMatrix.a[0]);
		glUniformMatrix4fv(uniformMap[BASIC_SHADER]["uP_Matrix"], 1, GL_FALSE, &projectionStack.Top().a[0]);
	}

	//Render the mesh-------------------------------------------------------//
	renderMe.Render();
}

/********************************************************************************
Reset
********************************************************************************/
void View::Reset()
{
	lightMan.Remove_All();
}

/********************************************************************************
Exit
********************************************************************************/
void View::Exit()
{
	//text shader VAO and VBO-------------//
	glDeleteBuffers(1, &text_vertexBuffer);

	//Text manager-------------------------//
	textMan.Exit();

	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
	m_window = NULL;
}

/********************** openGL *********************************/
GLFWwindow* View::getWindow(){return m_window;}