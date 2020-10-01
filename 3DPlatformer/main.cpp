#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "screen.h"
#include "shape.h"
#include "shader.h"
#include "error.h"
#include "render.h"
#include "lua.h"
#include "gui.h"

#include <glm\glm.hpp>
#include <glm\gtx\string_cast.hpp>

#include <iostream>

void handleInput(GLFWwindow* window);

int main()
{
    lua::init();

    if (!screen::init(WindowMode::WINDOWED, 1024, 720, "RealEngine"))
        return -1;

    glfwMakeContextCurrent(screen::window);
    //glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        return -1;

    shaders::base_shader = CreateShaderProgram("res/shaders/base.shader");

    lua::LoadMaterials(render::materials);
    lua::LoadShapes(render::shapes, render::lights);

    render::cam.position = { 0.0f, 0.0f, 5.0f };

    GLCALL(glEnable(GL_CULL_FACE));
    GLCALL(glEnable(GL_DEPTH_TEST));

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(screen::window, true);
    ImGui_ImplOpenGL3_Init();

    while (!glfwWindowShouldClose(screen::window))
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        GUI::ShowMainWindow();

        for (auto& shape : render::shapes)
            shape.second->Draw();

        handleInput(screen::window);
        
        float currentFrame = glfwGetTime();
        render::deltaTime = currentFrame - render::lastFrame;
        render::lastFrame = currentFrame;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLCALL(glfwSwapBuffers(screen::window));
        GLCALL(glfwPollEvents());
    }

    // Clean-up

    for (auto& pair : render::shapes)
    {
        auto& shape = pair.second;
        shape.reset();
    }

    for (auto& pair : render::materials)
    {
        Material& mat = pair.second;
        GLuint textures[] = { mat.diffuse, mat.specular };

        GLCALL(glDeleteTextures(2, textures));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    GLCALL(glDeleteProgram(shaders::base_shader));
    glfwTerminate();

    lua::close();

    return 0;
}