#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>

#include <glfw3.h>

#include "game.h"
#include "shape.h"
#include "player.h"

#include "glm\glm.hpp"
#include "glm\gtx\string_cast.hpp"

void keyPress(GLFWwindow*, int, int, int, int);

void framebuffer_size(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Fiola je legenda", NULL, NULL);

    glfwSetKeyCallback(window, keyPress);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
    glfwSetFramebufferSizeCallback(window, framebuffer_size);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    game::levels->at(game::currentLevel)();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (shape::Shape* shape : *game::shapes)
        {
            if (shape->isVisible())
            {
#ifdef  WIREFRAME
                shape->draw();
#else
                if (!shape->isDrawn())
                    shape->draw();
                else
                    shape->fill();
#endif
            }
        }

        game::tick();

        std::cout << "GAME -> FPS: " << 1.0 / game::deltaTime << std::endl;
        std::cout << "PLAYER -> Velocity: " << glm::to_string(game::player->getVelocity()) << std::endl;

        if (game::player->isDead())
            game::restartLevel();

        if (game::levelComplete)
            game::nextLevel();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean-up
    game::purge();

    glfwTerminate();
    return 0;
}