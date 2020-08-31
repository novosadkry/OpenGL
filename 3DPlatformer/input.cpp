#include "render.h"

bool isMouseHidden;

void handleInput(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        isMouseHidden = true;
    }

    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        isMouseHidden = false;
    }

    if (isMouseHidden)
    {
        bool bKeyA = (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT));
        bool bKeyD = (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT));
        bool bKeyW = (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP));
        bool bKeyS = (glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN));

        bool bKeySpace = glfwGetKey(window, GLFW_KEY_SPACE);
        bool bKeyCtrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);

        render::cam.position -= (bKeyA ? 1.0f : 0.0f) * render::cam.GetRightVector() * render::deltaTime;
        render::cam.position += (bKeyD ? 1.0f : 0.0f) * render::cam.GetRightVector() * render::deltaTime;
        render::cam.position += (bKeyW ? 1.0f : 0.0f) * render::cam.GetForwardVector() * render::deltaTime;
        render::cam.position -= (bKeyS ? 1.0f : 0.0f) * render::cam.GetForwardVector() * render::deltaTime;
        render::cam.position += (bKeySpace ? 1.0f : 0.0f) * render::cam.GetUpVector() * render::deltaTime;
        render::cam.position -= (bKeyCtrl ? 1.0f : 0.0f) * render::cam.GetUpVector() * render::deltaTime;
    }
}

void mouse_callback(GLFWwindow* window, double posX, double posY)
{
    static float lastX, lastY;

    if (isMouseHidden)
    {
        float offsetX = posX - lastX;
        float offsetY = lastY - posY;

        lastX = posX;
        lastY = posY;

        static float pitch, yaw = -180.0f;

        pitch += offsetY * 0.1f;
        yaw += offsetX * 0.1f;

        pitch = glm::clamp(pitch, -89.9f, 89.9f);

        render::cam.direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        render::cam.direction.y = sin(glm::radians(pitch));
        render::cam.direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    }
}