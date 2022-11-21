#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

namespace Eisen {
    class OpenGLApp {
    public:
        OpenGLApp() {};

        virtual ~OpenGLApp() {};

        virtual void run();  // sets up window and app, implemented in application.cpp

        virtual void init() {};  // change window settings and opengl version here
        virtual void shaderCompile() {};
        virtual void startup() {};
        virtual void render(double currentTime) {};
        virtual void shutdown() {};

        struct AppInfo {
            std::string title = "Welcome";
            int width = 800;
            int height = 600;
            int MajorVersion = 4;
            int MinorVersion = 5;
            bool fullscreen = 0;
            bool resize = 1;
        };

        AppInfo info;
        GLFWwindow* window = NULL;
    };
};  // namespace Eisen
