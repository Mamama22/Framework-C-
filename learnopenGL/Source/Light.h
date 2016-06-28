#ifndef LIGHT_H
#define LIGHT_H
#include "Vertex.h"
#define TOTAL_DIR_LIGHT 2
#define TOTAL_POINT_LIGHT 2
#define TOTAL_SPOT_LIGHT 3

/*************************************************************
Light manager, matches uniforms in shader.
No OOP used as we want to pass all the light uniforms as an array instead of 1 pass for each light

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/

/*************************************************************
Light manager
/*************************************************************/
class LightManager
{
public:

	int total_dir;
	int total_point;
	int total_spot;

	//Dir light---------------------------//
	Vector3 dir_direction[TOTAL_DIR_LIGHT];
	Color dir_color[TOTAL_DIR_LIGHT];

	//Point light---------------------------//
	Vector3 point_pos[TOTAL_POINT_LIGHT];

	//Spot light---------------------------//
	Vector3 spot_direction[TOTAL_SPOT_LIGHT];
	Vector3 spot_pos[TOTAL_SPOT_LIGHT];
	float spot_cutoff[TOTAL_SPOT_LIGHT];	//cos(angle)


	LightManager(){ total_dir = total_point = total_spot = 0; }
	~LightManager(){}

	/*************************************************************
	Add dir light
	/*************************************************************/
	void Add_DirLight(Vector3 dir, Color color)
	{
		dir_direction[total_dir] = dir;
		dir_color[total_dir] = color;
		++total_dir;
	}

	/*************************************************************
	Add point light
	/*************************************************************/
	void Add_PointLight(Vector3 pos)
	{
		point_pos[total_point] = pos;
		++total_point;
	}

	/*************************************************************
	Add spot light
	/*************************************************************/
	void Add_SpotLight(Vector3 pos, Vector3 target, float cutoff_Angle)
	{
		Vector3 dir = (target - pos).Normalized();
		spot_direction[total_spot] = dir;
		spot_pos[total_spot] = pos;
		spot_cutoff[total_spot] = cos(Math::DegreeToRadian(cutoff_Angle));
		++total_spot;
	}
};


#endif