#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "gui.h"
#include "render.h"

#include <list>
#include <vector>

bool showObjectsWindow;
bool showFPSChild;
std::list<float> timings;

void GUI::ShowMainWindow()
{
    ImGui::Begin("Debug");

    ImGui::Checkbox("FPS", &showFPSChild);

    if (showFPSChild)
        ShowFPSChild();

    ImGui::Checkbox("Objects", &showObjectsWindow);

    ImGui::End();

    if (showObjectsWindow)
        ShowObjectsWindow();
}

void GUI::ShowFPSChild()
{
    ImGui::BeginChild("FPS", ImVec2(0, 80));
    
    ImGui::Spacing();
    ImGui::Text("FPS");
    ImGui::SameLine();
    ImGui::TextColored(ImColor(255, 255, 0), "%.2f", 1 / render::deltaTime);
    ImGui::Separator();

    timings.push_back(1 / render::deltaTime);

    if (timings.size() > 100)
        timings.pop_front();

    std::vector<float> timing_values;

    float sum = 0;
    for (float& v : timings)
    {
        timing_values.push_back(v);
        sum += v;
    }

    float avg = sum / timings.size();
    char average[16];
    sprintf_s(average, "%.2f avg", avg);

    ImGui::PlotLines("Graph", timing_values.data(), timings.size(), 0, average, 0, avg * 2, ImVec2(0, 50));

    ImGui::EndChild();
}

void GUI::ShowObjectsWindow()
{
    ImGui::Begin("Objects", &showObjectsWindow);

    for (auto& shape : render::shapes)
    {
        if (ImGui::CollapsingHeader(shape.first.c_str()))
        {
            ImGui::BeginChild(shape.first.c_str(), ImVec2(0, 300));

            Shape* s = shape.second.get();

            ImGui::Spacing();
            ImGui::Text("Transform");
            ImGui::Separator();

            ImGui::DragFloat3("position", &s->position.x, 0.01f);
            ImGui::DragFloat3("scale", &s->scale.x, 0.01f, 0, 1000, "%.3f", ImGuiSliderFlags_ClampOnInput);

            glm::vec3 rotation = glm::degrees(s->rotation);
            ImGui::SliderFloat3("rotation", &rotation.x, 0, 360);
            s->rotation = glm::radians(rotation);

            ImGui::Spacing();
            ImGui::Text("Material");
            ImGui::Separator();

            ImGui::Image((void*)(intptr_t)s->material.diffuse, ImVec2(100, 100));
            ImGui::SameLine();
            ImGui::Image((void*)(intptr_t)s->material.specular, ImVec2(100, 100));

            ImGui::ColorEdit4("color", &s->material.color.x, ImGuiColorEditFlags_AlphaBar);
            ImGui::ColorEdit3("ambient", &s->material.ambient.x);
            ImGui::SliderFloat("shininess", &s->material.shininess, 0, 100);
            
            ImGui::EndChild();
        }
    }

    for (size_t i = 0; i < render::lights.size(); i++)
    {
        std::string name = std::string("Light") + std::to_string(i);

        if (ImGui::CollapsingHeader(name.c_str()))
        {
            ImGui::BeginChild(name.c_str(), ImVec2(0, 100));

            Light& l = render::lights[i];

            ImGui::Spacing();
            ImGui::Text("Transform");
            ImGui::Separator();

            ImGui::DragFloat3("position", &l.position.x, 0.01f);
            ImGui::DragFloat("intensity", &l.intensity, 0.1f, 0, 1000, "%.1f", ImGuiSliderFlags_ClampOnInput);
            ImGui::ColorEdit3("color", &l.color.r, ImGuiColorEditFlags_PickerHueWheel);

            ImGui::EndChild();
        }
    }

    ImGui::End();
}