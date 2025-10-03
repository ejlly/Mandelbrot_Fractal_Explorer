#include "window.hpp"

int main(int argc, char* argv[]) {
	int const width = 1500, height = 900;
	Window glWindow(width, height);
	glWindow.init(std::string("Mandelbrot Explorer"));

	glWindow.main_loop();

	return 0;
}
