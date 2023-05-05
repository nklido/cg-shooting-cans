#pragma once
#ifndef _ENVIROMENT
#define _ENVIROMENT


#include "glm\glm.hpp"
#include "SceneNode.h"
#include "OBJLoader.h"
#include "glm/gtc/matrix_transform.hpp"

class Enviroment : public SceneNode {

public:
	Enviroment();
	~Enviroment(void) {};

	//All geometry for the can will be loaded once

	static void CreateEnviroment() {
		
		geo_bus = InitMesh("bus.obj");
		geo_granary = InitMesh("Granary.obj");
		geo_barn = InitMesh("Barn.obj");
		geo_con_blue = InitMesh("ContainerBlue.obj");
		geo_con_red = InitMesh("ContainerRed.obj");
		geo_con_green = InitMesh("ContainerGreen.obj");
		geo_con_white = InitMesh("ContainerWhite.obj");
		geo_dirt = InitMesh("Dirt.obj");
		geo_rock = InitMesh("Rocks1.obj");
		geo_crater = InitMesh("Crater.obj");
		geo_oil_truck = InitMesh("OilTruck.obj");
		geo_hovel = InitMesh("Hovel.obj");
		geo_outpost = InitMesh("Outpost.obj");
		geo_derelict_house = InitMesh("DerelictHouse.obj");
		geo_barel1 = InitMesh("Barrel1.obj");
		geo_barel2 = InitMesh("Barrel2.obj");
		geo_barel3 = InitMesh("Barrel3.obj");
		geo_barel4 = InitMesh("Barrel4.obj");

		col_bus = LoadMesh("Bus-Collision.obj");
		col_granary = LoadMesh("Granary-Collision.obj");
		col_barn = LoadMesh("Barn-Collision.obj");
		col_container = LoadMesh("Container-Collision.obj");
		col_oil_truck = LoadMesh("OiLTruck-Collision.obj");
		col_hovel = LoadMesh("Hovel-Collision.obj");
		col_outpost = LoadMesh("Outpost-Collision.obj");
		col_barel = LoadMesh("Barrel4.obj");
		//col_derelict_house = LoadMesh("Derelict_house-Collision.obj");
	}

	static void DeleteEnviroment() {
		delete geo_bus;
		delete geo_granary;
		delete geo_barn;
		delete geo_con_red;
		delete geo_con_blue;
		delete geo_con_white;
		delete geo_con_green;
		delete geo_dirt;
		delete geo_rock;
		delete geo_crater;
		delete geo_outpost;
		delete geo_oil_truck;
		delete geo_hovel;
		delete geo_derelict_house;
		delete geo_barel1;
		delete geo_barel2;
		delete geo_barel3;
		delete geo_barel4;

		delete col_bus;
		delete col_granary;
		delete col_barn;
		delete col_container;
		delete col_oil_truck;
		delete col_hovel;
		delete col_outpost;
		delete col_derelict_house;
		delete col_barel;
	}

protected:
	static GeometryNode* geo_bus;
	static GeometryNode* geo_granary;
	static GeometryNode* geo_barn;
	static GeometryNode* geo_con_red;
	static GeometryNode* geo_con_green;
	static GeometryNode* geo_con_blue;
	static GeometryNode* geo_con_white;
	static GeometryNode* geo_dirt;
	static GeometryNode* geo_rock;
	static GeometryNode* geo_crater;
	static GeometryNode* geo_oil_truck;
	static GeometryNode* geo_outpost;
	static GeometryNode* geo_hovel;
	static GeometryNode* geo_derelict_house;
	static GeometryNode* geo_barel1;
	static GeometryNode* geo_barel2;
	static GeometryNode* geo_barel3;
	static GeometryNode* geo_barel4;



	static GeometricMesh * col_bus;
	static GeometricMesh * col_granary;
	static GeometricMesh* col_barn;
	static GeometricMesh* col_container;
	static GeometricMesh * col_oil_truck;
	static GeometricMesh* col_hovel;
	static GeometricMesh* col_outpost;
	static GeometricMesh* col_derelict_house;
	static GeometricMesh* col_barel;


};
#endif // 