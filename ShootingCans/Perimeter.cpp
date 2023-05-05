#include "Perimeter.h"



GeometryNode * Perimeter::geo_wall2 = NULL;
//GeometryNode * Perimeter::geo_wall2_col = NULL;
GeometryNode * Perimeter::geo_corner = NULL;
GeometryNode * Perimeter::geo_gate = NULL;


GeometricMesh * Perimeter::col_mesh = NULL;

Perimeter::Perimeter() {

	SceneNode * wall;
	for (int i = 0; i < 6; i++) {
		wall = new SceneNode(geo_wall2, col_mesh);
		wall->setLabel("WAll");
		wall->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-25 + i * 10, 0, -30)));
		addChild(wall);

		wall = new SceneNode(geo_wall2, col_mesh);
		wall->setLabel("WAll");
		wall->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-25 + i * 10, 0, 30)));
		addChild(wall);

		wall = new SceneNode(geo_wall2, col_mesh);
		wall->setLabel("WAll");
		wall->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(30, 0, -25 + i * 10))*
			glm::rotate(glm::mat4(1.0), glm::pi<float>() / 2, glm::vec3(0, 1, 0)));
		addChild(wall);

		wall = new SceneNode(geo_wall2, col_mesh);
		wall->setLabel("WAll");
		wall->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-30, 0, -25 + i * 10))*
			glm::rotate(glm::mat4(1.0), glm::pi<float>() / 2, glm::vec3(0, 1, 0)));
		addChild(wall);
	}


	SceneNode* corner = new SceneNode(geo_corner);
	corner->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(30,0,30)));
	addChild(corner);
	corner = new SceneNode(geo_corner);
	corner->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-30,0, 30)));
	addChild(corner);
	corner = new SceneNode(geo_corner);
	corner->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(30,0, -30)));
	addChild(corner);
	corner = new SceneNode(geo_corner);
	corner->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-30,0, -30)));
	addChild(corner);

	SceneNode* gate = new SceneNode(geo_gate);
	gate->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0,29))*
		glm::scale(glm::mat4(1.0),glm::vec3(2.5,1.5,2)));
	addChild(gate);

}