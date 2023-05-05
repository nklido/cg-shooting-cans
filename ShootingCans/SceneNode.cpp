#include "SceneNode.h"


SceneNode::SceneNode(GeometryNode * mesh, GeometricMesh * col_mesh) {
	this->mesh = mesh;
	this->col_mesh = col_mesh;
	if (col_mesh != nullptr) {
		this->col = new GeometryNode();
		this->col->Init(col_mesh);


		for (auto item : col_mesh->vertices)
			collision_hull.push_back(item);
	}

	parent = NULL;
	modelScale = glm::vec3(1, 1, 1);
}

SceneNode::~SceneNode() {
	delete col;
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::addChild(SceneNode * s) {
	children.push_back(s);
	s->parent = this;
}

void SceneNode::removeChild(SceneNode * s) {
	
	auto iter = std::find(children.begin(), children.end(), s);
	if (iter != children.end()) {
		children.erase(iter);
		//printf("deleted from root\n");
	}
	else {
		printf("not deleted\n");
	}
	
}

void SceneNode::update(float dt) {
	if (parent) {
		worldTransform = parent->worldTransform * transform; //"Pass" transformation from parent to this node
	}
	else {/*We are at root*/
		worldTransform = transform;
	}

	for (std::vector<SceneNode*>::iterator iter = children.begin(); iter != children.end(); ++iter) {
		(*iter)->update(dt);
	}

}

GeometryNode * SceneNode::InitMesh(char* filename) {
	char  filepath[100];

	strcpy(filepath, "../Data/Assets/");
	strcat(filepath, filename);
	OBJLoader loader;
	GeometryNode * node;

	auto mesh = loader.load(filepath);
	if (mesh != nullptr) {
		node = new GeometryNode();
		node->Init(mesh);
		return node;
	}
	else return nullptr;
}

GeometryNode * SceneNode::InitColl(char* filename,int index) {
	char  filepath[100];

	strcpy(filepath, "../Data/Assets/");
	strcat(filepath, filename);
	OBJLoader loader;
	GeometryNode * node;
	std::vector<glm::vec3> collision_hull;

	auto mesh = loader.load(filepath);
	if (mesh != nullptr) {
		node = new GeometryNode();
		node->Init(mesh);
		for (auto item : mesh->vertices)
			collision_hull.push_back(item);
		return node;
	}
	else return nullptr;
}

GeometricMesh * SceneNode::LoadMesh(char * filename) {
	char  filepath[100];

	strcpy(filepath, "../Data/Assets/");
	strcat(filepath, filename);
	OBJLoader loader;
	GeometryNode * node;
	std::vector<glm::vec3> collision_hull;

	auto mesh = loader.load(filepath);
	return mesh;
}