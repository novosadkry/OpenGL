#include "object.h"

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

            else if (line[1] == 't')
            {
                glm::vec2 vt;
                sscanf_s(&line.c_str()[2], "%f %f\n", &vt.x, &vt.y);
                obj->uvs.push_back(vt);
            }

            else
            {
                glm::vec3 v;
                sscanf_s(&line.c_str()[2], "%f %f %f\n", &v.x, &v.y, &v.z);
                obj->vertices.push_back(v);
            }
        }

        else if (line[0] == 'f')
        {
            GLuint vi[3], uvi[3], ni[3];
            sscanf_s(&line.c_str()[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vi[0], &uvi[0], &ni[0], &vi[1], &uvi[1], &ni[1], &vi[2], &uvi[2], &ni[2]);
            obj->vert_indices.push_back(vi[0] - 1);
            obj->vert_indices.push_back(vi[1] - 1);
            obj->vert_indices.push_back(vi[2] - 1);
            obj->uv_indices.push_back(uvi[0] - 1);
            obj->uv_indices.push_back(uvi[1] - 1);
            obj->uv_indices.push_back(uvi[2] - 1);
            obj->normal_indices.push_back(ni[0] - 1);
            obj->normal_indices.push_back(ni[1] - 1);
            obj->normal_indices.push_back(ni[2] - 1);
        }
    }
}

std::vector<Vertex> GenerateObjectVertices(Object& obj)
{
    std::vector<Vertex> out;

    for (size_t i = 0; i < obj.vert_indices.size(); i++)
    {
        const int vert_index = obj.vert_indices[i];
        const int normal_index = obj.normal_indices[i];
        const int uvs_index = obj.uv_indices[i];
    	
        out.emplace_back(Vertex {
            obj.vertices[vert_index],
            obj.normals[normal_index],
            obj.uvs[uvs_index]
        });
    }

    return out;
}