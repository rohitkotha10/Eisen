#pragma once
#include "Eisen.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace Eisen;

int xmouse, ymouse;
int press = 0;

int screen_width = 800;
int screen_height = 800;
float screen_aspect = (float)screen_width / (float)screen_height;
float fov = 45.0f;

glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPos); //also facing the scene
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp)); //camera is in left hand system
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));

string getPos(int xpos, int ypos) //for mouse picking
{
	int file = 'a' - 1; //0 in alphabet
	int rank = 0;
	file += xpos / 80;
	rank += ypos / 80;
	rank = 8 - rank;
	rank += (int)'1';
	string s;
	s.push_back((char)file);
	s.push_back((char)rank);
	return s;
}

void paintBox(GLuint& program, string place, int occupy)
{
	int i = place[0] - ('a' - 1);
	int j = place[1] - ('1' - 1);
	glm::mat4 model = glm::mat4(1.0f);
	int n = 10;

	float scale = 1.0f / n;
	float incr = scale;

	float xshift = -1.0f + (2 * i + 1) * incr;
	float yshift = -1.0f + (2 * j + 1) * incr;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
	model = glm::scale(model, glm::vec3(scale));

	setInt(program, "choice", 2);
	setInt(program, "outChoice", 2);

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 mvp = projection * view * model;

	setMat4(program, "mvp_matrix", mvp);
	setInt(program, "occupy", occupy);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void paintPiece(GLuint& program, string place)
{
	int i = place[0] - ('a' - 1);
	int j = place[1] - ('1' - 1);
	glm::mat4 model = glm::mat4(1.0f);
	int n = 10;

	float scale = 1.0f / n;
	float incr = scale;

	float xshift = -1.0f + (2 * i + 1) * incr;
	float yshift = -1.0f + (2 * j + 1) * incr;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
	model = glm::scale(model, glm::vec3(scale));

	setInt(program, "choice", 1);
	setInt(program, "outChoice", 1);
	setVec4(program, "color", glm::vec4(1.0f, 1.0f, 0.0f, 0.6f));
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 mvp = projection * view * model;

	setMat4(program, "mvp_matrix", mvp);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void getPreview(GLuint& program, map<string, string>& chessTable, string place)
{
	paintPiece(program, place);
}

int processMove(map<string, string>& table, string move)
{
	string fir = move.substr(0, 2);
	string sec = move.substr(2, 2);
	if (table[fir] == "empty" || table[fir] == "" || table[fir] == table[sec]) //check valid here
		return -1;
	table[sec] = table[fir];
	table[fir] = "empty";
	return 0;
}

void place(GLuint& program, GLuint& piece, string place)
{
	int i = place[0] - ('a' - 1);
	int j = place[1] - ('1' - 1);
	glm::mat4 model = glm::mat4(1.0f);
	int n = 10;

	float scale = 1.0f / n;
	float incr = scale;

	float xshift = -1.0f + (2 * i + 1) * incr;
	float yshift = -1.0f + (2 * j + 1) * incr;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
	model = glm::scale(model, glm::vec3(scale));

	setInt(program, "choice", 3);
	setInt(program, "outChoice", 3);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 mvp = projection * view * model;

	setMat4(program, "mvp_matrix", mvp);
	setVec4(program, "color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, piece);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void load_tex(GLuint& tex, string path, bool needAlpha)
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
		std::cout << "Failed to load texture: " << path << std::endl;
	}
	stbi_image_free(data);
}