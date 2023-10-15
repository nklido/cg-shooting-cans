#include <iostream>
#include "Renderer.h"
#include "GeometryNode.h"
#include "Tools.h"
#include <algorithm>
#include "ShaderProgram.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "OBJLoader.h"
//#define debuggin 


//RENDERER CONSTRUCTOR
Renderer::Renderer() : m_geometry_rendering_program("Geometry Rendering Program"),
m_postprocess_program("Post Processing Program"),
m_spot_light_shadow_map_program("SpotLight ShadowMap Program"){
	m_vbo_fbo_vertices = 0;
	m_vao_fbo = 0;

	//Geometry for debuggin
	m_vao = 0;
	m_vbo_fbo_vertices = 0;
	m_num_vertices = 0;
	
	//init geometrynodes


	m_fbo = 0;
	m_fbo_texture = 0;

	m_rendering_mode = RENDERING_MODE::TRIANGLES;
	m_continous_time = 0.0;


	m_camera_position = glm::vec3(25, 2.20, 25);
	m_camera_target_position = glm::vec3(0, 2.20, 0);
	m_camera_old_position = m_camera_position;
	m_camera_up_vector = glm::vec3(0, 1, 0);
	shots_fired = false;
	game_over = false;
	is_jumping = false;
	is_falling = false;
	max_jump = 1.f;
	fireTime = 0.0f;
	max_cans = 5;
	kills = 0;

	spawn_positions = { glm::vec2(4,4),
						glm::vec2(-6,7),
						glm::vec2(-20,20),
						glm::vec2(28,-28),
						glm::vec2(-28,0),
						glm::vec2(18,0)};


}

Renderer::~Renderer()
{
	// delete g_buffer
	glDeleteTextures(1, &m_fbo_texture);
	glDeleteFramebuffers(1, &m_fbo);

	// delete common data
	glDeleteVertexArrays(1, &m_vao_fbo);
	glDeleteBuffers(1, &m_vbo_fbo_vertices);

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo_vertices);

	//delete geometry

	delete rootNode;
	Can::DeleteCan();
	Perimeter::DeletePerimeter();
	
#ifndef debuggin
	Enviroment::DeleteEnviroment();
#endif // !debuggin

	
}

void Renderer::setMovementSpeed(float spd) {
	this->movement_speed = spd;
}

bool Renderer::Init(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	this->m_screen_width = SCREEN_WIDTH;
	this->m_screen_height = SCREEN_HEIGHT;

	// Initialize OpenGL functions

	//Set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//This enables depth and stencil testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	// glClearDepth sets the value the depth buffer is set at, when we clear it

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// open the viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //we set up our viewport

	bool techniques_initialization = InitRenderingTechniques();
	bool buffers_initialization = InitDeferredShaderBuffers();
	bool items_initialization = InitCommonItems();
	bool lights_sources_initialization = InitLightSources();
	bool meshes_initialization = InitGeometricMeshes();

	//If there was any errors
	if (Tools::CheckGLError() != GL_NO_ERROR)
	{
		printf("Exiting with error at Renderer::Init\n");
		return false;
	}
	//If everything initialized
	if (techniques_initialization && items_initialization && buffers_initialization)
	return techniques_initialization && items_initialization && buffers_initialization;
}

void Renderer::Update(float dt)
{


	Can::target = glm::vec3(m_camera_position.x, 0, m_camera_position.z);

	float movement_speed = 5.0f;

	glm::vec3 direction = glm::normalize(m_camera_target_position - m_camera_position);

	//dont allow movement on y axis when moving forward-backward
	glm::vec3 movement_direction = direction * glm::vec3(1, 0, 1);

	glm::vec3 walk_direction = glm::normalize(m_camera_position - m_camera_old_position);
	SceneNode*  coll = checkCollision(rootNode, 0.8f, m_camera_position, walk_direction, true);
	if (coll != nullptr) {

		std::string label = coll->getLabel().c_str();
		//printf("Collided with : %s\n", label);

		if (label.substr(0, 3) == "CAN") {
			printf("GAME OVER");
			game_over = true;
			return;
		}


		m_camera_position = m_camera_old_position - glm::normalize(walk_direction) * 0.1f;
		return;
	}
	m_camera_old_position = m_camera_position;

	m_camera_position += m_camera_movement.x * movement_speed * movement_direction * dt;
	m_camera_target_position += m_camera_movement.x * movement_speed * direction * dt;

	glm::vec3 right = glm::normalize(glm::cross(direction, m_camera_up_vector));
	m_camera_position += m_camera_movement.y * movement_speed * right * dt;
	m_camera_target_position += m_camera_movement.y * movement_speed * right * dt;


	glm::mat4 rotation = glm::mat4(1.0f);
	float angular_speed = glm::pi<float>() * 0.0025f;

	rotation *= glm::rotate(glm::mat4(1.0), m_camera_look_angle_destination.y * angular_speed, right);
	rotation *= glm::rotate(glm::mat4(1.0), -m_camera_look_angle_destination.x * angular_speed, m_camera_up_vector);
	m_camera_look_angle_destination = glm::vec2(0);

	direction = rotation * glm::vec4(direction, 0);
	float dist = glm::distance(m_camera_position, m_camera_target_position);
	m_camera_target_position = m_camera_position + direction * dist;

	m_view_matrix = glm::lookAt(m_camera_position, m_camera_target_position, m_camera_up_vector);

	m_continous_time += dt;


	m_spotlight_node.SetTarget(glm::vec3(m_camera_target_position.x, m_camera_target_position.y, m_camera_target_position.z));
	m_spotlight_node.SetPosition(m_camera_position - 0.3f * direction);


	//checking for Can collisions
	for (std::vector<Can*>::const_iterator iter = cans.begin(); iter != cans.end(); ++iter) {
		SceneNode* coll = checkCollision(rootNode, 1.f, (*iter)->getPosition(), (*iter)->getDirection(), (*iter)->getName().c_str());

		if (coll == nullptr) {
			(*iter)->walk(dt);
		}
		else {
			printf("Can: %s Collided with : %s\n", (*iter)->getLabel().c_str(), coll->getLabel().c_str());
			(*iter)->step_back(dt);
			(*iter)->setDirection(-(*iter)->getDirection());
			unsigned int seed = static_cast<unsigned int>(m_continous_time);
			srand(seed);
			int angle = rand();
			(*iter)->turn(glm::pi<float>() * angle / 180);
			(*iter)->walk(dt);
		}
	}

	//Fire gun
	if (shots_fired) {
		
		m_gun_transformation_matrix =
			glm::inverse(m_view_matrix)
			*glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 24, glm::vec3(1, 0, 0))
			*glm::translate(glm::mat4(1.0f), glm::vec3(0.065, -0.1, -0.15));


		if (fireTime == 0.0) {
			//printf("New bullet\n");
			bullets.push_back(new Bullet(m_camera_position, direction));
		}

		fireTime += dt;
		if (fireTime > 0.3) {
			shots_fired = false;
			fireTime = 0.0f;
		}
	}
	else {
		m_gun_transformation_matrix = glm::inverse(m_view_matrix)*glm::translate(glm::mat4(1.0f), glm::vec3(0.065, -0.1, -0.17));
		m_gun_transformation_normal_matrix = glm::transpose(glm::inverse(m_gun_transformation_matrix));
	}

	
	//jump
	if(is_jumping){
		float pos = m_camera_position.y;
		if (pos < 2.2 + max_jump && !is_falling) {
			m_camera_position.y+=0.3;

			
		}
		else {
			is_falling = true;
			m_camera_position.y -= 0.3;
		}

		if (pos <= 2.2) {
			m_camera_position.y = 2.2;
			is_jumping = false;
			is_falling = false;
		}
	}

	rootNode->update(dt);
	
	for (std::vector<Bullet*>::const_iterator iter = bullets.begin(); iter != bullets.end();) {

		if (!(*iter)->isActive()) {
			delete* iter;
			iter = bullets.erase(iter);
		}
		else {

			for (int i = 0; i < 30; i++) {

				(*iter)->update(dt);

				SceneNode* coll = checkCollision(rootNode, (*iter)->getRadius(), (*iter)->getPosition(), (*iter)->getDirection(), true);

				if (coll != nullptr) {

					printf("SHOT : %s\n", coll->getLabel().c_str());
					if (strstr(coll->getLabel().c_str(), "CAN")) {

						auto it = std::find(cans.begin(), cans.end(), coll->getParent());
						if (it != cans.end()) {
							cans.erase(it);
							printf("Num of cans now : %d\n", cans.size());
							rootNode->removeChild(coll->getParent());
							delete coll->getParent();
							kills++;
						}
						//delete coll;
						//printf("%s\n", typeid(coll->getParent()).name());
					}
					(*iter)->setActive(false);
					break;
				}
			}
			++iter;
		}
	}

	//printf("%d\n",(int) round(m_continous_time));
	if (m_continous_time > next_respawn_time) {
		if (cans.size() < max_cans) {
			auto rng = std::default_random_engine{};
			std::shuffle(std::begin(spawn_positions), std::end(spawn_positions), rng);
			Can * can = new Can(std::to_string(m_continous_time), spawn_positions.at(0));
			std::cout << "Respwning at : " << glm::to_string(spawn_positions.at(0)) << std::endl;

			rootNode->addChild(can);
			cans.push_back(can);

		}
		
		next_respawn_time = m_continous_time + 5;
	}

}


bool Renderer::InitCommonItems()
{
	//Generate a handle to a vertex array object (vao) --> stores
	//which attributes are enabled and the data format of each bounded vbo
	glGenVertexArrays(1, &m_vao_fbo);

	//Bind the vertex array object
	glBindVertexArray(m_vao_fbo);

	GLfloat fbo_vertices[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	//Generate a handle to a buffer that holds vertex data
	glGenBuffers(1, &m_vbo_fbo_vertices);

	//bind the vbo to access to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fbo_vertices);

	//Allocate buffer and fill it with data of size buffer_size in bytes
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);

	//describe to openGl how to interpret data.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //release

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);

	// Create a Cube
	glm::vec3 topLeftFrontVertex(-1.0, 1.0, 1.0);
	glm::vec3 topRightFrontVertex(1.0, 1.0, 1.0);
	glm::vec3 bottomLeftFrontVertex(-1.0, -1.0, 1.0);
	glm::vec3 bottomRightFrontVertex(1.0, -1.0, 1.0);

	glm::vec3 topLeftBackVertex(-1.0, 1.0, -1.0);
	glm::vec3 topRightBackVertex(1.0, 1.0, -1.0);
	glm::vec3 bottomLeftBackVertex(-1.0, -1.0, -1.0);
	glm::vec3 bottomRightBackVertex(1.0, -1.0, -1.0);

	std::vector<glm::vec3> vertices = {
		// front
		topLeftFrontVertex, bottomLeftFrontVertex, bottomRightFrontVertex,
		bottomRightFrontVertex, topRightFrontVertex, topLeftFrontVertex,
		// back
		bottomRightBackVertex, bottomLeftBackVertex, topLeftBackVertex,
		topLeftBackVertex, topRightBackVertex, bottomRightBackVertex,
		// top
		topLeftFrontVertex, topRightFrontVertex, topRightBackVertex,
		topRightBackVertex, topLeftBackVertex, topLeftFrontVertex,
		// bottom
		bottomRightBackVertex, bottomRightFrontVertex, bottomLeftFrontVertex,
		bottomLeftFrontVertex, bottomLeftBackVertex, bottomRightBackVertex,
		// left
		topLeftFrontVertex, topLeftBackVertex, bottomLeftBackVertex,
		bottomLeftBackVertex, bottomLeftFrontVertex, topLeftFrontVertex,
		// right
		topRightFrontVertex, bottomRightFrontVertex, bottomRightBackVertex,
		bottomRightBackVertex, topRightBackVertex, topRightFrontVertex

	};
	m_num_vertices = vertices.size();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0][0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// assign to each face a color
	GLuint m_vbo_colors;
	glGenBuffers(1, &m_vbo_colors);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colors);
	std::vector<glm::vec3> colors = {
		// front
		glm::vec3(1,0,0), glm::vec3(1,0,0), glm::vec3(1,0,0),
		glm::vec3(1,0,0), glm::vec3(1,0,0), glm::vec3(1,0,0),
		// back
		glm::vec3(1,0,0), glm::vec3(1,0,0), glm::vec3(1,0,0),
		glm::vec3(1,0,0), glm::vec3(1,0,0), glm::vec3(1,0,0),
		// top
		glm::vec3(0,1,0), glm::vec3(0,1,0), glm::vec3(0,1,0),
		glm::vec3(0,1,0), glm::vec3(0,1,0), glm::vec3(0,1,0),
		// bottom
		glm::vec3(0,1,0), glm::vec3(0,1,0), glm::vec3(0,1,0),
		glm::vec3(0,1,0), glm::vec3(0,1,0), glm::vec3(0,1,0),
		// left
		glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1),
		glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1),
		// right
		glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1),
		glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1)

	};
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0][0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);//



	GLuint m_vbo_fonts;
	glGenBuffers(1, &m_vbo_fonts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fonts);

	//glBufferData(GL_ARRAY_BUFFER,, &colors[0][0], GL_STATIC_DRAW);





	return true;
}
bool Renderer::InitRenderingTechniques()
{
	bool initialized = true;
	//init shaders
	//load uniforms
	std::string vertex_shader_path = "../Data/Shaders/basic_rendering.vert";
	std::string fragment_shader_path = "../Data/Shaders/basic_rendering.frag";
	m_geometry_rendering_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_geometry_rendering_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_geometry_rendering_program.CreateProgram();
	m_geometry_rendering_program.LoadUniform("uniform_projection_matrix");
	m_geometry_rendering_program.LoadUniform("uniform_view_matrix");
	m_geometry_rendering_program.LoadUniform("uniform_model_matrix");
	m_geometry_rendering_program.LoadUniform("uniform_normal_matrix");
	m_geometry_rendering_program.LoadUniform("uniform_diffuse");
	m_geometry_rendering_program.LoadUniform("uniform_specular");
	m_geometry_rendering_program.LoadUniform("uniform_shininess");
	m_geometry_rendering_program.LoadUniform("uniform_has_texture");
	m_geometry_rendering_program.LoadUniform("diffuse_texture");
	m_geometry_rendering_program.LoadUniform("uniform_camera_position");
	// Light Source Uniforms
	m_geometry_rendering_program.LoadUniform("uniform_light_projection_matrix");
	m_geometry_rendering_program.LoadUniform("uniform_light_view_matrix");
	m_geometry_rendering_program.LoadUniform("uniform_light_position");
	m_geometry_rendering_program.LoadUniform("uniform_light_direction");
	m_geometry_rendering_program.LoadUniform("uniform_light_color");
	m_geometry_rendering_program.LoadUniform("uniform_light_umbra");
	m_geometry_rendering_program.LoadUniform("uniform_light_penumbra");
	m_geometry_rendering_program.LoadUniform("uniform_cast_shadows");
	m_geometry_rendering_program.LoadUniform("shadowmap_texture");

	vertex_shader_path = "../Data/Shaders/postproc.vert";
	fragment_shader_path = "../Data/Shaders/postproc.frag";
	m_postprocess_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_postprocess_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_postprocess_program.CreateProgram();
	m_postprocess_program.LoadUniform("uniform_texture");
	m_postprocess_program.LoadUniform("uniform_time");

	vertex_shader_path = "../Data/Shaders/shadow_map_rendering.vert";
	fragment_shader_path = "../Data/Shaders/shadow_map_rendering.frag";
	m_spot_light_shadow_map_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_spot_light_shadow_map_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_spot_light_shadow_map_program.CreateProgram();
	m_spot_light_shadow_map_program.LoadUniform("uniform_projection_matrix");
	m_spot_light_shadow_map_program.LoadUniform("uniform_view_matrix");
	m_spot_light_shadow_map_program.LoadUniform("uniform_model_matrix");


	/*
	vertex_shader_path = "../Data/Shaders/fontVertex.vert";
	fragment_shader_path = "../Data/Shaders/fontFragment.frag";
	m_font_rendering_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_font_rendering_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_font_rendering_program.CreateProgram();
	m_font_rendering_program.LoadUniform("translation");
	m_font_rendering_program.LoadUniform("color");
	m_font_rendering_program.LoadUniform("font_atlas");
	*/

	return initialized;
}


bool Renderer::ReloadShaders()
{
	bool reloaded = true;
	// rendering techniques
	m_geometry_rendering_program.ReloadProgram();

	m_postprocess_program.ReloadProgram();

	m_spot_light_shadow_map_program.ReloadProgram();

	//m_font_rendering_program.ReloadProgram();

	return reloaded;
}


bool Renderer::InitDeferredShaderBuffers()
{
	// out texture	
	glGenTextures(1, &m_fbo_texture);
	// depth texture
	glGenTextures(1, &m_fbo_depth_texture);

	// framebuffer to link to everything together
	glGenFramebuffers(1, &m_fbo);

	return ResizeBuffers(m_screen_width, m_screen_height);
}

bool Renderer::ResizeBuffers(int width, int height)
{
	m_screen_width = width;
	m_screen_height = height;

	// texture
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// depth texture
	glBindTexture(GL_TEXTURE_2D, m_fbo_depth_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_screen_width, m_screen_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// framebuffer to link to everything together
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fbo_depth_texture, 0);

	GLenum status = Tools::CheckFramebufferStatus(m_fbo);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_projection_matrix = glm::perspective(glm::radians(60.f), width / (float)height, 0.1f, 1500.0f);
	m_view_matrix = glm::lookAt(m_camera_position, m_camera_target_position, m_camera_up_vector);

	return true;
}


bool Renderer::InitLightSources()
{
	m_spotlight_node.SetColor(25.0f * glm::vec3(0.79, 0.89, 1.0)); // (0.79, 0.89, 1.0)
	m_spotlight_node.SetConeSize(50,70); //(50, 70);
	m_spotlight_node.CastShadow(true);
	return true;
}

bool Renderer::InitGeometricMeshes()
{
	bool initialized = true;
	OBJLoader loader;
	m_base_geometry = InitMesh("ConcreteBase.obj");
	//gun
	m_gun_geometry = InitMesh("Gun01.obj");
	
#ifndef debuggin
	Enviroment::CreateEnviroment();
#endif // !debuggin

	
	Can::CreateCan();
	Perimeter::CreatePerimeter();
	rootNode = new SceneNode();

#ifndef debuggin
	rootNode->addChild(new Enviroment());
#endif // !debuggin

	
	rootNode->addChild(new Perimeter());
	
	return initialized;
}


GeometryNode * Renderer::InitMesh(char* filename) {
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

void Renderer::SetRenderingMode(RENDERING_MODE mode)
{
	m_rendering_mode = mode;
}

void Renderer::Render()
{
	
	RenderShadowMaps();

	// Draw the geometry
	RenderGeometry();

	// render to screen                    **********************************************
	RenderToOutFB();

	GLenum error = Tools::CheckGLError();
	if (error != GL_NO_ERROR)
	{
		printf("Reanderer:Draw GL Error\n");
		system("pause");
	}
}

void Renderer::RenderShadowMaps()
{
	// if the light source casts shadows
	if (m_spotlight_node.GetCastShadowsStatus())
	{
		int m_depth_texture_resolution = m_spotlight_node.GetShadowMapResolution();

		glBindFramebuffer(GL_FRAMEBUFFER, m_spotlight_node.GetShadowMapFBO());
		glViewport(0, 0, m_depth_texture_resolution, m_depth_texture_resolution);
		GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawbuffers);

		// clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_spot_light_shadow_map_program.Bind();
		glUniformMatrix4fv(m_spot_light_shadow_map_program["uniform_projection_matrix"], 1, GL_FALSE, glm::value_ptr(m_spotlight_node.GetProjectionMatrix()));
		glUniformMatrix4fv(m_spot_light_shadow_map_program["uniform_view_matrix"], 1, GL_FALSE, glm::value_ptr(m_spotlight_node.GetViewMatrix()));

	
		DrawShadow(m_base_geometry, m_base_transformation_matrix);
		DrawShadow(rootNode);

		glBindVertexArray(0);
		m_spot_light_shadow_map_program.Unbind();

		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


void Renderer::DrawShadow(SceneNode* node) {
	if (node->getMesh()) {
		glm::mat4 transformation = node->getWorldTransform()*glm::scale(glm::mat4(1.0), node->getModelScale());
		DrawShadow(node->getMesh(), transformation);

	}

	for (std::vector<SceneNode*>::const_iterator iter = node->getChildIteratorStart(); iter != node->getChildrenIteratorEnd(); ++iter) {
		DrawShadow(*iter);
	}
}

void Renderer::DrawShadow(GeometryNode * m_object_geometry, glm::mat4 m_object_transformation_matrix) {
	glBindVertexArray(m_object_geometry->m_vao);
	glUniformMatrix4fv(m_spot_light_shadow_map_program["uniform_model_matrix"], 1, GL_FALSE, glm::value_ptr(m_object_transformation_matrix));
	for (int j = 0; j < m_object_geometry->parts.size(); j++)
	{
		glDrawArrays(GL_TRIANGLES, m_object_geometry->parts[j].start_offset, m_object_geometry->parts[j].count);
	}
}

void Renderer::RenderGeometry()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, m_screen_width, m_screen_height);
	GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawbuffers);

	// clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	switch (m_rendering_mode)
	{
	case RENDERING_MODE::TRIANGLES:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case RENDERING_MODE::LINES:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case RENDERING_MODE::POINTS:
		glPointSize(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	};

	m_geometry_rendering_program.Bind();
	glUniformMatrix4fv(m_geometry_rendering_program["uniform_projection_matrix"], 1, GL_FALSE, glm::value_ptr(m_projection_matrix));
	glUniformMatrix4fv(m_geometry_rendering_program["uniform_view_matrix"], 1, GL_FALSE, glm::value_ptr(m_view_matrix));
	glUniform3f(m_geometry_rendering_program["uniform_camera_position"], m_camera_position.x, m_camera_position.y, m_camera_position.z);

	// pass the light source parameters
	glm::vec3 light_position = m_spotlight_node.GetPosition();
	glm::vec3 light_direction = m_spotlight_node.GetDirection();
	glm::vec3 light_color = m_spotlight_node.GetColor();
	glUniformMatrix4fv(m_geometry_rendering_program["uniform_light_projection_matrix"], 1, GL_FALSE, glm::value_ptr(m_spotlight_node.GetProjectionMatrix()));
	glUniformMatrix4fv(m_geometry_rendering_program["uniform_light_view_matrix"], 1, GL_FALSE, glm::value_ptr(m_spotlight_node.GetViewMatrix()));
	glUniform3f(m_geometry_rendering_program["uniform_light_position"], light_position.x, light_position.y, light_position.z);
	glUniform3f(m_geometry_rendering_program["uniform_light_direction"], light_direction.x, light_direction.y, light_direction.z);
	glUniform3f(m_geometry_rendering_program["uniform_light_color"], light_color.x, light_color.y, light_color.z);
	glUniform1f(m_geometry_rendering_program["uniform_light_umbra"], m_spotlight_node.GetUmbra());
	glUniform1f(m_geometry_rendering_program["uniform_light_penumbra"], m_spotlight_node.GetPenumbra());
	glUniform1i(m_geometry_rendering_program["uniform_cast_shadows"], (m_spotlight_node.GetCastShadowsStatus()) ? 1 : 0);
	glUniform1i(m_geometry_rendering_program["shadowmap_texture"], 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, (m_spotlight_node.GetCastShadowsStatus()) ? m_spotlight_node.GetShadowMapDepthTexture() : 0);

	// Enable Texture Unit 0
	glUniform1i(m_geometry_rendering_program["uniform_diffuse_texture"], 0);
	glActiveTexture(GL_TEXTURE0);

	//Draw base
	m_base_transformation_matrix = glm::scale(glm::mat4(1.0), glm::vec3(3,0,3));
	DrawObject(m_base_geometry, m_base_transformation_matrix, m_base_transformation_normal_matrix);
	DrawObject(m_gun_geometry, m_gun_transformation_matrix, m_gun_transformation_normal_matrix);
	DrawNode(rootNode);
	
	glBindVertexArray(0);
	glBindVertexArray(m_vao);
	//draw crosshair
	glm::mat4 cube_transformation =glm::inverse(m_view_matrix)
		*glm::translate(glm::mat4(1.0f), glm::vec3(0,0, -3))
		*glm::scale(glm::mat4(1.0f),glm::vec3(0.02,0.02,0.02));
	glUniformMatrix4fv(m_geometry_rendering_program["uniform_model_matrix"], 1, GL_FALSE, glm::value_ptr(cube_transformation));


	glDrawArrays(GL_TRIANGLES, 0, m_num_vertices);
	
	m_geometry_rendering_program.Unbind();

	glDisable(GL_DEPTH_TEST);
	if (m_rendering_mode != RENDERING_MODE::TRIANGLES)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPointSize(1.0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::DrawObject(GeometryNode * m_object_geometry, glm::mat4 m_object_transformation_matrix, glm::mat4 m_object_transformation_normal_matrix) {
	//draw given object
	glBindVertexArray(m_object_geometry->m_vao);
	glUniformMatrix4fv(m_geometry_rendering_program["uniform_model_matrix"], 1, GL_FALSE, glm::value_ptr(m_object_transformation_matrix));
	glUniformMatrix4fv(m_geometry_rendering_program["uniform_normal_matrix"], 1, GL_FALSE, glm::value_ptr(m_object_transformation_normal_matrix));
	for (int j = 0; j < m_object_geometry->parts.size(); j++) {

		glm::vec3 diffuseColor = m_object_geometry->parts[j].diffuseColor;
		glm::vec3 specularColor = m_object_geometry->parts[j].specularColor;
		float shininess = m_object_geometry->parts[j].shininess;

		glUniform3f(m_geometry_rendering_program["uniform_diffuse"], diffuseColor.r, diffuseColor.g, diffuseColor.b);
		glUniform3f(m_geometry_rendering_program["uniform_specular"], specularColor.r, specularColor.g, specularColor.b);
		glUniform1f(m_geometry_rendering_program["uniform_shininess"], shininess);
		glUniform1f(m_geometry_rendering_program["uniform_has_texture"], (m_object_geometry->parts[j].textureID > 0) ? 1.0f : 0.0f);
		glBindTexture(GL_TEXTURE_2D, m_object_geometry->parts[j].textureID);

		glDrawArrays(GL_TRIANGLES, m_object_geometry->parts[j].start_offset, m_object_geometry->parts[j].count);
	}

}

void Renderer::DrawNode(SceneNode * node) {
	if (node->getMesh()) {
		glm::mat4 transformation = node->getWorldTransform()*glm::scale(glm::mat4(1.0), node->getModelScale());
		glm::mat4 transformation_normal = glm::transpose(glm::inverse(transformation));
		DrawObject(node->getMesh(), transformation, transformation_normal);
		
		if (node->getCol()) {
			//DrawObject(node->getCol(), transformation, transformation_normal);
		}
	}

	for (std::vector<SceneNode*>::const_iterator iter = node->getChildIteratorStart(); iter != node->getChildrenIteratorEnd(); ++iter) {
		DrawNode(*iter);
	}
}

bool Renderer::checkCollision(SceneNode * node, float radius, const glm::vec3 & pos, const glm::vec3 & dir) {
	for (std::vector<SceneNode*>::const_iterator iter = node->getChildIteratorStart(); iter != node->getChildrenIteratorEnd(); ++iter) {
		if (checkCollision(*iter, radius, pos, dir)) {
			return true;
		}
	}
	
	if (!node->getCollisionHull().empty()) {
		if (collided(radius, node->getWorldTransform(), node->getCollisionHull(), pos, dir)) {
			return true;
		}
		else {
			return false;
		}

	}
	return false;
}


SceneNode* Renderer::checkCollision(SceneNode * node, float radius, const glm::vec3 & pos, const glm::vec3 & dir,bool flag) {
	for (std::vector<SceneNode*>::const_iterator iter = node->getChildIteratorStart(); iter != node->getChildrenIteratorEnd(); ++iter) {
		SceneNode* p = nullptr;
		p = checkCollision(*iter, radius, pos, dir, true);
		if (p) {
			return p;
		}
	}

	if (!node->getCollisionHull().empty()) {
		if (collided(radius, node->getWorldTransform(), node->getCollisionHull(), pos, dir)) {
			return node;
		}
		else {
			return nullptr;
		}
	}
	return nullptr;
}

SceneNode* Renderer::checkCollision(SceneNode * node, float radius, const glm::vec3 & pos, const glm::vec3 & dir, const char* except) {
	for (std::vector<SceneNode*>::const_iterator iter = node->getChildIteratorStart(); iter != node->getChildrenIteratorEnd(); ++iter) {
		SceneNode* p = nullptr;
		p = checkCollision(*iter, radius, pos, dir, true);
		if (p) {
			if (strcmp(except, p->getLabel().c_str()) != 0) {
				return p;
			}
		}
		else {
			return nullptr;
		}
	}
	
	if (!node->getCollisionHull().empty()) {
		if (collided(radius, node->getWorldTransform(), node->getCollisionHull(), pos, dir)) {
			return node;
		}
		else
			return nullptr;
	}
	return nullptr;
}


SceneNode* Renderer::checkCollision(float radius, const glm::vec3 & pos, const glm::vec3 & dir) {
	return checkCollision(rootNode, radius, pos, dir, true);
}




void Renderer::RenderToOutFB()
{
	// render to screen                    **********************************************
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_screen_width, m_screen_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	m_postprocess_program.Bind();

	glBindVertexArray(m_vao_fbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	glUniform1i(m_postprocess_program["uniform_texture"], 0);
	glUniform1f(m_postprocess_program["uniform_time"], m_continous_time);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);

	m_postprocess_program.Unbind();
}

void Renderer::CameraMoveForward(bool enable)
{
	m_camera_movement.x = (enable) ? 1 : 0;
}
void Renderer::CameraMoveBackWard(bool enable)
{
	m_camera_movement.x = (enable) ? -1 : 0;
}

void Renderer::CameraMoveLeft(bool enable)
{
	m_camera_movement.y = (enable) ? -1 : 0;
}
void Renderer::CameraMoveRight(bool enable)
{
	m_camera_movement.y = (enable) ? 1 : 0;
}

void Renderer::CameraLook(glm::vec2 lookDir)
{
	m_camera_look_angle_destination = glm::vec2(1, -1) * lookDir;
}

void Renderer::jump() {
	if (!is_jumping) {
		is_jumping = true;
	}
}
void Renderer::shotsFired() {
	shots_fired = true;
}
