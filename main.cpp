#include <iostream>

#include <glm/glm.hpp>

#include <engine/engine.hpp>
#include <engine/signal.hpp>
#include <engine/shader.hpp>

Engine engine(1280, 720, "sigma clouds");
Shader main_shader;
unsigned int VBO;
unsigned int VAO;

void on_ready() {
	glfwSetInputMode(engine.main_window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSize(engine.main_window.glfw_window, 1280, 720);

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
}
void process(float delta) {
	main_shader.execute();

	main_shader.set_uniform("u_time", (float)glfwGetTime());

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
