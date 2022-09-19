#include "Eisen.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>;

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

int xmouse, ymouse;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	xmouse = xpos;
	ymouse = ypos;
}

class my_app : public OpenGLApp
{
	GLuint program;
	GLuint vao;
	GLuint vertBuffer;
	GLuint indBuffer;
	GLuint tex0;
	GLuint tex1;
public:
	void init()
	{
		info.width = 1280;
		info.height = 720;
		info.MajorVersion = 4;
		info.MinorVersion = 5;
		info.title = "Triangle";
		info.color = new float[4] {0.1f, 0.1f, 0.1f, 0.1f};
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
		glfwSetCursorPosCallback(window, mouse_callback);

		static const GLfloat vertices[] =
		{
			0.5f, 0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 1.0f
		};
		static const int indices[] =
		{
			0, 1, 3,
			1, 2, 3
		};
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vertBuffer);
		glGenBuffers(1, &indBuffer);

		glGenTextures(1, &tex0);
		glGenTextures(1, &tex1);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindTexture(GL_TEXTURE_2D, tex0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char* data = stbi_load("res/media/smile.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, tex1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/media/wood.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
		std::cout << xmouse << ' ' << ymouse << ' ' << std::endl;
		glUseProgram(program);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);

		setInt(program, "tex0", 0);
		setInt(program, "tex1", 1);
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