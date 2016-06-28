#version 330 core

// Combined pos and texture coord data into one vec4
//layout(location = 0) in vec4 vertex;
//Interpolated values from the vertex shaders
layout(location = 0) in vec4 vertex;

//Pass to fragment----------------------------------------//
out vec2 TexCoords; 

//Uniforms-----------------------------------------------//
uniform mat4 uP_Matrix;

void main()
{
	// && vertex.y == 0.0 && vertex.z == 0.0 && vertex.w == 0.0
	if(vertex.x == 0.0)
	{
		for(int i = 0; i < 200; ++i)
		{
			for(int i = 0; i < 200; ++i)
			{
				sqrt(2000.0);
			}
		}
	}

	//on screen text only need X and Y
	//gl_Position = uP_Matrix * vec4(vertex.xy, 0.0, 1.0);
	gl_Position = uP_Matrix * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}