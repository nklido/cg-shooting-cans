#pragma once

#ifndef _BULLET
#define _BULLET

#include "glm\glm.hpp"
#include "glm/ext.hpp"
#include <iostream>

class Bullet{

public:
	Bullet(glm::vec3 position, glm::vec3 direction) {
		starting_position = position;
		current_position = position;
		this->direction = direction;
		active = true;
	}
	~Bullet() {}; //printf("Bullet_Death!!!!\n"); 

	bool isActive() { return active; }
	void setActive(bool active) { this->active = active; }
	glm::vec3 getDirection() { return direction; }
	glm::vec3 getPosition() { return current_position; }
	float getRadius() { return radius; };
	
	virtual void update(float dt) {
		current_position += glm::normalize(direction)*bullet_speed;

		distance_travelled = glm::length(starting_position - current_position);
		//std::cout << glm::to_string(current_position) << std::endl;

		if (distance_travelled >= range) {
			//std::cout << distance_travelled << std::endl;
			active = false;
		}
	}

protected:
	const float bullet_speed = 2.f;
	const float radius = 1;
	const float range = 50;

	glm::vec3 starting_position;
	glm::vec3 direction;
	glm::vec3 current_position;
	
	float distance_travelled;
	bool active;



};

#endif