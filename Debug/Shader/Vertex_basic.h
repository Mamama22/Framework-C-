#version 330 core

// Interpolated values from the vertex shaders
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

//Pass to fragment----------------------------------------//
out vec3 vertexColor; // Specify a color output to the fragment shader

//Uniforms-----------------------------------------------//
uniform mat4 uMV_Matrix;
uniform mat4 uP_Matrix;

void main()
{
	gl_Position = uP_Matrix * uMV_Matrix * vec4(position, 1.0);
	vertexColor = color;
}