#version 330 core

// Interpolated values from the vertex shaders
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoord;

//Pass to fragment----------------------------------------//
out vec3 vertexColor; // Specify a color output to the fragment shader
out vec3 vertexPos;
out vec3 vertexNormal;
out vec2 vertexTexcoord;

//Uniforms-----------------------------------------------//
uniform mat4 uM_Matrix;
uniform mat4 uMV_Matrix;
uniform mat4 uP_Matrix;

void main()
{
	gl_Position = uP_Matrix * uMV_Matrix * vec4(position, 1.0);
	vertexColor = color;
	
	 // Transform the normal's orientation into eye space-----------------------//
    vertexNormal = vec3(uM_Matrix * vec4(normal, 0.0));
	
	// Transform the vertex into eye space-----------------------------------//
	vertexPos = vec3(uM_Matrix * vec4(position, 1.0));
	
	//texcoord--------------------------------------------------------------//
	vertexTexcoord = texcoord;
}