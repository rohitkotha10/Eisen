#include "chess.h"

#include <windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}//force GPU use

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		press = 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

void  keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	xmouse = (int)xpos;
	ymouse = (int)ypos;
}

class my_app : public OpenGLApp
{
	GLuint program;
	GLuint vao;
	GLuint vertBuffer;
	GLuint indBuffer;

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

	map<string, string> chessTable;
	int startGame;
	int preview; //is move display on or off
	string curPreview; //current preview position
	string prevStart;
	string prevEnd;
	int turn; //0 for black, 1 for white
	int counter;

public:
	void init()
	{
		info.width = 800;
		info.height = 800;
		info.MajorVersion = 4;
		info.MinorVersion = 5;
		info.title = "Chess";
		info.color = new float[4] {0.1f, 0.1f, 0.1f, 1.0f};
		info.fullscreen = false;
		info.resize = false;
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

		cout << "Welcome to CHESS!!!\n";
		cout << "White to Start\n1: ";

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, keyboard_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);


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

		chessTable["a1"] = "wr";
		chessTable["b1"] = "wn";
		chessTable["c1"] = "wb";
		chessTable["d1"] = "wq";
		chessTable["e1"] = "wk";
		chessTable["f1"] = "wb";
		chessTable["g1"] = "wn";
		chessTable["h1"] = "wr";

		chessTable["a2"] = "wp";
		chessTable["b2"] = "wp";
		chessTable["c2"] = "wp";
		chessTable["d2"] = "wp";
		chessTable["e2"] = "wp";
		chessTable["f2"] = "wp";
		chessTable["g2"] = "wp";
		chessTable["h2"] = "wp";

		chessTable["a8"] = "br";
		chessTable["b8"] = "bn";
		chessTable["c8"] = "bb";
		chessTable["d8"] = "bq";
		chessTable["e8"] = "bk";
		chessTable["f8"] = "bb";
		chessTable["g8"] = "bn";
		chessTable["h8"] = "br";

		chessTable["a7"] = "bp";
		chessTable["b7"] = "bp";
		chessTable["c7"] = "bp";
		chessTable["d7"] = "bp";
		chessTable["e7"] = "bp";
		chessTable["f7"] = "bp";
		chessTable["g7"] = "bp";
		chessTable["h7"] = "bp";

		startGame = 0;
		preview = 0;
		turn = 1;
		counter = 1;


	}

	void render(double currentTime)
	{
		glUseProgram(program);
		glBindVertexArray(vao);

		glm::mat4 model = glm::mat4(1.0f);
		int n = 10;

		float scale = 1.0f / n;
		float incr = scale;
		for (int i = 1; i <= 8; i++)
		{
			for (int j = 1; j <= 8; j++)
			{
				if ((i + j) % 2 == 0)
					setVec4(program, "color", glm::vec4(0.5f, 0.6f, 0.3f, 1.0f));
				else
					setVec4(program, "color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				float xshift = -1.0f + (2 * i + 1) * incr;
				float yshift = -1.0f + (2 * j + 1) * incr;
				model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
				model = glm::scale(model, glm::vec3(scale));

				setInt(program, "choice", 0);
				setInt(program, "outChoice", 0);
				glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
				glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
				glm::mat4 mvp = projection * view * model;

				setMat4(program, "mvp_matrix", mvp);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			}
		}

		if (preview == 1)
		{
			getPreview(program, chessTable, curPreview);
		}

		if (startGame == 1)
		{
			paintPiece(program, prevStart);
			paintPiece(program, prevEnd);
		}

		for (pair<string, string> cur : chessTable)
		{
			if (cur.second == "empty")
				continue;
			if (cur.second == "wp")
				place(program, texPawnWhite, cur.first);
			else if (cur.second == "wr")
				place(program, texRookWhite, cur.first);
			else if (cur.second == "wn")
				place(program, texKnightWhite, cur.first);
			else if (cur.second == "wb")
				place(program, texBishopWhite, cur.first);
			else if (cur.second == "wq")
				place(program, texQueenWhite, cur.first);
			else if (cur.second == "wk")
				place(program, texKingWhite, cur.first);

			else if (cur.second == "bp")
				place(program, texPawnBlack, cur.first);
			else if (cur.second == "br")
				place(program, texRookBlack, cur.first);
			else if (cur.second == "bn")
				place(program, texKnightBlack, cur.first);
			else if (cur.second == "bb")
				place(program, texBishopBlack, cur.first);
			else if (cur.second == "bq")
				place(program, texQueenBlack, cur.first);
			else if (cur.second == "bk")
				place(program, texKingBlack, cur.first);
		}

		if (press == 1)
		{
			onClick();
			press = 0;
		}
	}

	void onClick()
	{
		string curSel = getPos(xmouse, ymouse);
		string curPiece = chessTable[curSel];
		if ((curPiece[0] == 'w' && turn == 1) || (curPiece[0] == 'b' && turn == 0))
		{
			preview = 1;
			curPreview = curSel;
		}

		else
		{
			if (preview == 1)
			{
				//check if move is valid and proceed or else put preview to 0;
				string query = curPreview + curSel;

				if (processMove(chessTable, query) == 0)
				{
					startGame = 1;
					cout << query << ' ';
					if (turn == 0)
					{
						counter++;
						cout << endl;
						cout << counter << ": ";
					}
					turn = 1 - turn;
					prevStart = curPreview;
					prevEnd = curSel;
				}

			}
			preview = 0;
		}
	}

	void shutdown()
	{
		glDeleteProgram(program);
		glDeleteBuffers(1, &vertBuffer);
		glDeleteBuffers(1, &indBuffer);
		glDeleteVertexArrays(1, &vao);

		glDeleteTextures(1, &texKingWhite);
		glDeleteTextures(1, &texQueenWhite);
		glDeleteTextures(1, &texRookWhite);
		glDeleteTextures(1, &texKnightWhite);
		glDeleteTextures(1, &texBishopWhite);
		glDeleteTextures(1, &texPawnWhite);

		glDeleteTextures(1, &texKingBlack);
		glDeleteTextures(1, &texQueenBlack);
		glDeleteTextures(1, &texRookBlack);
		glDeleteTextures(1, &texKnightBlack);
		glDeleteTextures(1, &texBishopBlack);
		glDeleteTextures(1, &texPawnBlack);
	}
};

int main()
{
	my_app* app = new my_app;
	app->run();
	delete app;
	return 0;
}