#version 330 core

//From vertex------------------------------------------------//
in vec3 vertexColor; // The input variable from the vertex shader (same name and same type)


//To geometry----------------------------------------------//
out vec4 color;

void main()
{
	color = vec4(vertexColor, 1.0);
}