#include "pieces.h"

#include <windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}//force GPU use

int start = 0;
int xmouse, ymouse;
int press = 0;
int selected = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		press = 1 - press;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//glViewport(0, 0, width, height);
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

void onClick()
{

}

class my_app : public OpenGLApp
{
	GLuint program;
	GLuint vao;
	GLuint vertBuffer;
	GLuint indBuffer;
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

	map<string, string> chessTable;
	string curSel;
public:
	void init()
	{
		info.width = 800;
		info.height = 800;
		info.MajorVersion = 4;
		info.MinorVersion = 5;
		info.title = "Chess";
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
		std::cout << glGetString(GL_VENDOR) << std::endl;
		std::cout << glGetString(GL_VERSION) << std::endl;
		std::cout << glGetString(GL_RENDERER) << std::endl << std::endl;

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

		chessTable["A1"] = "wr";
		chessTable["B1"] = "wn";
		chessTable["C1"] = "wb";
		chessTable["D1"] = "wq";
		chessTable["E1"] = "wk";
		chessTable["F1"] = "wb";
		chessTable["G1"] = "wn";
		chessTable["H1"] = "wr";

		chessTable["A2"] = "wp";
		chessTable["B2"] = "wp";
		chessTable["C2"] = "wp";
		chessTable["D2"] = "wp";
		chessTable["E2"] = "wp";
		chessTable["F2"] = "wp";
		chessTable["G2"] = "wp";
		chessTable["H2"] = "wp";

		chessTable["A8"] = "br";
		chessTable["B8"] = "bn";
		chessTable["C8"] = "bb";
		chessTable["D8"] = "bq";
		chessTable["E8"] = "bk";
		chessTable["F8"] = "bb";
		chessTable["G8"] = "bn";
		chessTable["H8"] = "br";

		chessTable["A7"] = "bp";
		chessTable["B7"] = "bp";
		chessTable["C7"] = "bp";
		chessTable["D7"] = "bp";
		chessTable["E7"] = "bp";
		chessTable["F7"] = "bp";
		chessTable["G7"] = "bp";
		chessTable["H7"] = "bp";
		press = 0;
	}

	void render(double currentTime)
	{
		/*if (start == 1)
		{
			cout << "White Turn: ";
			string query;
			cin >> query;
			getMove(chessTable, query);
			start = 2;
		}

		else if (start == 2)
		{
			cout << "Black Turn: ";
			string query;
			cin >> query;
			cout << endl;
			getMove(chessTable, query);
			start = 1;
		}

		if (start == 0)
		{
			cout << "Welcome to CHESS!!!\n" <<
				"Enter your moves in the Long Algebraic Notation i.e. <startplace><endplace>\n" <<
				"Example: a1a2, will move piece from A1 to A2\n\n\n" << endl;
			start = 1;
		}*/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(program);
		glBindVertexArray(vao);

		setMat4(program, "trans_matrix", glm::mat4(1.0f));
		setVec3(program, "color", glm::vec3(1.0f, 1.0f, 1.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texBoard);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		for (auto cur : chessTable)
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
			if (selected == 0)
			{
				curSel = getPos(xmouse, ymouse);
				press = 0;
				selected = 1;
			}
			else if (selected == 1)
			{
				string query = curSel + getPos(xmouse, ymouse);
				press = 0;
				selected = 0;
				getMove(chessTable, query);
			}
		}

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