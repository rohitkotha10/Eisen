#include "application.h"

using namespace std;

namespace Eisen {
    void OpenGLApp::run() {
        glfwInit();

        init();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.MajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.MinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, info.resize);

        // create window from here

        window = glfwCreateWindow(
            info.width, info.height, info.title.c_str(), info.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

        if (window == NULL) {
            cout << "Failed to create GLFW window" << endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            cout << "Failed to initialize GLAD" << endl;
            return;
        }

        cout << glGetString(GL_VENDOR) << endl;
        cout << glGetString(GL_VERSION) << endl;
        cout << glGetString(GL_RENDERER) << endl << endl;

        // starts app from here
        shaderCompile();
        startup();

        while (!glfwWindowShouldClose(window)) {
            render(glfwGetTime());

            glfwPollEvents();
            glfwSwapInterval(1);
            glfwSwapBuffers(window);
        }

        shutdown();

        glfwDestroyWindow(window);
        glfwTerminate();
    };
}  // namespace Eisen
