#include "Can.h"
#include "glm\glm.hpp"
#include "GLEW\glew.h"

GeometryNode * Can::geo_body = NULL;
GeometryNode * Can::geo_flamer = NULL;
GeometryNode * Can::geo_cannon = NULL;
GeometryNode * Can::geo_lowleg = NULL;
GeometryNode * Can::geo_upleg = NULL;
GeometryNode * Can::geo_foot = NULL;


glm::vec3 Can::target = glm::vec3(0,0,0);

//GeometryNode * Can::geo_body_col = NULL;
GeometricMesh * Can::col_mesh = NULL;


Can::Can(std::string name,glm::vec2 pos) {
	
	rotation_angle = 0;
	this->name = "CAN"+name;
	position = glm::vec3(pos.x,1.63,pos.y);
	flag = 0;


	direction = glm::normalize(glm::vec3(1, 0, 0));//glm::normalize(glm::vec3(1, 0, 1)*cos(v1*glm::pi<float>() / 180));
	movement_speed = 0.25f;//0.25;
	

	body = new SceneNode(geo_body,col_mesh);
	body->setLabel("CAN" + name);
	body->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(position.x,position.y,position.z))
		*glm::rotate(glm::mat4(1.0f),v1*glm::pi<float>()/180, glm::vec3(0, 1, 0)));
	addChild(body);

	flamer = new SceneNode(geo_flamer);
	flamer->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0.32, 0)));
	body->addChild(flamer);

	cannon = new SceneNode(geo_cannon);
	cannon->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0.32, 0)));
	body->addChild(cannon);

	leftUpper = new SceneNode(geo_upleg);
	leftUpper->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-0.64, -0.08, 0.015))* glm::rotate(glm::mat4(1.0), -glm::pi<float>() / 4, glm::vec3(1, 0, 0)));
	body->addChild(leftUpper);

	rightUpper = new SceneNode(geo_upleg);
	rightUpper->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.64, -0.08, 0.015)));
	body->addChild(rightUpper);

	leftLower = new SceneNode(geo_lowleg);
	leftLower->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, -0.64, 0.64))*glm::rotate(glm::mat4(1.0), 2 * glm::pi<float>() / 4, glm::vec3(1, 0, 0)));
	leftUpper->addChild(leftLower);

	rightLower = new SceneNode(geo_lowleg);
	rightLower->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, -0.64, 0.64)));
	rightUpper->addChild(rightLower);

	leftFoot = new SceneNode(geo_foot);
	leftFoot->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, -0.66, -0.65))* glm::rotate(glm::mat4(1.0), -glm::pi<float>() / 4, glm::vec3(1, 0, 0)));
	leftLower->addChild(leftFoot);

	rightFoot = new SceneNode(geo_foot);
	rightFoot->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, -0.66, -0.65)));
	rightLower->addChild(rightFoot);
}

void Can::walk() {
	
	old_position = position;
	position += direction*movement_speed;
	glm::vec3 old_dir = direction;
	direction = glm::normalize(Can::target - position);
}

void Can::step_back() {
	position = old_position;
	position -= direction*0.2 ;
}

void Can::turn(float angle) {
	direction = -glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
	rotation_angle = glm::pi<float>()/2;// -rotation_angle;// + angle

	//direction.x -= 0.5;
	//direction.z -= 0.5;

	//direction.x = 
	//direction.x =(1/cos(angle));
	//direction.z = 1- (1 / cos(angle));
	//std::cout << "direction = " << glm::to_string(direction) << std::endl;
}


void Can::update(float dt) {
	//rotation_angle += glm::dot(glm::normalize(old_dir), glm::normalize(direction));

	body->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(position.x, 1.63, position.z))*
		glm::rotate(glm::mat4(1.0), rotation_angle, glm::vec3(0, 1, 0))
	);

	float theta = glm::pi<float>() / 48;
	float theta2 = glm::pi<float>() /48 ;
	if (flag < 12) {
		rightUpper->setTransform(rightUpper->getTransform() * glm::rotate(glm::mat4(1.0), -theta, glm::vec3(1, 0, 0)));
		rightLower->setTransform(rightLower->getTransform() * glm::rotate(glm::mat4(1.0), 2 * theta, glm::vec3(1, 0, 0)));
		rightFoot->setTransform(rightFoot->getTransform() * glm::rotate(glm::mat4(1.0), -theta, glm::vec3(1, 0, 0)));

		leftUpper->setTransform(leftUpper->getTransform() * glm::rotate(glm::mat4(1.0), theta2, glm::vec3(1, 0, 0)));
		leftLower->setTransform(leftLower->getTransform() * glm::rotate(glm::mat4(1.0), -2 * theta2, glm::vec3(1, 0, 0)));
		leftFoot->setTransform(leftFoot->getTransform() * glm::rotate(glm::mat4(1.0), theta2, glm::vec3(1, 0, 0)));
		flag++;
		if (flag == 12) {
			flag = 24;
		}
	}
	else {
		rightUpper->setTransform(rightUpper->getTransform() * glm::rotate(glm::mat4(1.0), theta, glm::vec3(1, 0, 0)));
		rightLower->setTransform(rightLower->getTransform() * glm::rotate(glm::mat4(1.0), -2 * theta, glm::vec3(1, 0, 0)));
		rightFoot->setTransform(rightFoot->getTransform() * glm::rotate(glm::mat4(1.0), theta, glm::vec3(1, 0, 0)));

		leftUpper->setTransform(leftUpper->getTransform() * glm::rotate(glm::mat4(1.0), -theta2, glm::vec3(1, 0, 0)));
		leftLower->setTransform(leftLower->getTransform() * glm::rotate(glm::mat4(1.0), 2 * theta2, glm::vec3(1, 0, 0)));
		leftFoot->setTransform(leftFoot->getTransform() * glm::rotate(glm::mat4(1.0), -theta2, glm::vec3(1, 0, 0)));
		flag = flag--;
		if (flag == 12) {
			flag = 0;
		}
	}
	SceneNode::update(dt);
}





