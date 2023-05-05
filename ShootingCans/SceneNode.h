#pragma once
#include "glm\glm.hpp"
#include "GeometryNode.h"
#include "GeometricMesh.h"
#include <vector>
#include "OBJLoader.h"
#include <map>

//Tree structure so that a SceneNode's transformation matrix will be relative to its 
//parent
class SceneNode {
public: 
	//ctr
	SceneNode(GeometryNode * mesh = NULL, GeometricMesh * col = NULL);
	//dtr
	 ~SceneNode();


	//set - get methods
	void setTransform(const glm::mat4 &matrix) {
		transform = matrix;
	}

	const glm::mat4& getTransform() const {
		return transform;
	}

	glm::mat4 getWorldTransform() const {
		return worldTransform;
	}

	void setModelScale(glm::vec3 scale) {
		modelScale = scale;
	}
	
	glm::vec3 getModelScale() const {
		return modelScale;
	}

	GeometryNode * getMesh() const {
		return mesh;
	}

	GeometryNode * getCol() const {
		return col;
	}

	std::vector<glm::vec3> getCollisionHull() {
		return collision_hull;
	}
	
	void setMesh(GeometryNode * m) {
		mesh = m;
	}

	void setCollisionHull(const std::vector<glm::vec3>& v) {
		collision_hull = v;
	}

	

	void addChild(SceneNode * node);
	void removeChild(SceneNode *node);
	

	virtual void update(float dt);
	//virtual glm::vec3 getDirection() { return glm::vec3(0, 0, 0); }
	//virtual glm::vec3 getPosition() { return glm::vec3(0, 0, 0); }

	void setLabel(std::string str) {
		label = str;
	}
	std::string getLabel() { return label; };

	std::vector<SceneNode*>::const_iterator  getChildIteratorStart() {
		return children.begin();
	}

	std::vector<SceneNode*>::const_iterator getChildrenIteratorEnd() {
		return children.end();
	}

	SceneNode * getParent() { return parent; }

	static GeometryNode * InitMesh(char* filename);
	static GeometryNode * InitColl(char* filaname,int);
	static GeometricMesh * LoadMesh(char* filaname);

protected:
	

	SceneNode * parent;
	GeometryNode* mesh;
	GeometryNode* col;
	GeometricMesh* col_mesh;


	glm::mat4 worldTransform; //wotransform coming from parent
	glm::mat4 transform; // local (relative) transform

	glm::vec3 modelScale; // modelScale kept separately in case we don't want children to scale as well

	glm::vec4 color;
	std::vector<SceneNode *> children;
	std::vector<glm::vec3> collision_hull; //Collision vertices


	std::string label;
	static const int EMPTY = 0;
	static const int CAN = 1;
	static const int WALL = 2;
	
};