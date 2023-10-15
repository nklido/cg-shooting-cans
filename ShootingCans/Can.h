#pragma once
#ifndef _CAN
#define _CAN


#include "glm\glm.hpp"
#include "SceneNode.h"
#include "OBJLoader.h"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <stdlib.h>
#include "Renderer.h"

class Can : public SceneNode {

public:
	Can(std::string name, glm::vec2 pos = glm::vec2(0, 0));
	 ~Can() {};

	//All geometry for the can will be loaded once

	static void CreateCan() {
		geo_body = InitMesh("WalkerBody.obj");
		geo_cannon = InitMesh("WalkerCannon.obj");
		geo_flamer = InitMesh("WalkerFlamer.obj");
		geo_foot = InitMesh("WalkerFoot.obj");
		geo_lowleg = InitMesh("WalkerLowerLeg.obj");
		geo_upleg = InitMesh("WalkerUpperLeg.obj");

		//geo_body_col = InitColl("WalkerBody-Collision.obj",SceneNode::CAN);
		col_mesh = LoadMesh("WalkerBody-Collision.obj");
	}

	static void DeleteCan() {
		delete geo_body;
		delete geo_cannon;
		delete geo_flamer;
		delete geo_foot;
		delete geo_upleg;
		delete geo_lowleg;
		//delete geo_body_col;
		delete col_mesh;
	}

	//static void setTarget(glm::vec3 pos) {target= pos;}
	static glm::vec3 target; //player position

	virtual void update(float dt);
	virtual void walk(float dt);
	virtual void step_back(float dt);
	virtual void turn(float rad);

	virtual glm::vec3 getPosition() {return position;}
	virtual glm::vec3 getDirection() { return direction;}

	virtual void setDirection(glm::vec3 dir) { direction = dir; }
	virtual void setPosition(glm::vec3 pos) { position = pos; }

	

	std::string getName() { return name; }

protected :
	static GeometryNode* geo_body, *geo_cannon, *geo_flamer, *geo_foot, *geo_lowleg, *geo_upleg;
	//static GeometryNode* geo_body_col
	static GeometricMesh* col_mesh;
	float movement_speed;

	std::string name;


	glm::vec3 position; //position of the can in the plane
	glm::vec3 old_position; 
	glm::vec3 direction;
	
	
	
	float rotation_angle;

	int flag;
	SceneNode* body, *cannon, *flamer, *leftUpper,*rightUpper,*leftLower,*rightLower, *leftFoot, *rightFoot;
	SceneNode* body_col;
	int v1;

};

#endif // !_CAN