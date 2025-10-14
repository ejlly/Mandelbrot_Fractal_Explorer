#include <GL/glew.h>
#include "keys.hpp"
#include "window.hpp"
#include "mandel_calculation.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    DragState& drag = win->get_drag_state();

    if (ImGui::GetIO().WantCaptureKeyboard)
        return;
    
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS) {
        int const memory_index = win->get_memory_index();
        if (memory_index > 0) {
            win->set_memory_index(memory_index - 1);
        }
    }
    else if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_E) && action == GLFW_PRESS) {
        int const memory_index = win->get_memory_index();
        if (win->get_memory_index() + 1 < win->get_memory_size()) {
            win->set_memory_index(memory_index + 1);
        }
    }
    else if(key == GLFW_KEY_R && action == GLFW_RELEASE) {
        //redraw
        drag.dragging = false;
        drag.dirty = false;

        win->add_plot(0, 0, win->getwidth()-1, win->getheight()-1, true);
    }
    else if(key == GLFW_KEY_S && action == GLFW_RELEASE) {
        //save current image
        Plot& plot = win->get_last_plot();
        std::string const filepath = "pics/" + plot.img_title;
        plot.img.save_BMP(filepath);
        printf("Saved picture as %s\n", filepath.c_str());
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (ImGui::GetIO().WantCaptureMouse)
        return;
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    DragState& drag = win->get_drag_state();

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &drag.startX, &drag.startY);
            drag.dragging = true;
            drag.endX = drag.startX;
            drag.endY = drag.startY;
            drag.dirty = true;
        }
        else if (drag.dragging && action == GLFW_RELEASE) {
            glfwGetCursorPos(window, &drag.endX, &drag.endY);
            drag.dragging = false;
            drag.dirty = false;

            if(drag.startX != drag.endX && drag.startY != drag.endY){
                win->add_plot(static_cast<int>(drag.startX),
                              static_cast<int>(drag.startY),
                              static_cast<int>(drag.endX),
                              static_cast<int>(drag.endY)
                             );
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        drag.dragging = false;
        drag.dirty = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    if (ImGui::GetIO().WantCaptureMouse)
        return;
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    DragState& drag = win->get_drag_state();

    if (drag.dragging) {
        drag.endX = xpos;
        drag.endY = ypos;

        drag.dirty = true;
    }
}