#pragma once

#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <engine/window.hpp>
#include <engine/signal.hpp>

class Engine {
private:
	double last; double current; double max_fps; double all_fps; unsigned int frame_cnt; float delta;

public:
	Window main_window;

	static inline Signal<double, double> global_mouse_event;

	Engine(unsigned int width, unsigned int height, std::string app_name);
	~Engine();

	Signal<> ready_signal;
	Signal<float> process_signal;
	Signal<> quit_signal;
	void run();

	void print_fps_info();
};
