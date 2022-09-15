#include "Application.h"

namespace Eisen
{
	void OpenGLApp::run()
	{
		init();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.MajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.MinorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//create window from here

		window = glfwCreateWindow(info.width,
			info.height,
			info.title.c_str(),
			info.fullscreen ? glfwGetPrimaryMonitor() : NULL,
			NULL);

		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		//starts app from here
		shaderCompile();
		startup();

		while (!glfwWindowShouldClose(window))
		{

			glClearBufferfv(GL_COLOR, 0, info.color);

			render(glfwGetTime());

			glfwPollEvents();
			glfwSwapInterval(1);
			glfwSwapBuffers(window);
		}

		shutdown();

		glfwDestroyWindow(window);
		glfwTerminate();
	};
}