#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "gui.h"
#include "render.h"

void GUI::ShowObjectsWindow()
{
    ImGui::Begin("Objects");

    for (auto& shape : render::shapes)
    {
        if (ImGui::CollapsingHeader(shape.first.c_str()))
        {
            Shape* s = shape.second.get();

            ImGui::SliderFloat3("position", &s->position.x, -10, 10);
            ImGui::SliderFloat3("scale", &s->scale.x, 0, 10);

            glm::vec3 rotation = glm::degrees(s->rotation);
            ImGui::SliderFloat3("rotation", &rotation.x, 0, 360);
            s->rotation = glm::radians(rotation);
        }
    }

    for (size_t i = 0; i < render::lights.size(); i++)
    {
        std::string name = std::string("Light") + std::to_string(i);

        if (ImGui::CollapsingHeader(name.c_str()))
        {
            ImGui::BeginChild(name.c_str(), ImVec2(0, 100));

            Light& l = render::lights[i];

            ImGui::SliderFloat3("position", &l.position.x, -10, 10);
            ImGui::SliderFloat("intensity", &l.intensity, 0, 10);
            ImGui::ColorEdit3("color", &l.color.r);

            ImGui::EndChild();
        }
    }

    ImGui::End();
}