#pragma once

#ifndef _PLAYER
#define _PLAYER

#include "glm\glm.hpp"
#include "glm/ext.hpp"
#include <iostream>

class Player {

public:
	Player(glm::vec3 position, glm::vec3 direction) {
		starting_position = position;
		current_position = position;
		this->direction = direction;
	}
	~Player() {};

	bool isJumping() { return is_jumping; }

	void jump() {
		if(!is_jumping)
		this->is_jumping = true; 
	}
	glm::vec3 getDirection() { return direction; }
	glm::vec3 getPosition() { return current_position; }
	float getRadius() { return radius; };

	virtual void update(float dt) {

	}

protected:
	const float height = 1.90;
	const float radius = 1.f;

	glm::vec3 starting_position;
	glm::vec3 direction;
	glm::vec3 current_position;
	
	bool is_jumping;
};

#endif