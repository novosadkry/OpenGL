#include "shape.h"
#include "error.h"
#include "shader.h"
#include "screen.h"
#include "render.h"
#include "object.h"

#include <glm\gtx\transform.hpp>
#include <iostream>

static GLuint GenerateVBO(GLfloat* vertices, GLsizei vertices_size)
{
    GLuint buffer;
    GLCALL(glGenBuffers(1, &buffer));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW));

    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0));
    GLCALL(glEnableVertexAttribArray(0));

    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3)));
    GLCALL(glEnableVertexAttribArray(1));

    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 6)));
    GLCALL(glEnableVertexAttribArray(2));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    std::cout << "[OpenGL:INFO] Generated object VBO (" << vertices_size / (sizeof(GLfloat) * 8) << " vertices, " << vertices_size << " bytes)" << std::endl;

    return buffer;
}

static GLuint GenerateIBO(GLuint* indices, GLsizei indices_size)
{
    GLuint buffer;
    GLCALL(glGenBuffers(1, &buffer));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW));

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    return buffer;
}

static void VBOTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
    GLCALL(GLint transform_loc = glGetUniformLocation(shaders::base_shader, "u_Transform"));
    ASSERT(transform_loc != -1);

    GLCALL(GLint normal_loc = glGetUniformLocation(shaders::base_shader, "u_Normal"));
    ASSERT(normal_loc != -1);

    glm::mat4 translate_mat = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rot_mat = glm::toMat4(glm::quat(rot));

    glm::mat4 transform_mat = translate_mat * rot_mat * scale_mat;
    glm::mat3 normal_mat = glm::transpose(glm::inverse(transform_mat));

    GLCALL(glUniformMatrix4fv(transform_loc, 1, GL_FALSE, &transform_mat[0][0]));
    GLCALL(glUniformMatrix3fv(normal_loc, 1, GL_FALSE, &normal_mat[0][0]));
}

static void VBOModelViewProjection(Camera* cam)
{
    GLCALL(GLint mvp_loc = glGetUniformLocation(shaders::base_shader, "u_MVP"));
    ASSERT(mvp_loc != -1);

    GLCALL(GLint viewPos_loc = glGetUniformLocation(shaders::base_shader, "u_ViewPos"));
    ASSERT(viewPos_loc != -1);

    glm::mat4 projection_mat = glm::perspective(
        glm::radians(45.0f),
        (float)screen::width / (float)screen::height,
        0.1f,
        100.0f
    );

    glm::mat4 view_mat = glm::lookAt(
        cam->position,
        cam->position + cam->GetForwardVector(),
        cam->GetUpVector()
    );

    glm::mat4 mvp = projection_mat * view_mat;

    GLCALL(glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]));
    GLCALL(glUniform3f(viewPos_loc, cam->position.x, cam->position.y, cam->position.z));
}

static void VBOMaterial(Material material)
{
    GLCALL(GLint ambient_loc = glGetUniformLocation(shaders::base_shader, "u_Material.ambient"));
    ASSERT(ambient_loc != -1);

    GLCALL(GLint shininess_loc = glGetUniformLocation(shaders::base_shader, "u_Material.shininess"));
    ASSERT(shininess_loc != -1);

    GLCALL(GLint color_loc = glGetUniformLocation(shaders::base_shader, "u_Material.color"));
    ASSERT(color_loc != -1);

    GLCALL(GLint diffuse_loc = glGetUniformLocation(shaders::base_shader, "u_Material.diffuse"));
    ASSERT(diffuse_loc != -1);

    GLCALL(GLint specular_loc = glGetUniformLocation(shaders::base_shader, "u_Material.specular"));
    ASSERT(specular_loc != -1);

    GLCALL(glUniform3f(ambient_loc, material.ambient.x, material.ambient.y, material.ambient.z));
    GLCALL(glUniform4f(color_loc, material.color.x, material.color.y, material.color.z, material.color.w));
    GLCALL(glUniform1f(shininess_loc, material.shininess));
    GLCALL(glUniform1i(diffuse_loc, 0));
    GLCALL(glUniform1i(specular_loc, 1));

    if (material.diffuse)
    {
        GLCALL(glActiveTexture(GL_TEXTURE0));
        GLCALL(glBindTexture(GL_TEXTURE_2D, material.diffuse));
    }

    if (material.specular)
    {
        GLCALL(glActiveTexture(GL_TEXTURE1));
        GLCALL(glBindTexture(GL_TEXTURE_2D, material.specular));
    }
}

static void VBOShading(std::vector<Light>* lights)
{
    for (int i = 0; i < lights->size(); i++)
    {
        Light& light = lights->at(i);
        char s[32];

        sprintf_s(s, "u_Light[%d].position", i);

        GLCALL(GLint pos_loc = glGetUniformLocation(shaders::base_shader, s));
        ASSERT(pos_loc != -1);

        sprintf_s(s, "u_Light[%d].color", i);

        GLCALL(GLint color_loc = glGetUniformLocation(shaders::base_shader, s));
        ASSERT(color_loc != -1);

        sprintf_s(s, "u_Light[%d].intensity", i);

        GLCALL(GLint intensity_loc = glGetUniformLocation(shaders::base_shader, s));
        ASSERT(intensity_loc != -1);

        GLCALL(glUniform3f(pos_loc, light.position.x, light.position.y, light.position.z));
        GLCALL(glUniform3f(color_loc, light.color.x, light.color.y, light.color.z));
        GLCALL(glUniform1f(intensity_loc, light.intensity));
    }

    GLCALL(GLint light_count_loc = glGetUniformLocation(shaders::base_shader, "u_Light_Count"));
    ASSERT(light_count_loc != -1);
    GLCALL(glUniform1i(light_count_loc, lights->size()));
}

glm::vec3 Camera::GetForwardVector()
{
    return glm::normalize(direction);
}

glm::vec3 Camera::GetRightVector()
{
    return glm::normalize(glm::cross(GetForwardVector(), glm::vec3(0, 1, 0)));
}

glm::vec3 Camera::GetUpVector()
{
    return glm::normalize(glm::cross(GetRightVector(), GetForwardVector()));
}

void Shape::Draw()
{
    GLCALL(glBindVertexArray(this->vao));
    GLCALL(glUseProgram(shaders::base_shader));

    VBOTransform(this->position, this->rotation, this->scale);
    VBOModelViewProjection(&render::cam);

    VBOMaterial(this->material);
    VBOShading(&render::lights);

    if (this->ibo)
    {
        GLCALL(glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_INT, nullptr));
    }

    else
    {
        GLCALL(glDrawArrays(GL_TRIANGLES, 0, this->indices));
    }
}

Shape::~Shape()
{
    GLuint buffers[] = { vbo, ibo };
    GLCALL(glDeleteBuffers(2, buffers));
    GLCALL(glDeleteVertexArrays(1, &vao));
}

ShapedObject::ShapedObject(const char* object_path)
{
    GLCALL(glGenVertexArrays(1, &this->vao));
    GLCALL(glBindVertexArray(this->vao));

    Object obj;

    if (render::objects.count(object_path) != 0)
        obj = render::objects[object_path];

    else
    {
        LoadObjectFromFile(&obj, object_path);
        render::objects[object_path] = obj;
    }

    std::vector<GLfloat> vertices = GenerateObjectVertices(obj);

    this->vbo = GenerateVBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    this->indices = obj.vert_indices.size();

    GLCALL(glBindVertexArray(0));
}