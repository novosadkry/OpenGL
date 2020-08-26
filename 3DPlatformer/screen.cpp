#include "screen.h"
#include "error.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void framebuffer_size(GLFWwindow* window, int width, int height)
{
    screen::width = width;
    screen::height = height;

    glViewport(0, 0, width, height);
}

namespace screen
{
    int width = 0;
    int height = 0;

    GLFWwindow* window = nullptr;

    int init(WindowMode window_mode, int width, int height, const char* title)
    {
        if (!glfwInit())
            return 0;

        if (screen::window)
            glfwDestroyWindow(screen::window);
        
        GLFWwindow* window = nullptr;
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        switch (window_mode)
        {
            case WindowMode::WINDOWED:
                window = glfwCreateWindow(
                    width,
                    height,
                    title,
                    NULL,
                    NULL
                );
                break;

            case WindowMode::FULLSCREEN:
                window = glfwCreateWindow(
                    width,
                    height,
                    title,
                    monitor,
                    NULL
                );
                break;

            case WindowMode::BORDERLESS:
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

                window = glfwCreateWindow(
                    mode->width,
                    mode->height,
                    title,
                    monitor,
                    NULL
                );
                break;
        }

        if (!window)
        {
            glfwTerminate();
            return 0;
        }

        glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetFramebufferSizeCallback(window, framebuffer_size);
        glfwSetCursorPosCallback(window, mouse_callback);

        glfwGetWindowSize(window, &screen::width, &screen::height);

        screen::window = window;

        return 1;
    }
}