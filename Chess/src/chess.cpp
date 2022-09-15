#include "Eisen.h"
#include "pieces.h"

#include <vector>

using namespace std;
using namespace Eisen;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//glViewport(0, 0, width, height);
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
	GLuint texBoard;
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

		glGenTextures(1, &texBoard);

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

		int n = 10;
		vector<vector<char>> temp(n, vector<char>(n, 'n'));
		for (int i = 1; i < 9; i++)
		{
			for (int j = 1; j < 9; j++)
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

		load_tex(texBoard, "res/media/board.jpg", false);

		load_tex(texKingWhite, "res/media/wking.png", true);
		load_tex(texQueenWhite, "res/media/wqueen.png", true);
		load_tex(texRookWhite, "res/media/wrook.png", true);
		load_tex(texKnightWhite, "res/media/wknight.png", true);
		load_tex(texBishopWhite, "res/media/wbishop.png", true);
		load_tex(texPawnWhite, "res/media/wpawn.png", true);

		load_tex(texKingBlack, "res/media/bking.png", true);
		load_tex(texQueenBlack, "res/media/bqueen.png", true);
		load_tex(texRookBlack, "res/media/brook.png", true);
		load_tex(texKnightBlack, "res/media/bknight.png", true);
		load_tex(texBishopBlack, "res/media/bbishop.png", true);
		load_tex(texPawnBlack, "res/media/bpawn.png", true);
	}

	void render(double currentTime)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(program);
		glBindVertexArray(vao);

		setMat4(program, "trans_matrix", glm::mat4(1.0f));
		setVec3(program, "color", glm::vec3(0.7f, 0.6f, 0.5f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texBoard);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		place(program, texRookWhite, "A1");
		place(program, texKnightWhite, "B1");
		place(program, texBishopWhite, "C1");
		place(program, texQueenWhite, "D1");
		place(program, texKingWhite, "E1");
		place(program, texBishopWhite, "F1");
		place(program, texKnightWhite, "G1");
		place(program, texRookWhite, "H1");

		place(program, texPawnWhite, "A2");
		place(program, texPawnWhite, "B2");
		place(program, texPawnWhite, "C2");
		place(program, texPawnWhite, "D2");
		place(program, texPawnWhite, "E2");
		place(program, texPawnWhite, "F2");
		place(program, texPawnWhite, "G2");
		place(program, texPawnWhite, "H2");

		place(program, texRookBlack, "A8");
		place(program, texKnightBlack, "B8");
		place(program, texBishopBlack, "C8");
		place(program, texQueenBlack, "D8");
		place(program, texKingBlack, "E8");
		place(program, texBishopBlack, "F8");
		place(program, texKnightBlack, "G8");
		place(program, texRookBlack, "H8");

		place(program, texPawnBlack, "A7");
		place(program, texPawnBlack, "B7");
		place(program, texPawnBlack, "C7");
		place(program, texPawnBlack, "D7");
		place(program, texPawnBlack, "E7");
		place(program, texPawnBlack, "F7");
		place(program, texPawnBlack, "G7");
		place(program, texPawnBlack, "H7");



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