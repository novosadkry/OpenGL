#include "texture.h"
#include "error.h"

#include <iostream>

GLuint GenerateTexture(const char* texture_path)
{
	GLuint texture;

	GLCALL(glGenTextures(1, &texture));
	GLCALL(glBindTexture(GL_TEXTURE_2D, texture));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texture_path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	}

	else
		std::cout << "[OpenGL:WARN] " << "Failed to load texture from " << texture_path << std::endl;

	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	stbi_image_free(data);

	return texture;
}

GLuint GenerateTextureEmpty(GLsizei width, GLsizei height)
{
	GLuint texture;

	GLCALL(glGenTextures(1, &texture));
	GLCALL(glBindTexture(GL_TEXTURE_2D, texture));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	return texture;
}