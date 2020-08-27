#include "shape.h"
#include "error.h"
#include "shader.h"
#include "screen.h"
#include "render.h"
#include "object.h"

#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

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

static void VBOTransform(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
{
    GLCALL(GLint loc = glGetUniformLocation(shaders::base_shader, "u_Transform"));
    ASSERT(loc != -1);

    glm::mat4 translate_mat = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rot_mat = glm::toMat4(rot);

    glm::mat4 transform_mat = translate_mat * rot_mat * scale_mat;

    GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, &transform_mat[0][0]));
}

static void VBOModelViewProjection(Camera* cam)
{
    GLCALL(GLint loc = glGetUniformLocation(shaders::base_shader, "u_MVP"));
    ASSERT(loc != -1);

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

    GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));
}

static void VBOColor(glm::vec4 color)
{
    GLCALL(GLint loc = glGetUniformLocation(shaders::base_shader, "u_Color"));
    ASSERT(loc != -1);
    GLCALL(glUniform4f(loc, color.x, color.y, color.z, color.w));
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

        GLCALL(glUniform3f(pos_loc, light.position->x, light.position->y, light.position->z));
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

    VBOColor(this->color);
    VBOShading(&render::lights);

    if (this->texture)
    {
        GLCALL(glBindTexture(GL_TEXTURE_2D, texture));
    }

    if (this->ibo)
    {
        GLCALL(glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_INT, nullptr));
    }

    else
    {
        GLCALL(glDrawArrays(GL_TRIANGLES, 0, this->indices));
    }

    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    GLCALL(glBindVertexArray(0));
}

Shape::~Shape()
{
    GLuint buffers[] = { vbo, ibo };
    GLCALL(glDeleteBuffers(2, buffers));
    GLCALL(glDeleteTextures(1, &texture));
    GLCALL(glDeleteVertexArrays(1, &vao));
}

ShapedObject::ShapedObject(const char* object_path, const char* texture_path)
{
    GLCALL(glGenVertexArrays(1, &this->vao));
    GLCALL(glBindVertexArray(this->vao));

    Object obj;

    LoadObjectFromFile(&obj, object_path);
    std::vector<GLfloat> vertices = GenerateObjectVertices(obj);

    this->vbo = GenerateVBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    this->indices = obj.vert_indices.size();
    this->texture = GenerateTexture(texture_path);

    GLCALL(glBindVertexArray(0));
}