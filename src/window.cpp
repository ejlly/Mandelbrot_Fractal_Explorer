#include <cassert>


#include "window.hpp"
#include "keys.hpp"
#include "mandel_calculation.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Window::Window(int _width, int _height){
	width = _width;
	height = _height;

    //glfwMakeContextCurrent(window);
}

Window::Window(){
	Window(1500, 900);
}

Window::~Window(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* Window::getaddr(){
	return window;
}

void Window::setWindow(GLFWwindow* _window){
	window = _window;
}

int Window::getwidth(){
	return width;
}

int Window::getheight(){
	return height;
}

int Window::get_nb_its(){
	return nb_its;
}

bool Window::add_plot(int sto_x, int sto_y, int mouse_x, int mouse_y) {
    //implements the zoom, returns true if success

    assert(memory.size() > 0);
    Plot& plot = memory.back();

	if(mouse_x == sto_x || mouse_y == sto_y) return false;

    int top_left_x = std::max(0, std::min(mouse_x, sto_x));
    int top_left_y = std::max(0, std::min(mouse_y, sto_y));
    int bottom_right_x = std::min(std::max(mouse_x, sto_x), width - 1);
    int bottom_right_y = std::min(std::max(mouse_y, sto_y), height - 1);

    if((bottom_right_x - top_left_x)*height >= (bottom_right_y - top_left_y)*width) {
        int const bary = (bottom_right_y + top_left_y)/2;
        double const ratio = static_cast<double>((bottom_right_x - top_left_x))/2 * height/width;
        top_left_y     = static_cast<int>(bary - ratio);
        bottom_right_y = static_cast<int>(bary + ratio);
    }
    else {
        int const bary = (bottom_right_x + top_left_x)/2;
        double const ratio = static_cast<double>((bottom_right_y - top_left_y))/2 * width/height;
        top_left_x     = static_cast<int>(bary - ratio);
        bottom_right_x = static_cast<int>(bary + ratio);
    }

    long double const abscisse(plot.top_right.x - plot.bottom_left.x),
                      ordonnee(plot.top_right.y - plot.bottom_left.y);

    Complex const new_bottom_left(plot.bottom_left.x + top_left_x * abscisse/width,
                                  plot.bottom_left.y + (height - bottom_right_y) * ordonnee/height);

    Complex const new_top_right(plot.top_right.x - (width - bottom_right_x) * abscisse/width,
                                plot.top_right.y - top_left_y * ordonnee/height);

    memory.push_back(Plot(width, height, new_bottom_left, new_top_right));

    calculate_frame(*this, memory.back(), false, false);

    return true;
}

DragState& Window::get_drag_state() {
    return drag;
}

void Window::init(std::string const title) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}
	glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(window, this);

	GLfloat vertices[] = {
		// Positions		Colors			  Texture Coords
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Top Right
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Bottom Right
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
		-1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f // Top Left
	};

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	shaderProgram.init("/home/nabboutc/Documents/misc_alg/cpp_train/mandel/src/shaders/vertexShader.vs",
                       "/home/nabboutc/Documents/misc_alg/cpp_train/mandel/src/shaders/fragmentShader.fs");


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    //ImGUI initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

	dragSelectionProgram.init("/home/nabboutc/Documents/misc_alg/cpp_train/mandel/src/shaders/color.vs",
                       "/home/nabboutc/Documents/misc_alg/cpp_train/mandel/src/shaders/color.fs");

    glGenVertexArrays(1, &drag.rectVAO);
    glGenBuffers(1, &drag.rectVBO);

    glBindVertexArray(drag.rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, drag.rectVBO);

    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Window::update_fractal(BMP_Picture const& img) {
	static GLuint texture = 0;
	if (texture == 0) {
		glGenTextures(1, &texture);
	}

    std::vector<unsigned char> rgb;
    rgb.reserve(width * height * 3);
    // Flip rows so that the first row in the buffer is the top of the image
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            Pixel const& p = img(i, j);
            rgb.push_back(p.get_r());
            rgb.push_back(p.get_g());
            rgb.push_back(p.get_b());
        }
    }

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data());

	glClearColor(1, 0, 0 ,1);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shaderProgram.uniformi("img_output", 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

    glUseProgram(0);
}

void Window::update_drag_box() {
    if (!drag.dragging) return;

    if (drag.dirty) {
        auto toGL = [&](double x, double y) {
            float nx = 2.0f * float(x) / width  - 1.0f;
            float ny = 1.0f - 2.0f * float(y) / height;
            return std::pair<float,float>(nx, ny);
        };
        auto [nx1, ny1] = toGL(drag.startX, drag.startY);
        auto [nx2, ny2] = toGL(drag.endX, drag.endY);
        float rectVerts[] = { nx1, ny1, nx2, ny1, nx2, ny2, nx1, ny2 };

        glBindBuffer(GL_ARRAY_BUFFER, drag.rectVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(rectVerts), rectVerts);
        
        drag.dirty = false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    dragSelectionProgram.use();
    dragSelectionProgram.uniformf("uColor", 0.2f, 0.4f, 1.0f, 0.9f);

    glBindVertexArray(drag.rectVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);
    glDisable(GL_BLEND);
}

bool Window::main_loop() {
    memory.push_back(Plot(width, height));
    calculate_frame(*this, memory.back(), false, false);
    update_fractal(memory.back().img);

    // Bind AntTweakBar variable to memory.nb_its
    //TwRemoveAllVars(TwGetBarByName("Settings"));

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Controls");
        static bool use_input = false;
        static bool input_just_activated = false;
        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsKeyPressed(ImGuiKey_P)) {
            use_input = true;
            input_just_activated = true;
        }
        if (use_input) {
            ImGui::SetNextItemWidth(300);
            if (input_just_activated) {
                ImGui::SetKeyboardFocusHere();
                input_just_activated = false;
            }
            bool changed = ImGui::InputInt("Precision", &nb_its, 1, 100);
            if (changed || !ImGui::IsItemActive()) {
                use_input = false;
            }
        } else {
            if (ImGui::IsItemActive()) {
                ImGui::SetNextItemWidth(300);
            }
            ImGui::SliderInt("Precision", &nb_its, 1, 25000);
        }
        ImGui::End();

        update_fractal(memory.back().img);
        update_drag_box();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return true;
}