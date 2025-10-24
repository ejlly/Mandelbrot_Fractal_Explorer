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

		bool isJulia = false;

		std::string date, img_title;
	
		Plot();
		Plot(int width, int height);
		Plot(int width, int height, Complex const& _origin);
		Plot(int width, int height, Complex const& _bottom_left, Complex const& _top_right);
		Plot(int width, int height, Complex const& _bottom_left, Complex const& _top_right, Complex const& _origin);
};

class Window{
	private:
		GLFWwindow *window;
		int width, height;

        GLuint VAO, VBO, EBO;
		int nb_its = 100;
		int memory_index = 0;

        DrawingProgram shaderProgram, dragSelectionProgram;

        DragState drag;

		std::vector<Plot> memory;

		// Save dialog state (used when user presses 'S')
		bool save_dialog_open = false;
		int save_target_width = 0;
		int save_target_height = 0;

	public:
		Window(int _width, int _height);
		Window();
		~Window();

		GLFWwindow* getaddr();
		void setWindow(GLFWwindow* _window);

		int getwidth();
		void setwidth(int w);
		int getheight();
		void setheight(int h);
		int get_nb_its();

		int get_memory_index();
		int get_memory_size();
		void set_memory_index(int index);

		Plot& get_last_plot();

        bool add_plot(int sto_x, int sto_y, int mouse_x, int mouse_y, bool recalculate=false);
		bool add_julia_plot(int mouse_x, int mouse_y);

		// Save dialog API
		bool is_save_dialog_open() const;
		void open_save_dialog();
		void close_save_dialog();
		void set_save_target_size(int w, int h);
		std::pair<int,int> get_save_target_size() const;
	void save_plot_at_size(const Plot &plot, int target_w, int target_h, const std::string &filepath, int precision);

        DragState& get_drag_state();

		void init(std::string const title);
    	void update_fractal(BMP_Picture const& img);
        void update_drag_box();

    	bool main_loop();
};