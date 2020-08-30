#include "screen.h"
#include "shape.h"
#include "shader.h"
#include "error.h"
#include "render.h"
#include "lua.h"

#include <glm\glm.hpp>
#include <glm\gtx\string_cast.hpp>

#include <iostream>

void handleInput(GLFWwindow* window);

int main()
{
    lua::init();

    if (!screen::init(WindowMode::WINDOWED, 1024, 720, "Fiola je legenda"))
        return -1;

    glfwMakeContextCurrent(screen::window);
    //glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        return -1;

    shaders::base_shader = CreateShaderProgram();

    lua::LoadMaterials(render::materials);
    lua::LoadObjects(render::shapes, render::lights);

    render::cam.position = { 0.0f, 0.0f, 5.0f };

    GLCALL(glEnable(GL_CULL_FACE));
    GLCALL(glEnable(GL_DEPTH_TEST));

    while (!glfwWindowShouldClose(screen::window))
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (auto& shape : render::shapes)
            shape.second->Draw();

        handleInput(screen::window);
        
        float currentFrame = glfwGetTime();
        render::deltaTime = currentFrame - render::lastFrame;
        render::lastFrame = currentFrame;

        //std::cout << "FPS: " << 1.0f / render::deltaTime << std::endl;

        GLCALL(glfwSwapBuffers(screen::window));
        GLCALL(glfwPollEvents());
    }

    // Clean-up

    for (auto& pair : render::shapes)
    {
        auto& shape = pair.second;
        shape.release();
    }

    for (auto& pair : render::materials)
    {
        Material& mat = pair.second;
        GLuint textures[] = { mat.diffuse, mat.specular };

        GLCALL(glDeleteTextures(2, textures));
    }

    glDeleteProgram(shaders::base_shader);
    glfwTerminate();

    lua::close();

    return 0;
}