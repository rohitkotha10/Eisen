#include <windows.h>
extern "C" {
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}  // force GPU use

#include "Eisen.h"

using namespace Eisen;
using namespace std;

bool firstMouse = true;
int press = 0;

int screen_width = 1280;
int screen_height = 720;
float screen_aspect = (float)screen_width / (float)screen_height;
float fov = 45.0f;

float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;

float pitch = 0;
float yaw = -90;

float deltaTime;
float lastFrame;

glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPos);          // also facing the scene
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));  // camera is in left hand system
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) press = 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
};

void cursor_callback(GLFWwindow* window, double xdpos, double ydpos) {
    float xpos = (float)xdpos;
    float ypos = (float)ydpos;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

class my_app: public OpenGLApp {
    Program myProgram;

    Mesh myPlane;
    Texture wood;

    int start = 0;
    Timer t1;

public:
    void init() {
        info.width = screen_width;
        info.height = screen_height;
        info.MajorVersion = 4;
        info.MinorVersion = 5;
        info.title = "Sandbox";
        info.fullscreen = false;
        info.resize = false;
    }

    void shaderCompile() { myProgram.create("src/vs.shader", "src/fs.shader"); }

    void startup() {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, keyboard_callback);
        glfwSetCursorPosCallback(window, cursor_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        t1.start("First Frame");

        Vertex a0;
        a0.position = glm::vec3(10.0f, -0.5f, 10.0f);
        a0.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        a0.texPos = glm::vec2(10.0f, 0.0f);

        Vertex a1;
        a1.position = glm::vec3(-10.0f, -0.5f, 10.0f);
        a1.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        a1.texPos = glm::vec2(0.0f, 0.0f);

        Vertex a2;
        a2.position = glm::vec3(-10.0f, -0.5f, -10.0f);
        a2.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        a2.texPos = glm::vec2(0.0f, 10.0f);

        Vertex a3;
        a3.position = glm::vec3(10.0f, -0.5f, -10.0f);
        a3.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        a3.texPos = glm::vec2(10.0f, 10.0f);

        vector<Vertex> vertices = {a0, a1, a2, a3};
        vector<GLuint> indices = {0, 1, 2, 0, 2, 3};

        wood.loadTexture("../media/wood.png", "diffuse");
        vector<Texture> textures;
        textures.push_back(wood);
        
        myPlane.createMesh(vertices, indices, textures, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    void render(double currentTime) {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = (float)currentTime;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        myProgram.use();
        glm::mat4 projection = glm::perspective(glm::radians(fov), screen_aspect, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));

        myProgram.setMat4("projection_matrix", projection);
        myProgram.setMat4("view_matrix", view);
        myProgram.setMat4("model_matrix", model);

        myProgram.setVec3("lightPos", glm::vec3(0.0f, 0.3f, 0.0f));
        myProgram.setVec3("viewPos", glm::vec3(cameraPos));

        myPlane.draw(myProgram);

        if (start == 0) {
            t1.display();
            start = 1;
        }

        onKeyUpdate();
    }

    void onKeyUpdate() {
        const float cameraSpeed = 2.5f * deltaTime;  // adjust accordingly

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += cameraSpeed * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraPos += cameraSpeed * cameraUp;

        // roll, q and e
        float rollSpeed = 50.0f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            worldUp = glm::mat3(glm::rotate(glm::mat4(1.0f), -glm::radians(rollSpeed), cameraFront)) * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            worldUp = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rollSpeed), cameraFront)) * cameraUp;

        cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    }

    void shutdown() {
        myProgram.shutdown();
        myPlane.shutdown();
    }
};

int main() {
    my_app* app = new my_app;
    app->run();
    delete app;
    return 0;
}
