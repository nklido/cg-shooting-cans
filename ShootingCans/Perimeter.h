#pragma once
#ifndef _PERIMETER
#define _PERIMETER


#include "glm\glm.hpp"
#include "SceneNode.h"
#include "OBJLoader.h"
#include "glm/gtc/matrix_transform.hpp"

class Perimeter : public SceneNode {

public:
	Perimeter();
	~Perimeter(void) {};

	//All geometry for the can will be loaded once

	static void CreatePerimeter() {
		geo_wall2 = InitMesh("Wallsegment2.obj");
		geo_corner = InitMesh("WallCorner.obj");
		geo_gate = InitMesh("WallGate.obj");
		
		col_mesh = LoadMesh("Wallsegment2-Collision.obj");
	}

	static void DeletePerimeter() {
		delete geo_wall2;
		//delete geo_wall2_col;
		delete col_mesh;
	}
	//const std::string LABEL = "WALL";
	//virtual std::string getLabel() { return "WALL"; };

protected:
	static GeometryNode* geo_wall2;
	static GeometryNode* geo_gate;
	static GeometryNode* geo_corner;
	//static GeometryNode* geo_wall2_col;
	static GeometricMesh * col_mesh;
};

#endif // 