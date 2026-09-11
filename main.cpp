#include <iostream>

#include <glm/glm.hpp>

#include <engine/engine.hpp>
#include <engine/signal.hpp>
#include <engine/shader.hpp>
#include <engine/camera.hpp>
#include <engine/mesh.hpp>

Engine engine(1280, 720, "sigma clouds");
Camera main_camera;
Shader main_shader;
unsigned int VBO;
unsigned int VAO;

void on_ready() {
	glfwSetInputMode(engine.main_window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSize(engine.main_window.glfw_window, engine.main_window.width, engine.main_window.height);

	engine.global_mouse_event.connect([](double x, double y) {main_camera.mouse_controll(glm::vec2(x, y));} );

	main_shader.init_shader_program();
	main_shader.attach_shader("FRAGMENT", "shaders/f_tracer.glsl");
	main_shader.attach_shader("VERTEX", "shaders/v_tracer.glsl");
	main_shader.create_shader_program();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	float screen_rect[18] ={-1.0f, -1.0f, 0.0f,
							1.0f, -1.0f, 0.0f,
							-1.0f, 1.0f, 0.0f,

							1.0f, -1.0f, 0.0f,
							-1.0f, 1.0f, 0.0f,
							1.0f, 1.0f, 0.0f};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), screen_rect, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	main_shader.execute();
	main_shader.set_uniform("resolution", glm::vec2(engine.main_window.width, engine.main_window.height));
	main_shader.set_uniform("near", 0.1f);
	main_shader.set_uniform("far", 100.0f);
	main_shader.set_uniform("fov", 90.0f);

	Mesh test;
	brdf_material mat = (brdf_material){glm::vec3(0.0f, 0.2f, 0.5f), 0.5f, 0.5f};
	test.material = mat;
	test.load_from_obj("./test/obj/sphere.obj");
	
	std::vector<Mesh*> set;
	set.push_back(&test);
	
	ssbo_from_mesh_set(set);
}
void process(float delta) {
	main_shader.set_uniform("time", (float)glfwGetTime());
	main_camera.camera_controll(delta, engine.main_window);

	main_shader.set_uniform("camera_pos", main_camera.position);
	main_shader.set_uniform("camera_look_dir", main_camera.get_look_dir());

	glBindVertexArray(VAO);	
	glDrawArrays(GL_TRIANGLES, 0, 18);

	engine.print_fps_info();
}
void on_quit() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

int main() {
	engine.ready_signal.connect(on_ready);
	engine.process_signal.connect(process);
	engine.quit_signal.connect(on_quit);
	
	engine.run();
}
