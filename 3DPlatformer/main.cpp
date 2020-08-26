#include "screen.h"
#include "shape.h"
#include "shader.h"
#include "error.h"
#include "render.h"

#include "glm\glm.hpp"
#include "glm\gtx\string_cast.hpp"

#include <iostream>

void handleInput(GLFWwindow* window);

int main()
{
    if (!screen::init(WindowMode::WINDOWED, 1024, 720, "Fiola je legenda"))
        return -1;

    glfwMakeContextCurrent(screen::window);
    //glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        return -1;

    shaders::base_shader = CreateShaderProgram();

    std::vector<Shape*> shapes;

    Shape* cube = new ShapedObject("res/obj/zuga.obj");
    cube->color = { 1.0f, 0.2f, 0.2f, 1.0f };

    Shape* cubeLight = new Cube();
    cubeLight->scale = { 0.1f, 0.1f, 0.1f };
    cubeLight->position = { 0.3f, 0.3f, 5.0f };

    shapes.push_back(cube);
    shapes.push_back(cubeLight);

    Camera cam;
    cam.position = { 0.0f, 0.0f, 5.0f };

    Light light;
    light.intensity = 5.0f;
    light.position = &cubeLight->position;

    render::cam = &cam;
    render::light = &light;

    GLCALL(glEnable(GL_CULL_FACE));
    GLCALL(glEnable(GL_DEPTH_TEST));

    while (!glfwWindowShouldClose(screen::window))
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (auto& shape : shapes)
            shape->Draw();

        handleInput(screen::window);
        
        float currentFrame = glfwGetTime();
        render::deltaTime = currentFrame - render::lastFrame;
        render::lastFrame = currentFrame;

        std::cout << "FPS: " << 1.0f / render::deltaTime << std::endl;

        GLCALL(glfwSwapBuffers(screen::window));
        GLCALL(glfwPollEvents());
    }

    for (auto& shape : shapes)
        delete shape;

    glDeleteProgram(shaders::base_shader);
    glfwTerminate();

    return 0;
}