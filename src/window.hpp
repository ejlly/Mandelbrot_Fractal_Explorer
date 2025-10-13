#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "shader_progs.hpp"
#include "bmp/bmp.hpp"

struct DragState {
    bool dragging = false, dirty = false;
    double startX = 0.0, startY = 0.0;
    double endX = 0.0, endY = 0.0;

    GLuint rectVAO, rectVBO;
};

struct Complex {
	long double x, y;

	inline Complex() {
		x = 0;
		y = 0;
	}

	inline Complex(long double x_coord, long double y_coord) {
		x = x_coord;
		y = y_coord;
	}

	inline Complex(Complex const& z) {
		x = z.x;
		y = z.y;
	}

	Complex operator+(Complex &z) {
		return Complex(x+z.x, y+z.y);
	}

	inline void evo(Complex a) {
		//Opé optimisée pour Mandelbrot
		long double const tmpx = x;
		x = x*x - y*y + a.x;
		y = 2*tmpx*y + a.y;
	}

	inline long double abs2() const {
		//module carré
		return x*x+y*y;
	}

	std::string to_string() const {
		return std::to_string(x) + " " + std::to_string(y);
	}
};

class Plot {
	public:
		BMP_Picture img;
		Complex bottom_left, top_right, origin;

		std::string date, img_title;
	
		Plot();
		Plot(int width, int height);
		Plot(int width, int height, Complex const& _bottom_left, Complex const& _top_right);
};

class Window{
	private:
		GLFWwindow *window;
		int width, height;

        GLuint VAO, VBO, EBO;
		int nb_its = 100;

        DrawingProgram shaderProgram, dragSelectionProgram;

        DragState drag;

        std::vector<Plot> memory;
	
	public:
		Window(int _width, int _height);
		Window();
		~Window();

		GLFWwindow* getaddr();
		void setWindow(GLFWwindow* _window);
		int getwidth();
		int getheight();
		int get_nb_its();

        bool add_plot(int sto_x, int sto_y, int mouse_x, int mouse_y);

        DragState& get_drag_state();

		void init(std::string const title);
    	void update_fractal(BMP_Picture const& img);
        void update_drag_box();

    	bool main_loop();
};