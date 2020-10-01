#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "gui.h"
#include "render.h"

#include <list>
#include <vector>

bool showShapesWindow;
bool showFPSChild = true;
bool showCamChild = true;

void GUI::ShowMainWindow()
{
    ImGui::Begin("Debug");

    ImGui::Checkbox("FPS", &showFPSChild);

    if (showFPSChild)
        ShowFPSChild();

    ImGui::Checkbox("Cam", &showCamChild);

    if (showCamChild)
        ShowCamChild();

    ImGui::Checkbox("Shapes", &showShapesWindow);

    ImGui::End();

    if (showShapesWindow)
        ShowShapesWindow();
}

void GUI::ShowCamChild()
{
    ImGui::BeginChild("Cam", ImVec2(0, 100));

    ImGui::Spacing();
    ImGui::Text("Camera");
    ImGui::Separator();

    ImGui::DragFloat3("position", &render::cam.position.x, 0.01f);
    ImGui::DragFloat3("direction", &render::cam.direction.x, 0.01f, 0, 0, "%.3f", ImGuiSliderFlags_ReadOnly | ImGuiSliderFlags_NoInput);
    ImGui::SliderFloat("speed", &render::cam.speed, 0, render::cam.maxSpeed, "%.2f");

    ImGui::EndChild();
}

void GUI::ShowFPSChild()
{
    ImGui::BeginChild("FPS", ImVec2(0, 80));
    
    ImGui::Spacing();
    ImGui::Text("FPS");
    ImGui::SameLine();
    ImGui::TextColored(ImColor(255, 255, 0), "%.2f", 1 / render::deltaTime);
    ImGui::Separator();

    static std::list<float> timings;
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

void GUI::ShowShapesWindow()
{
    ImGui::Begin("Shapes", &showShapesWindow);

    ImGui::BeginChild("Shapes", ImVec2(0, ImGui::GetWindowHeight() - 200));

    std::list<std::string> shapes_to_destroy;
    for (auto& shape : render::shapes)
    {
        if (ImGui::CollapsingHeader(shape.first.c_str()))
        {
            ImGui::BeginChild(shape.first.c_str(), ImVec2(0, 320));

            std::shared_ptr<Shape> s = shape.second;

            ImGui::Spacing();
            ImGui::Text("Transform");
            ImGui::Separator();

            ImGui::DragFloat3("position", &s->position.x, 0.01f);
            ImGui::DragFloat3("scale", &s->scale.x, 0.01f, 0, 1000, "%.3f", ImGuiSliderFlags_ClampOnInput);

            glm::vec3 rotation = glm::degrees(s->rotation);
            ImGui::SliderFloat3("rotation", &rotation.x, 0, 360);
            s->rotation = glm::radians(rotation);

            ImGui::Spacing();
            ImGui::SameLine(ImGui::GetWindowWidth() - 70);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 0, 0));
            if (ImGui::Button("Destroy"))
                shapes_to_destroy.push_back(shape.first);
            ImGui::PopStyleColor();

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

    std::list<std::string> lights_to_destroy;
    for (auto& light : render::lights)
    {
        if (ImGui::CollapsingHeader(light.first.c_str()))
        {
            ImGui::BeginChild(light.first.c_str(), ImVec2(0, 120));

            Light& l = light.second;

            ImGui::Spacing();
            ImGui::Text("Transform");
            ImGui::Separator();

            ImGui::DragFloat3("position", &l.position.x, 0.01f);
            ImGui::DragFloat("intensity", &l.intensity, 0.1f, 0, 1000, "%.1f", ImGuiSliderFlags_ClampOnInput);
            ImGui::ColorEdit3("color", &l.color.r, ImGuiColorEditFlags_PickerHueWheel);

            ImGui::Spacing();
            ImGui::SameLine(ImGui::GetWindowWidth() - 70);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 0, 0));
            if (ImGui::Button("Destroy"))
                lights_to_destroy.push_back(light.first);
            ImGui::PopStyleColor();

            ImGui::EndChild();
        }
    }

    for (auto& shape : shapes_to_destroy)
        render::shapes.erase(shape);

    for (auto& light : lights_to_destroy)
        render::lights.erase(light);

    ImGui::EndChild();

    ImGui::BeginChild("Shape creation");

    ImGui::Dummy(ImVec2(0, 0));
    ImGui::Text("Shape creation");
    ImGui::Separator();
    ImGui::Spacing();

    static bool shapedObject = true;
    static bool light = false;

    if (ImGui::RadioButton("ShapedObject", shapedObject))
    {
        shapedObject = true;
        light = false;
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Light", light))
    {
        shapedObject = false;
        light = true;
    }

    ImGui::Spacing();

    if (shapedObject)
    {
        static std::string name;
        static std::string object;
        static std::string material;

        ImGui::InputText("name", &name);
        ImGui::InputText(".obj path", &object, ImGuiInputTextFlags_CharsNoBlank);

        if (ImGui::BeginCombo("material", material.c_str()))
        {
            for (auto& mat : render::materials)
            {
                if (ImGui::Selectable(mat.first.c_str()))
                    material = mat.first;
            }

            ImGui::EndCombo();
        }

        ImGui::Dummy(ImVec2(0, ImGui::GetWindowHeight() - 150));

        if (ImGui::Button("Create", ImVec2(ImGui::GetWindowWidth(), 0)))
        {
            std::shared_ptr<Shape> shape;
            shape = std::make_shared<ShapedObject>(object.c_str());
            shape->material = render::materials[material];

            render::shapes.emplace(name, std::move(shape));
        }
    }

    else if (light)
    {
        static std::string name;
        static Light light;

        ImGui::InputText("name", &name);
        ImGui::ColorEdit3("color", &light.color.r, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::DragFloat("intensity", &light.intensity, 0.1f, 0, 1000, "%.1f", ImGuiSliderFlags_ClampOnInput);

        ImGui::Dummy(ImVec2(0, ImGui::GetWindowHeight() - 150));

        if (ImGui::Button("Create", ImVec2(ImGui::GetWindowWidth(), 0)))
            render::lights.emplace(name, light);
    }

    ImGui::EndChild();

    ImGui::End();
}