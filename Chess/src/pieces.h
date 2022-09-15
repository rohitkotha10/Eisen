#pragma once
#include "Eisen.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>;

using namespace std;
using namespace Eisen;

void load_tex(GLuint tex, string path, bool needAlpha)
{
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (needAlpha == false)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void place(GLuint program, GLuint piece, string place)
{
	int i = place[0] - 64;
	int j = place[1] - '0';
	glm::mat4 curMat = glm::mat4(1.0f);
	int n = 10;

	float scale = 1.0f / n;
	float incr = scale;

	float xshift = -1.0f + (2 * i + 1) * incr;
	float yshift = -1.0f + (2 * j + 1) * incr;
	curMat = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
	curMat = glm::scale(curMat, glm::vec3(scale));

	setMat4(program, "trans_matrix", curMat);
	setVec3(program, "color", glm::vec3(0.2f, 0.2f, 0.2f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, piece);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}