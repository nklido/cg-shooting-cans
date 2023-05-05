#include "Enviroment.h"
#include "glm\glm.hpp"
#include "GLEW\glew.h"

GeometryNode * Enviroment::geo_bus = NULL;
GeometryNode * Enviroment::geo_granary = NULL;
GeometryNode * Enviroment::geo_barn = NULL;
GeometryNode * Enviroment::geo_con_blue = NULL;
GeometryNode * Enviroment::geo_con_red = NULL;
GeometryNode * Enviroment::geo_con_green = NULL;
GeometryNode * Enviroment::geo_con_white = NULL;
GeometryNode * Enviroment::geo_crater = NULL;
GeometryNode * Enviroment::geo_rock = NULL;
GeometryNode * Enviroment::geo_dirt = NULL;
GeometryNode * Enviroment::geo_hovel = NULL;
GeometryNode * Enviroment::geo_outpost = NULL;
GeometryNode * Enviroment::geo_oil_truck = NULL;
GeometryNode * Enviroment::geo_derelict_house = NULL;
GeometryNode * Enviroment::geo_barel1 = NULL;
GeometryNode * Enviroment::geo_barel2 = NULL;
GeometryNode * Enviroment::geo_barel3 = NULL;
GeometryNode * Enviroment::geo_barel4 = NULL;

GeometricMesh * Enviroment::col_bus = NULL;
GeometricMesh * Enviroment::col_granary = NULL;
GeometricMesh * Enviroment::col_barn = NULL;
GeometricMesh * Enviroment::col_container = NULL;
GeometricMesh * Enviroment::col_oil_truck = NULL;
GeometricMesh * Enviroment::col_hovel = NULL;
GeometricMesh * Enviroment::col_outpost = NULL;
GeometricMesh * Enviroment::col_derelict_house = NULL;
GeometricMesh * Enviroment::col_barel = NULL;


Enviroment::Enviroment() {

	//bus 1
	SceneNode* bus = new SceneNode(geo_bus, col_bus);
	bus->setLabel("BUS");
	bus->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-15, 0, -15))*
		glm::rotate(glm::mat4(1.0), -glm::pi<float>() / 3, glm::vec3(0, 1, 0)));
	addChild(bus);
	
	//bus 2
	
	bus = new SceneNode(geo_bus, col_bus);
	bus->setLabel("BUS");
	bus->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(15, 0, 15))*
		glm::rotate(glm::mat4(1.0), glm::pi<float>() / 3, glm::vec3(0, 1, 0)));
	addChild(bus);

	//Granary

	SceneNode * granary = new SceneNode(geo_granary, col_granary);
	granary->setLabel("GRANARY");
	granary->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(26.5, 0, 0))*
		glm::rotate(glm::mat4(1.0), glm::pi<float>() / 2, glm::vec3(0, 1, 0)));
	addChild(granary);


	//Barn

	SceneNode * barn = new SceneNode(geo_barn, col_barn);
	barn->setLabel("BARN");
	barn->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(-24, 0, 0))*
		glm::rotate(glm::mat4(1.0), glm::pi<float>() / 2, glm::vec3(0, 1, 0)));
	addChild(barn);

	
	SceneNode * container = new SceneNode(geo_con_red, col_container);
	container->setLabel("CONTAINER");
	container->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(14, 0, -14))*
		glm::rotate(glm::mat4(1.0), glm::pi<float>() / 3, glm::vec3(0, 1, 0)));
	addChild(container);

	SceneNode * top_container = new SceneNode(geo_con_white, col_container);
	top_container->setLabel("CONTAINER");
	top_container->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(3.5,0, 0)));
	container->addChild(top_container);


	top_container = new SceneNode(geo_con_green);
	top_container->setLabel("CONTAINER");
	top_container->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(1, 2.8, 0))*
		glm::rotate(glm::mat4(1.0), -glm::pi<float>() / 3, glm::vec3(0, 1, 0)));
	container->addChild(top_container);

	container = new SceneNode(geo_con_blue, col_container);
	container->setLabel("CONTAINER");
	container->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(-10, 0,12))*
		glm::rotate(glm::mat4(1.0), glm::pi<float>() / 6, glm::vec3(0, 1, 0)));
	addChild(container);




	SceneNode * dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(24, 0, -24)));
	addChild(dirt);
	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(24, 0, -18)));
	addChild(dirt);
	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(24, 0, -11)));
	addChild(dirt);
	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(24, 0, 11)));
	addChild(dirt);
	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(24, 0, 18)));
	addChild(dirt);
	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(24, 0, 24)));
	addChild(dirt);



	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(-25, 0,25)));
	addChild(dirt);

	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(-25, 0, -25)));
	addChild(dirt);
	dirt = new SceneNode(geo_dirt);
	dirt->setLabel("dirt?");
	dirt->setTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(-5,0,-5)));
	addChild(dirt);

	SceneNode * truck = new SceneNode(geo_oil_truck, col_oil_truck);
	truck->setLabel("TRUCK");
	truck->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(24,0,-18))*
		glm::rotate(glm::mat4(1.0), -glm::pi<float>()/ 2, glm::vec3(0, 1, 0)));
	addChild(truck);

	SceneNode *outpost = new SceneNode(geo_outpost, col_outpost);
	outpost->setLabel("OUTPOST");
	outpost->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-26, 0, -26)));
	addChild(outpost);
	outpost = new SceneNode(geo_outpost, col_outpost);
	outpost->setLabel("OUTPOST");
	outpost->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-26, 0, 26)));
	addChild(outpost);


	SceneNode* crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-4, 0,-6)));
	addChild(crater);
	crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(4, 0,3)));
	addChild(crater);
	crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(11, 0, -6)));
	addChild(crater);
	crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-11, 0, -15)));
	addChild(crater);



	crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(5, 0, 25)));
	addChild(crater);

	crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -15)));
	addChild(crater);

	crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-20, 0, -15)));
	addChild(crater);
	crater = new SceneNode(geo_crater);
	crater->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(13, 0, 26)));
	addChild(crater);



	SceneNode* rock = new SceneNode(geo_rock);
	rock->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(5, 0,6)));
	addChild(rock);
	rock = new SceneNode(geo_rock);
	rock->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-15, 0, 6)));
	addChild(rock);

	SceneNode* hovel = new SceneNode(geo_hovel, col_hovel);
	hovel->setLabel("HOVEL");
	hovel->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0,-27)));
	addChild(hovel);



	SceneNode* barrel4 = new SceneNode(geo_barel4);
	barrel4->setLabel("Barrel");
	barrel4->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(-0.5,0,0.5)));
	addChild(barrel4);

	barrel4 = new SceneNode(geo_barel4);
	barrel4->setLabel("Barrel");
	barrel4->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.5, 0, 0.5)));
	addChild(barrel4);

	barrel4 = new SceneNode(geo_barel4);
	barrel4->setLabel("Barrel");
	barrel4->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0,-0.5)));
	addChild(barrel4);

	SceneNode *barrel3 = new SceneNode(geo_barel2);
	barrel3->setLabel("Barrel");
	barrel3->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0,1,0)));
	addChild(barrel3);



}

