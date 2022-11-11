// #include <windows.h>
// extern "C" {
//_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// }  // force GPU use

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
    GLuint program;

    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    Model ourModel;
    Mesh myMesh;
    Mesh anotherMesh;
    Renderer myRenderer;

    int start = 0;
    Timer t1;

public:
    void init() {
        info.width = screen_width;
        info.height = screen_height;
        info.MajorVersion = 4;
        info.MinorVersion = 5;
        info.title = "Sandbox";
        info.color = new float[4] {0.1f, 0.1f, 0.1f, 0.1f};
        info.fullscreen = false;
        info.resize = false;
    }

    void shaderCompile() {
        // vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        std::string trial = parse("src/vs.shader");
        const GLchar* vsSource = trial.c_str();
        glShaderSource(vs, 1, &vsSource, NULL);
        glCompileShader(vs);

        int success;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vs, 512, NULL, infoLog);
            std::cout << "Vertex Shader Error\n" << infoLog << std::endl;
        }

        // frag shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        trial = parse("src/fs.shader");
        const GLchar* fsSource = trial.c_str();
        glShaderSource(fs, 1, &fsSource, NULL);
        glCompileShader(fs);

        success = 0;
        glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
        if (!success) {
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

    void startup() {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, keyboard_callback);
        glfwSetCursorPosCallback(window, cursor_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        t1.start("Startup");
        ourModel.loadModel("../media/backpack/backpack.obj");

        Texture myTex = loadMeshTexture("../media/wood.jpg", "diffuse");
        float texFloat = (float)myTex.id;

        vector<Vertex> myVertices;
        Vertex a0;
        a0.position = glm::vec3(-0.5f, -0.5f, 0.0f);
        a0.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a0.texPos = glm::vec2(0.0f, 0.0f);
        a0.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a0.texIndex = texFloat;

        Vertex a1;
        a1.position = glm::vec3(-0.5f, 0.5f, 0.0f);
        a1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a1.texPos = glm::vec2(0.0f, 1.0f);
        a1.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a1.texIndex = texFloat;

        Vertex a2;
        a2.position = glm::vec3(0.5f, 0.5f, 0.0f);
        a2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a2.texPos = glm::vec2(1.0f, 1.0f);
        a2.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a2.texIndex = texFloat;

        Vertex a3;
        a3.position = glm::vec3(0.5f, -0.5f, 0.0f);
        a3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a3.texPos = glm::vec2(1.0f, 0.0f);
        a3.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a3.texIndex = texFloat;

        myVertices.push_back(a0);
        myVertices.push_back(a1);
        myVertices.push_back(a2);
        myVertices.push_back(a3);

        vector<GLuint> myIndices = {0, 1, 2, 2, 3, 0};

        myMesh.createMesh(myVertices, myIndices, myTex);

        Texture anotherTex = loadMeshTexture("../media/smile.jpg", "diffuse");
        float atexFloat = (float)anotherTex.id;

        vector<Vertex> anotherVertices;
        Vertex b0;
        b0.position = glm::vec3(-0.5f, -0.5f, -1.0f);
        b0.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        b0.texPos = glm::vec2(0.0f, 0.0f);
        b0.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        b0.texIndex = atexFloat;

        Vertex b1;
        b1.position = glm::vec3(-0.5f, 0.5f, -1.0f);
        b1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        b1.texPos = glm::vec2(0.0f, 1.0f);
        b1.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        b1.texIndex = atexFloat;

        Vertex b2;
        b2.position = glm::vec3(0.5f, 0.5f, -1.0f);
        b2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        b2.texPos = glm::vec2(1.0f, 1.0f);
        b2.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        b2.texIndex = atexFloat;

        Vertex b3;
        b3.position = glm::vec3(0.5f, -0.5f, -1.0f);
        b3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        b3.texPos = glm::vec2(1.0f, 0.0f);
        b3.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        b3.texIndex = atexFloat;

        anotherVertices.push_back(b0);
        anotherVertices.push_back(b1);
        anotherVertices.push_back(b2);
        anotherVertices.push_back(b3);

        vector<GLuint> anotherIndices = {0, 1, 2, 2, 3, 0};

        anotherMesh.createMesh(anotherVertices, anotherIndices, anotherTex);

        myRenderer.init();

        glUseProgram(program);
        int samplers[16];
        for (int i = 0; i < 16; i++) { samplers[i] = i; }
        setIntArray(program, "diffuse", 16, samplers);
    }

    void render(double currentTime) {
        float currentFrame = (float)currentTime;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glUseProgram(program);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), screen_aspect, 0.1f, 100.0f);
        setMat4(program, "view_matrix", view);
        setMat4(program, "projection_matrix", projection);
        setMat4(program, "model_matrix", model);

        //myRenderer.drawMesh(program, myMesh);
        //myRenderer.drawMesh(program, anotherMesh);
        myRenderer.drawModel(program, ourModel);
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
    }

    void shutdown() { glDeleteProgram(program); }
};

int main() {
    my_app* app = new my_app;
    app->run();
    delete app;
    return 0;
}
