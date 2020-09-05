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

        render::cam.position -= (bKeyA ? render::cam.speed : 0.0f) * render::cam.GetRightVector() * render::deltaTime;
        render::cam.position += (bKeyD ? render::cam.speed : 0.0f) * render::cam.GetRightVector() * render::deltaTime;
        render::cam.position += (bKeyW ? render::cam.speed : 0.0f) * render::cam.GetForwardVector() * render::deltaTime;
        render::cam.position -= (bKeyS ? render::cam.speed : 0.0f) * render::cam.GetForwardVector() * render::deltaTime;
        render::cam.position += (bKeySpace ? render::cam.speed : 0.0f) * render::cam.GetUpVector() * render::deltaTime;
        render::cam.position -= (bKeyCtrl ? render::cam.speed : 0.0f) * render::cam.GetUpVector() * render::deltaTime;
    }
}

void mouse_callback(GLFWwindow* window, double posX, double posY)
{
    static float lastX, lastY;

    float offsetX = posX - lastX;
    float offsetY = lastY - posY;

    lastX = posX;
    lastY = posY;

    static float pitch, yaw = -90.0f;

    if (isMouseHidden)
    {
        pitch += offsetY * 0.1f;
        yaw += offsetX * 0.1f;

        pitch = glm::clamp(pitch, -89.9f, 89.9f);

        render::cam.direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        render::cam.direction.y = sin(glm::radians(pitch));
        render::cam.direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (isMouseHidden)
    {
        render::cam.speed += yoffset;
        
        if (render::cam.speed > render::cam.maxSpeed)
            render::cam.speed = render::cam.maxSpeed;

        else if (render::cam.speed < 0.0f)
            render::cam.speed = 0.0f;
    }
}