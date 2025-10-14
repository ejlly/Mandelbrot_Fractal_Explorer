#include "window.hpp"

int main(int argc, char* argv[]) {
	int width = 1500, height = 900;

	// Parse command-line arguments
	for (int i = 1; i < argc; ++i) {
		if ((std::string(argv[i]) == "-w" || std::string(argv[i]) == "--width") && i + 1 < argc) {
			width = std::stoi(argv[++i]);
		}
		if ((std::string(argv[i]) == "-h" || std::string(argv[i]) == "--height") && i + 1 < argc) {
			height = std::stoi(argv[++i]);
		}
	}

	Window glWindow(width, height);
	glWindow.init(std::string("Mandelbrot Explorer"));

	glWindow.main_loop();

	return 0;
}
