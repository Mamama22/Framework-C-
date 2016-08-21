#version 330 core

//From vertex------------------------------------------------//
in vec3 vertexColor; // The input variable from the vertex shader (same name and same type)
in vec2 vertexTexcoord;

//To geometry----------------------------------------------//
out vec4 color;

uniform bool u_TextureEnabled;
uniform sampler2D u_Texture;

void main()
{
	if(u_TextureEnabled == true)
		color = texture2D( u_Texture, vertexTexcoord );
	else
		color = vec4(vertexColor, 1.0);
}