#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <engine/window.hpp>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 forward;

	glm::vec2 old_mouse_pos;

	Camera();

	glm::vec3 get_look_dir();

	void camera_controll(float delta, Window& window);
	void mouse_controll(glm::vec2 velocity);
};
