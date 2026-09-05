#include <engine/camera.hpp>

Camera::Camera() {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);

	forward = glm::vec3(0, 0, -1);
	
	old_mouse_pos = glm::vec2(0.0f, 0.0f);
}

glm::vec3 Camera::get_look_dir() {
	return glm::vec3(glm::vec4(forward, 1.0f) * glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z));
}

void Camera::mouse_controll(glm::vec2 mouse_pos) {
	rotation.x += (old_mouse_pos.y - mouse_pos.y) * -0.003f;
	rotation.y += (old_mouse_pos.x - mouse_pos.x) * -0.003f;
	rotation.x = glm::clamp(rotation.x, glm::radians(-89.0f), glm::radians(89.0f));

	old_mouse_pos = mouse_pos;
}
void Camera::camera_controll(float delta, Window& window) {
	delta *= 3.0f;

	glm::mat4 rot_mat = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	glm::vec3 fwd = glm::vec3(glm::vec4(forward, 1.0f) * rot_mat);

	if (window.is_action_pressed(GLFW_KEY_W))
        position += fwd * delta;
	if (window.is_action_pressed(GLFW_KEY_S))
        position -= fwd * delta;
	if (window.is_action_pressed(GLFW_KEY_A))
        position -= glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0))) * delta;
	if (window.is_action_pressed(GLFW_KEY_D))
        position += glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0))) * delta;
	if (window.is_action_pressed(GLFW_KEY_SPACE))
        position += glm::vec3(0.0f, 1.0f, 0.0f) * delta;
	if (window.is_action_pressed(GLFW_KEY_LEFT_SHIFT))
        position += glm::vec3(0.0f, -1.0f, 0.0f) * delta;
}
