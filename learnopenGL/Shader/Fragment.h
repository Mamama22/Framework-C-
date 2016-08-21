#version 330 core

//From vertex------------------------------------------------//
in vec3 vertexColor; // The input variable from the vertex shader (same name and same type)
in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTexcoord;

//To geometry----------------------------------------------//
out vec4 color;

//dir light-------------------------------//
uniform vec3 u_DirLight_Dir[2];
uniform vec3 u_DirLight_Color[2];

//point light-------------------------------//
uniform vec3 u_PointLight_Pos[2];	//all point light are white

//spot light-------------------------------//
uniform vec3 u_SpotLight_Pos[3];	
uniform vec3 u_SpotLight_Dir[3];	
uniform float u_SpotLight_Cutoff[3];	

//Light count----------------------------//
uniform int u_TotalDirLight;
uniform int u_TotalPointLight;
uniform int u_TotalSpotLight;

uniform vec3 u_EyePos;

//Texture----------------------------//
uniform bool u_TextureEnabled;
uniform sampler2D u_Texture;

/********************************************************************************************************************************
Get the basic 3 factors of light
********************************************************************************************************************************/
vec3 GetLightBasic(vec3 lightColor, vec3 lightDir, vec3 objNormal, vec3 VertToEye) {

	vec3 ambient = vec3(0.0);
	vec3 diffuseColor = vec3(0.0);
	vec3 specularColor = vec3(0.0);
	
	 //ambient-------------------------------//
	 ambient += lightColor * 0.05;
	  
	  //diffuse-------------------------------//
	  float diffuseFactor = dot(objNormal, -lightDir) * 0.25;
	  
	  if(diffuseFactor > 0.0)
	  {
		   diffuseColor += lightColor * diffuseFactor;
		   
		   //specular---------------------------------//
		   //Get the reflected ray dir vector
		   //vec3 Reflect = normalize(u_LightDir - (NobjNormal * 2.0 * dot(NobjNormal, u_LightDir)));
		   vec3 Reflect = normalize(reflect(lightDir, objNormal));
		   float specularFactor = dot(VertToEye, Reflect);
		  
			if(specularFactor > 0.0)
			{
				specularFactor = pow(specularFactor, 15.0);
				specularColor += lightColor * 0.2 * specularFactor;
			}
		}

		return ambient + diffuseColor + specularColor;
}

/********************************************************************************************************************************
get point light
********************************************************************************************************************************/
vec3 CalPointLight(vec3 lightPos, vec3 LightToObjDir, vec3 objNormal, vec3 VertToEye) {
	
	//variables----------------------------//
	float distance = length(vertexPos - lightPos);
	
	vec3 color = GetLightBasic(vec3(1.0, 1.0, 1.0), -LightToObjDir, objNormal, VertToEye);
	
	//attenuation------------------------------//
	float attenuation = 1.0 + 0.1 * distance + 0.05 * distance * distance;
	return color * (1.0 / attenuation) * 4.0;
}

/********************************************************************************************************************************
get spot light
********************************************************************************************************************************/
vec3 CalSpotLight(vec3 lightPos, vec3 lightDir, vec3 LightToObjDir, float cutoff, vec3 objNormal, vec3 VertToEye) {
	
	//dot the light dir and 
	float spotFactor = dot(lightDir, -LightToObjDir);
	
	vec3 baseColor = vec3(0.0);
	
	if(spotFactor > cutoff) {
		baseColor = CalPointLight(lightPos, LightToObjDir, objNormal, VertToEye);
		baseColor = baseColor * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - cutoff));
	}
	return baseColor;
}

/********************************************************************************************************************************
Main function
********************************************************************************************************************************/
void main()
{
	vec3 NobjNormal = normalize(vertexNormal);
	vec3 VertToEye =  normalize(u_EyePos - vertexPos);
	
	vec3 totalColor = vec3(0.0);

	//Dir light----------------------------------------------------------//
	for(int i = 0; i < u_TotalDirLight; ++i)
		totalColor += GetLightBasic(u_DirLight_Color[i], u_DirLight_Dir[i], NobjNormal, VertToEye);
		
	//Point light----------------------------------------------------------//
	for(int i = 0; i < u_TotalPointLight; ++i)
		totalColor += CalPointLight(u_PointLight_Pos[i], normalize(u_PointLight_Pos[i] - vertexPos), NobjNormal, VertToEye);
	
	//Spot light----------------------------------------------------------//
	for(int i = 0; i < u_TotalSpotLight; ++i)	
		totalColor += CalSpotLight(u_SpotLight_Pos[i], u_SpotLight_Dir[i], normalize(u_SpotLight_Pos[i] - vertexPos), u_SpotLight_Cutoff[i], NobjNormal, VertToEye);
		
	if(u_TextureEnabled == true)
		color = texture2D( u_Texture, vertexTexcoord ) * vec4(totalColor, 1.0);
	else
		color = vec4(vertexColor * totalColor, 1.0f);
}