#include "screen.h"
#include "shape.h"
#include "shader.h"
#include "error.h"
#include "render.h"

#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

#include <glm\glm.hpp>
#include <glm\gtx\string_cast.hpp>

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

    Material defaultMat;
    defaultMat.ambient = { 0.2f, 0.2f, 0.2f };
    defaultMat.shininess = 32.0f;
    defaultMat.diffuse = GenerateTexture("res/textures/white.png");
    defaultMat.specular = GenerateTexture("res/textures/white.png");
    defaultMat.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    Material crateMat;
    crateMat.ambient = { 0.2f, 0.2f, 0.2f };
    crateMat.shininess = 32.0f;
    crateMat.diffuse = GenerateTexture("res/textures/container.png");
    crateMat.specular = GenerateTexture("res/textures/container_specular.png");
    crateMat.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    Shape* textShape = new ShapedObject("res/obj/napis.obj");
    textShape->position = { 2.0f, 0.0f, 0.0f };
    textShape->material = defaultMat;

    Shape* crate = new ShapedObject("res/obj/cube.obj");
    crate->position = { 0.0f, 0.0f, 0.0f };
    crate->material = crateMat;

    Shape* cubeLight = new ShapedObject("res/obj/cube.obj");
    cubeLight->scale = { 0.1f, 0.1f, 0.1f };
    cubeLight->position = { 0.3f, 0.3f, 5.0f };
    cubeLight->material = defaultMat;

    Shape* cubeLight2 = new ShapedObject("res/obj/cube.obj");
    cubeLight2->scale = { 0.1f, 0.1f, 0.1f };
    cubeLight2->position = { 2.3f, 0.3f, 5.0f };
    cubeLight2->material = defaultMat;

    render::shapes.push_back(textShape);
    render::shapes.push_back(crate);
    render::shapes.push_back(cubeLight);
    render::shapes.push_back(cubeLight2);

    Light light;
    light.intensity = 2.0f;
    light.position = &cubeLight->position;

    Light light2;
    light2.intensity = 2.0f;
    light2.position = &cubeLight2->position;

    render::cam.position = { 0.0f, 0.0f, 5.0f };
    render::lights.push_back(light);
    render::lights.push_back(light2);

    GLCALL(glEnable(GL_CULL_FACE));
    GLCALL(glEnable(GL_DEPTH_TEST));

    while (!glfwWindowShouldClose(screen::window))
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (auto& shape : render::shapes)
            shape->Draw();

        handleInput(screen::window);
        
        float currentFrame = glfwGetTime();
        render::deltaTime = currentFrame - render::lastFrame;
        render::lastFrame = currentFrame;

        //std::cout << "FPS: " << 1.0f / render::deltaTime << std::endl;

        GLCALL(glfwSwapBuffers(screen::window));
        GLCALL(glfwPollEvents());
    }

    for (auto& shape : render::shapes)
        delete shape;

    glDeleteProgram(shaders::base_shader);
    glfwTerminate();

    return 0;
}