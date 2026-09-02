#include <iostream>

#include <glm/glm.hpp>

#include <engine/engine.hpp>
#include <engine/signal.hpp>
#include <engine/shader.hpp>

Engine engine(1920, 1080, "make in babus");

void on_ready() {
	glfwSetInputMode(engine.main_window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void process(float delta) {
	engine.print_fps_info();
}
void on_quit() {
	std::cout << "quit\n";
}

int main() {
	engine.ready_signal.connect(on_ready);
	engine.process_signal.connect(process);
	engine.quit_signal.connect(on_quit);
	
	engine.run();
}
