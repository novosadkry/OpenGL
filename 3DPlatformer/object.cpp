#include "object.h"

#include <iostream>
#include <fstream>
#include <string>

void LoadObjectFromFile(Object* obj, const char* path)
{
    std::ifstream file_stream(path);
    std::string line;

    while (std::getline(file_stream, line))
    {
        if (line[0] == 'v')
        {
            if (line[1] == 'n')
            {
                glm::vec3 vn;
                sscanf_s(&line.c_str()[2], "%f %f %f\n", &vn.x, &vn.y, &vn.z);
                obj->normals.push_back(vn);
            }

            else if (line[1] == 't');

            else
            {
                glm::vec3 v;
                sscanf_s(&line.c_str()[2], "%f %f %f\n", &v.x, &v.y, &v.z);
                obj->vertices.push_back(v);
            }
        }

        else if (line[0] == 'f')
        {
            GLuint vi[3], ni[3];
            sscanf_s(&line.c_str()[2], "%d/%*d/%d %d/%*d/%d %d/%*d/%d\n", &vi[0], &ni[0], &vi[1], &ni[1], &vi[2], &ni[2]);
            obj->vert_indices.push_back(vi[0] - 1);
            obj->vert_indices.push_back(vi[1] - 1);
            obj->vert_indices.push_back(vi[2] - 1);
            obj->normal_indices.push_back(ni[0] - 1);
            obj->normal_indices.push_back(ni[1] - 1);
            obj->normal_indices.push_back(ni[2] - 1);
        }
    }
}

std::vector<glm::vec3> GenerateObjectVertices(Object& obj)
{
    std::vector<glm::vec3> out;

    for (int i = 0; i < obj.vert_indices.size(); i++)
    {
        int vert_index = obj.vert_indices.at(i);
        int normal_index = obj.normal_indices.at(i);

        out.push_back(obj.vertices[vert_index]);
        out.push_back(obj.normals[normal_index]);
    }

    return out;
}