#include <GL/glew.h>

#include "keys.hpp"
#include "window.hpp"
#include "mandel_calculation.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    DragState& drag = win->get_drag_state();

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &drag.startX, &drag.startY);
            drag.dragging = true;
            drag.endX = drag.startX;
            drag.endY = drag.startY;
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
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    DragState& drag = win->get_drag_state();

    if (drag.dragging) {
        drag.endX = xpos;
        drag.endY = ypos;

        drag.dirty = true;
    }
}