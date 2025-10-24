#include <cassert>
#include <ctime>


#include "window.hpp"
#include "keys.hpp"
#include "mandel_calculation.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Plot::Plot() {
	//Generate date
	time_t const cur_time = std::time(NULL);
	date = std::asctime(std::localtime(&cur_time)); 
	//date[19] = ',';
	date.erase(0, 4);

    if(date.find('\n') != std::string::npos)
        date.erase(date.find('\n'), 1);

    std::cout << date << std::endl;
}

Plot::Plot(int width, int height) : Plot() {

	if (width >= height) {
		bottom_left = Complex(-2.*width/height, -2);
		top_right = Complex(2.*width/height, 2);
	}
	else {
		bottom_left = Complex(-2, -2.*height/width);
		top_right = Complex(2, 2.*height/width);
	}

	//bottom_left = Complex(0, 0);
	//top_right = Complex(1, 1.5);

	img = BMP_Picture(width, height);
	img_title = date + bottom_left.to_string() + "_" + top_right.to_string() + ".bmp";
}

Plot::Plot(int width, int height, Complex const& _origin) : Plot(width, height) {
    isJulia = true;
    origin = _origin;
}

Plot::Plot(int width, int height, Complex const& _bottom_left, Complex const& _top_right) : Plot() {
	bottom_left = _bottom_left;
	top_right = _top_right;

	img = BMP_Picture(width, height);
	img_title = date + bottom_left.to_string() + "_" + top_right.to_string() + ".bmp";
}

Plot::Plot(int width, int height, Complex const& _bottom_left, Complex const& _top_right, Complex const& _origin) :
    Plot(width, height, _bottom_left, _top_right) {
        isJulia = true;
        origin = _origin;
}

Window::Window(int _width, int _height){
	width = _width;
	height = _height;
}

// Save dialog helpers
bool Window::is_save_dialog_open() const {
    return save_dialog_open;
}

void Window::open_save_dialog() {
    save_dialog_open = true;
    save_target_width = width;
    save_target_height = height;
}

void Window::close_save_dialog() {
    save_dialog_open = false;
}

void Window::set_save_target_size(int w, int h) {
    save_target_width = w;
    save_target_height = h;
}

std::pair<int,int> Window::get_save_target_size() const {
    return {save_target_width, save_target_height};
}

// Save plot at a different resolution without changing current view
void Window::save_plot_at_size(const Plot &plot, int target_w, int target_h, const std::string &filepath, int precision) {
    // Create a copy of the plot with different image size
    Plot tmp;


    // Adjust bounds to target aspect ratio while keeping the same center
    long double const cur_w = plot.top_right.x - plot.bottom_left.x;
    long double const cur_h = plot.top_right.y - plot.bottom_left.y;
    long double const target_aspect = static_cast<long double>(target_w) / static_cast<long double>(target_h);
    long double const cur_aspect = cur_w / cur_h;

    if (cur_aspect < target_aspect) {
        // current is wider than target -> expand height
        long double const new_h = cur_w / target_aspect;
        long double const center_y = (plot.top_right.y + plot.bottom_left.y) / 2.0L;
        tmp.bottom_left.y = center_y - new_h / 2.0L;
        tmp.top_right.y = center_y + new_h / 2.0L;
        tmp.bottom_left.x = plot.bottom_left.x;
        tmp.top_right.x = plot.top_right.x;
    } else {
        // current is taller (or equal) than target -> expand width
        long double const new_w = cur_h * target_aspect;
        long double const center_x = (plot.top_right.x + plot.bottom_left.x) / 2.0L;
        tmp.bottom_left.x = center_x - new_w / 2.0L;
        tmp.top_right.x = center_x + new_w / 2.0L;
        tmp.bottom_left.y = plot.bottom_left.y;
        tmp.top_right.y = plot.top_right.y;
    }
    tmp.isJulia = plot.isJulia;
    tmp.origin = plot.origin;
    tmp.img = BMP_Picture(target_w, target_h);

    // Temporarily create a fake Window for calculation with the target size
    int old_width = width;
    int old_height = height;
    int old_nb_its = nb_its;
    width = target_w;
    height = target_h;
    nb_its = precision;

    calculate_frame(*this, tmp, true);

    // Restore window size
    width = old_width;
    height = old_height;
    nb_its = old_nb_its;

    tmp.img.save_BMP(filepath);
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

void Window::setwidth(int w) {
    width = w;
}

int Window::getheight(){
	return height;
}

void Window::setheight(int h) {
    height = h;
}

int Window::get_nb_its(){
	return nb_its;
}

int Window::get_memory_index() {
    return memory_index;
}
int Window::get_memory_size() {
    return static_cast<int>(memory.size());
}
void Window::set_memory_index(int index) {
    assert(index >= 0 && index < static_cast<int>(memory.size()));
    memory_index = index;
}

Plot& Window::get_last_plot() {
    assert(!memory.empty());
    return memory.back();
}

bool Window::add_plot(int sto_x, int sto_y, int mouse_x, int mouse_y, bool recalculate) {
    //implements the zoom, returns true if success

    assert(memory.size() > 0);
    Plot& plot = memory[memory_index];

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


    if (memory_index + 1 < get_memory_size()) {
        memory.erase(memory.begin() + memory_index + 1, memory.end());
        memory_index = static_cast<int>(memory.size()) - 1;
    }
    memory_index++;
    if(!plot.isJulia)
        memory.push_back(Plot(width, height, new_bottom_left, new_top_right));
    else
        memory.push_back(Plot(width, height, new_bottom_left, new_top_right, plot.origin));
    calculate_frame(*this, memory[memory_index], recalculate);

    return true;
}

bool Window::add_julia_plot(int mouse_x, int mouse_y) {
    //implements the zoom, returns true if success

    assert(memory.size() > 0);
    Plot& plot = memory[memory_index];

    long double const abscisse(plot.top_right.x - plot.bottom_left.x),
                      ordonnee(plot.top_right.y - plot.bottom_left.y);

    Complex const new_origin(plot.bottom_left.x + mouse_x * abscisse/width,
                             plot.bottom_left.y + (height - mouse_y) * ordonnee/height);
    

    if (memory_index + 1 < get_memory_size()) {
        memory.erase(memory.begin() + memory_index + 1, memory.end());
        memory_index = static_cast<int>(memory.size()) - 1;
    }
    memory_index++;

    memory.push_back(Plot(width, height, new_origin));
    calculate_frame(*this, memory[memory_index], false);

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
    // Enable VSync to reduce CPU usage
    glfwSwapInterval(1);
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
    calculate_frame(*this, memory[memory_index], false);
    update_fractal(memory[memory_index].img);

    // Bind AntTweakBar variable to memory.nb_its
    //TwRemoveAllVars(TwGetBarByName("Settings"));

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);


    auto draw_imgui_ui = [&]() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Controls");
        static bool use_input_precision = false;
        static bool input_just_activated_precision = false;

        static bool waiting_for_julia_origin = false;
        ImGuiIO& io = ImGui::GetIO();
        if(ImGui::IsKeyPressed(ImGuiKey_P)) {
            use_input_precision = true;
            input_just_activated_precision = true;
        }
        if(ImGui::IsKeyPressed(ImGuiKey_J)) {
            waiting_for_julia_origin = true;
        }

        if(waiting_for_julia_origin) {
            ImGui::Text("Click on the image to select the Julia set origin");
            if (ImGui::IsMouseClicked(0)) {
                double mouse_x, mouse_y;
                glfwGetCursorPos(window, &mouse_x, &mouse_y);
                waiting_for_julia_origin = false;
                add_julia_plot(static_cast<int>(mouse_x), static_cast<int>(mouse_y));
            }
        } else {
            ImGui::Text("Press 'J' to select Julia set origin");
        }

        if(use_input_precision) {
            ImGui::SetNextItemWidth(320);
            if(input_just_activated_precision) {
                ImGui::SetKeyboardFocusHere();
                input_just_activated_precision = false;
            }
            bool changed = ImGui::InputInt("Precision", &nb_its, 1, 100);
            if(changed || !ImGui::IsItemActive()) {
                use_input_precision = false;
            }
        }
        else {
            if(ImGui::IsItemActive()) {
                ImGui::SetNextItemWidth(320);
            }
            ImGui::SliderInt("Precision", &nb_its, 1, 25000);
        }
        ImGui::End();

        // Save dialog modal
        if (is_save_dialog_open()) {
            ImGui::OpenPopup("Save Image");
        }

        if (ImGui::BeginPopupModal("Save Image", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            Plot &plot = memory[memory_index];
            ImGui::Text("Save current plot");
            ImGui::Separator();

            static int tmp_w = 0;
            static int tmp_h = 0;
            static int tmp_precision = 0;
            if (tmp_w == 0 || tmp_h == 0) {
                auto sz = get_save_target_size();
                tmp_w = sz.first;
                tmp_h = sz.second;
                tmp_precision = nb_its;
            }

            ImGui::Text("Choose an option:");
            if (ImGui::Button("Save as is")) {
                std::string const filepath = "pics/" + plot.img_title;
                plot.img.save_BMP(filepath);
                printf("Saved picture as %s\n", filepath.c_str());
                tmp_w = tmp_h = 0;
                close_save_dialog();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Change resolution")) {
                // show inputs
                ImGui::InputInt("Width", &tmp_w);
                ImGui::InputInt("Height", &tmp_h);
            }

            ImGui::Text("\nOr enter width/height and press Confirm:");
            ImGui::InputInt("Target Width", &tmp_w);
            ImGui::InputInt("Target Height", &tmp_h);
            ImGui::Separator();
            ImGui::Text("Precision (iterations)");
            ImGui::SliderInt("##save_precision", &tmp_precision, 1, 25000);

            if (ImGui::Button("Confirm")) {
                if (tmp_w > 0 && tmp_h > 0) {
                    std::string const filepath = "pics/" + plot.img_title;
                    save_plot_at_size(plot, tmp_w, tmp_h, filepath, tmp_precision);
                    printf("Saved picture as %s (res %dx%d, prec=%d)\n", filepath.c_str(), tmp_w, tmp_h, tmp_precision);
                }
                tmp_w = tmp_h = 0;
                tmp_precision = 0;
                close_save_dialog();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                tmp_w = tmp_h = 0;
                close_save_dialog();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        update_fractal(memory[memory_index].img);
        update_drag_box();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    draw_imgui_ui();    
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
        draw_imgui_ui();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return true;
}