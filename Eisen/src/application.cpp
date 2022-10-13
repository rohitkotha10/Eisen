#include "Application.h"

namespace Eisen
{
	void OpenGLApp::run()
	{
		glfwInit();

		init();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.MajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.MinorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, info.resize);


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

		std::cout << glGetString(GL_VENDOR) << std::endl;
		std::cout << glGetString(GL_VERSION) << std::endl;
		std::cout << glGetString(GL_RENDERER) << std::endl << std::endl;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		//starts app from here
		shaderCompile();
		startup();

		while (!glfwWindowShouldClose(window))
		{
			glClearColor(info.color[0], info.color[1], info.color[2], info.color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

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