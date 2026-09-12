#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

struct triangle {
	glm::mat3 points;
	glm::vec3 normal;
};
struct gpu_triangle {
	glm::vec4 p1;
	glm::vec4 p2;
	glm::vec4 p3;
	glm::vec4 normal;
	glm::uvec4 material_id;
};

struct brdf_material {
	glm::vec3 albedo;
	float roughness;
	float metallic;
};

class Mesh {
public:
	std::vector<triangle> triangles;
	brdf_material material;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	
	void load_from_obj(std::string path);
	glm::mat4 get_model();
};

void ssbo_from_mesh_set(std::vector<Mesh*>& scene);
