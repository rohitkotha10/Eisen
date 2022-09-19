#include "Eisen.h"

#include <vector>

using namespace std;
using namespace Eisen;

int getCnt(int i, int j, vector<vector<char>> arr, int n, int m)
{
	if (i >= n || j >= m)
		return -1;

	int cnt = 0;
	if (i > 0 && j < m - 1 && arr[i - 1][j + 1] == '1')
		cnt++;
	if (i > 0 && arr[i - 1][j] == '1')
		cnt++;
	if (i > 0 && j > 0 && arr[i - 1][j - 1] == '1')
		cnt++;
	if (j < m - 1 && arr[i][j + 1] == '1')
		cnt++;
	if (j > 0 && arr[i][j - 1] == '1')
		cnt++;
	if (i < n - 1 && j < m - 1 && arr[i + 1][j + 1] == '1')
		cnt++;
	if (i < n - 1 && arr[i + 1][j] == '1')
		cnt++;
	if (i < n - 1 && j > 0 && arr[i + 1][j - 1] == '1')
		cnt++;
	return cnt;
}

vector<vector<char>> conway_life(vector<vector<char>> arr, int n, int m)
{
	vector<vector<char>> ans(n, vector<char>(m));

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			int nei = getCnt(i, j, arr, n, m);
			if (arr[i][j] == '0')
			{
				if (nei == 3)
					ans[i][j] = '1';
				else
					ans[i][j] = '0';
			}
			else if (arr[i][j] == '1')
			{
				if (nei == 2 || nei == 3)
					ans[i][j] = '1';
				else
					ans[i][j] = '0';
			}
		}
	}
	return ans;

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

class my_app : public OpenGLApp
{
	GLuint program;
	GLuint vao;
	GLuint vertBuffer;
	GLuint indBuffer;
	vector<vector<char>> curConway;
	float startTime;
public:
	void init()
	{
		info.width = 800;
		info.height = 800;
		info.MajorVersion = 4;
		info.MinorVersion = 5;
		info.title = "Conway Game of Life";
		info.color = new float[4] {0.0f, 0.0f, 0.0f, 0.0f};
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
			1.0f, 1.0f,
			1.0f, -1.0f,
			-1.0f, -1.0f,
			-1.0f, 1.0f
		};
		static const int indices[] =
		{
			0, 1, 3,
			1, 2, 3
		};
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vertBuffer);
		glGenBuffers(1, &indBuffer);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		int n = 64;
		int m = 64;
		vector<vector<char>> temp(n, vector<char>(m));
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
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
		curConway = temp;
		startTime = 2;
	}

	void render(double currentTime)
	{
		glUseProgram(program);
		glBindVertexArray(vao);
		int time = 0;
		setMat4(program, "trans_matrix", glm::mat4(1.0f));
		setVec3(program, "color", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glm::mat4 curMat = glm::mat4(1.0f);

		int n = (int)curConway.size();
		int m = (int)curConway[0].size();

		float scale = 1.0f / max(n, m);
		float incr = scale;
		curConway = conway_life(curConway, n, m);

		/*float stepTime = 1;
		if (currentTime > startTime)
		{
			curConway = conway_life(curConway, n, m);
			startTime += stepTime;
		}*/

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (curConway[i][j] == '0')
				{
					float xshift = -1.0f + (2 * j + 1) * incr;
					float yshift = 1.0f - (2 * i + 1) * incr;
					curMat = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
					curMat = glm::scale(curMat, glm::vec3(scale));

					setMat4(program, "trans_matrix", curMat);
					setVec3(program, "color", glm::vec3(0.0f, 0.0f, 0.0f));
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
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