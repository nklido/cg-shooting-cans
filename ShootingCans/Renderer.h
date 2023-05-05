#ifndef BIM_ENGINE_RENDERER_H
#define BIM_ENGINE_RENDERER_H


#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "ShaderProgram.h"
#include "SpotlightNode.h"
#include "isect.h"
#include <algorithm>
#include "Can.h"
#include "Perimeter.h"
#include "Bullet.h"
#include "Enviroment.h"
#include <time.h>
#include <random>


class Renderer
{
public:
	enum RENDERING_MODE
	{
		TRIANGLES,
		LINES,
		POINTS
	};
	float movement_speed;
	float sensitivity;
	

protected:
	int												m_screen_width, m_screen_height;
	glm::mat4										m_view_matrix;
	glm::mat4										m_projection_matrix;
	glm::vec3										m_camera_position;
	glm::vec3										m_camera_old_position;
	glm::vec3										m_camera_target_position;
	glm::vec3										m_camera_up_vector;
	glm::vec3										m_player_init_pos;
	glm::vec2										m_camera_movement;
	glm::vec2										m_camera_look_angle_destination;
	int max_cans;
	bool shots_fired;
	bool game_over;
	std::vector<Bullet*> bullets;
	std::vector<class Can*> cans;
	std::vector<glm::vec2> spawn_positions;
	//std::vector<glm::vec3> collision_hull;
	int kills;


	// Geometry Rendering Intermediate Buffer
	GLuint m_fbo;
	GLuint m_fbo_depth_texture;
	GLuint m_fbo_texture;
	GLuint m_vao_fbo, m_vbo_fbo_vertices;

	//Geometry debugging

	GLuint m_vao;
	GLuint m_vbo_vertices;
	GLuint m_num_vertices;


	float m_continous_time;
	float next_respawn_time;

	int m_number_of_vertices;

	// Rendering Mode
	RENDERING_MODE m_rendering_mode;

	// Lights
	SpotLightNode m_spotlight_node;

	class SceneNode * rootNode;

	// Meshes

	//base
	class GeometryNode* m_base_geometry;
	glm::mat4 m_base_transformation_matrix;
	glm::mat4 m_base_transformation_normal_matrix;
	//gun
	class GeometryNode* m_gun_geometry;
	glm::mat4 m_gun_transformation_matrix;
	glm::mat4 m_gun_transformation_normal_matrix;

	// Protected Functions
	bool InitRenderingTechniques();
	bool InitDeferredShaderBuffers();
	bool InitCommonItems();
	bool InitLightSources();
	bool InitGeometricMeshes();

	ShaderProgram								m_geometry_rendering_program;
	ShaderProgram								m_postprocess_program;
	ShaderProgram								m_spot_light_shadow_map_program;
	//ShaderProgram								m_font_rendering_program;

public:
	Renderer();
	~Renderer();
	bool										Init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void										Update(float dt);
	bool										ResizeBuffers(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	bool										ReloadShaders();
	void										Render();

	// Passes
	void										RenderShadowMaps();
	void										RenderGeometry();
	void										RenderToOutFB();
	
	// Set functions
	void										SetRenderingMode(RENDERING_MODE mode);

	// Camera Function
	void										CameraMoveForward(bool enable);
	void										CameraMoveBackWard(bool enable);
	void										CameraMoveLeft(bool enable);
	void										CameraMoveRight(bool enable);
	void										jump();
	void										CameraLook(glm::vec2 lookDir);	
	void setMovementSpeed(float spd);
	void shotsFired();
	bool is_jumping;
	bool is_falling;
	float max_jump;
	float fireTime;
	bool isGameOver() { return game_over; }

	int getNumOfKills() { return kills; }


	SceneNode* checkCollision(
		float radius,
		const glm::vec3 & pos,
		const glm::vec3 & dir);

private:
	void DrawNode(SceneNode *node);
	void DrawObject(GeometryNode * m_object_geometry,
					glm::mat4 m_object_transformation_matrix,
					glm::mat4 m_object_transformation_normal_matrix); //gets called in RenderGeometry for ever object 

	void DrawShadow(GeometryNode * m_object_geometry, glm::mat4 m_object_transformation_matrix);
	GeometryNode * InitMesh(char* filename);

	void DrawShadow(SceneNode* node);

	bool checkCollision(
		SceneNode * node,
		float radius,
		const glm::vec3 & pos,
		const glm::vec3 & dir);

	
	SceneNode* checkCollision(
		SceneNode * node,
		float radius,
		const glm::vec3 & pos,
		const glm::vec3 & dir,
		bool flag);


	SceneNode* checkCollision(
		SceneNode * node,
		float radius,
		const glm::vec3 & pos,
		const glm::vec3 & dir,
		const char* except_this);
	

	static bool collided(float radius,
		const glm::mat4 & collision_hull_transform,
		const std::vector<glm::vec3>& collision_hull,
		const glm::vec3 & pos,
		const glm::vec3 & dir) 
	{
		std::vector<glm::vec3> transformed_hull(collision_hull);
		std::transform(transformed_hull.begin(),
			transformed_hull.end(),
			transformed_hull.begin(),
			[&collision_hull_transform](glm::vec3 v) {return glm::vec3(collision_hull_transform*glm::vec4(v, 1.0f)); }
		);
		return intersect_triangles_collision_only(pos, dir, transformed_hull.data(),
			(unsigned int)transformed_hull.size()/3, radius);
	}
	
};

#endif
