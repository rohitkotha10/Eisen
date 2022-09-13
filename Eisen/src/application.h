#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

namespace Eisen
{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	class OpenGLApp
	{
	public:
		OpenGLApp() {};

		virtual ~OpenGLApp() {};

		virtual void run(); //sets up window and app, implemented in application.cpp

		virtual void init() {}; //change window settings and opengl version here
		virtual void shaderCompile() {};
		virtual void startup() {};
		virtual void render(double currentTime) {};
		virtual void shutdown() {};

		struct AppInfo
		{
			std::string title = "Welcome";
			int width = 800;
			int height = 600;
			int MajorVersion = 4;
			int MinorVersion = 5;
			float* color = new float[4] { 0.15f, 0.15f, 0.15f, 0.15f };
			bool fullscreen = 0;
		};

		AppInfo info;
		GLFWwindow* window = NULL;
	};
};

#define START_APP(my_app)        \
int main()                       \
{                                \
	my_app* app = new my_app;    \
	app->run();                  \
	delete app;                  \
	return 0;                    \
}                                \


