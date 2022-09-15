#include "Eisen.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>;

#include <vector>

using namespace std;
using namespace Eisen;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

void  keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
};

class my_app : public OpenGLApp
{
	GLuint program;
	GLuint vao;
	GLuint vertBuffer;
	GLuint indBuffer;
	vector<vector<char>> board;
	GLuint texKingWhite;
	GLuint texQueenWhite;
	GLuint texRookWhite;
	GLuint texKnightWhite;
	GLuint texBishopWhite;
	GLuint texPawnWhite;

	GLuint texKingBlack;
	GLuint texQueenBlack;
	GLuint texRookBlack;
	GLuint texKnightBlack;
	GLuint texBishopBlack;
	GLuint texPawnBlack;
public:
	void init()
	{
		info.width = 800;
		info.height = 800;
		info.MajorVersion = 4;
		info.MinorVersion = 5;
		info.title = "Triangle";
		info.color = new float[4] {0.0f, 0.6f, 0.0f, 1.0f};
		info.fullscreen = false;
	}

	void shaderCompile()
	{
		//vertex shader
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		std::string trial = parse("res/shaders/vs.shader");
		const GLchar* vsSource = trial.c_str();
		glShaderSource(vs, 1, &vsSource, NULL);
		glCompileShader(vs);

		int success;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vs, 512, NULL, infoLog);
			std::cout << "Vertex Shader Error\n" << infoLog << std::endl;
		}

		//frag shader
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		trial = parse("res/shaders/fs.shader");
		const GLchar* fsSource = trial.c_str();
		glShaderSource(fs, 1, &fsSource, NULL);
		glCompileShader(fs);

		success = 0;
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fs, 512, NULL, infoLog);
			std::cout << "Frag Shader Error\n" << infoLog << std::endl;
		}

		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "Shader Linking Error\n" << infoLog << std::endl;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void startup()
	{
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, keyboard_callback);

		static const GLfloat vertices[] =
		{
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};
		static const int indices[] =
		{
			0, 1, 3,
			1, 2, 3
		};
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vertBuffer);
		glGenBuffers(1, &indBuffer);

		glGenTextures(1, &texKingWhite);
		glGenTextures(1, &texQueenWhite);
		glGenTextures(1, &texRookWhite);
		glGenTextures(1, &texKnightWhite);
		glGenTextures(1, &texBishopWhite);
		glGenTextures(1, &texPawnWhite);

		glGenTextures(1, &texKingBlack);
		glGenTextures(1, &texQueenBlack);
		glGenTextures(1, &texRookBlack);
		glGenTextures(1, &texKnightBlack);
		glGenTextures(1, &texBishopBlack);
		glGenTextures(1, &texPawnBlack);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		int n = 8;
		vector<vector<char>> temp(n, vector<char>(n));
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (i % 2 == 0)
				{
					if (j % 2 == 0)
						temp[i][j] = '0';
					else
						temp[i][j] = '1';
				}
				else
				{
					if (j % 2 == 0)
						temp[i][j] = '1';
					else
						temp[i][j] = '0';
				}
			}
		}
		board = temp;

		glBindTexture(GL_TEXTURE_2D, texKingWhite);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char* data = stbi_load("res/media/bking.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	void render(double currentTime)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(program);
		glBindVertexArray(vao);

		setInt(program, "tex", 0);
		setMat4(program, "trans_matrix", glm::mat4(1.0f));
		setVec3(program, "color", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glm::mat4 curMat = glm::mat4(1.0f);

		int n = board.size();

		float scale = 1.0f / n;
		float incr = scale;

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (board[i][j] == '0')
				{
					float xshift = -1.0f + (2 * j + 1) * incr;
					float yshift = 1.0f - (2 * i + 1) * incr;
					curMat = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
					curMat = glm::scale(curMat, glm::vec3(scale));

					setMat4(program, "trans_matrix", curMat);
					setVec3(program, "color", glm::vec3(0.7f, 0.5f, 0.3f));
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
		}

		float xshift = -1.0f + (2 * 0 + 1) * incr;
		float yshift = 1.0f - (2 * 0 + 1) * incr;
		curMat = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
		curMat = glm::scale(curMat, glm::vec3(scale));

		setInt(program, "tex", 1);
		setMat4(program, "trans_matrix", curMat);
		setVec3(program, "color", glm::vec3(0.2f, 0.2f, 0.2f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texKingWhite);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void shutdown()
	{
		glDeleteProgram(program);
		glDeleteBuffers(1, &vertBuffer);
		glDeleteBuffers(1, &indBuffer);
		glDeleteVertexArrays(1, &vao);
	}
};

START_APP(my_app); 