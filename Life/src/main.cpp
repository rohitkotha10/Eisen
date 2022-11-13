#include <windows.h>
extern "C" {
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}  // force GPU use
#include <vector>

#include "Eisen.h"

using namespace std;
using namespace Eisen;

int screen_width = 800;
int screen_height = 800;
float screen_aspect = (float)screen_width / (float)screen_height;
float fov = 45.0f;

int getCnt(int i, int j, vector<vector<char>> arr, int n, int m) {
    if (i >= n || j >= m) return -1;

    int cnt = 0;
    if (i > 0 && j < m - 1 && arr[i - 1][j + 1] == '1') cnt++;
    if (i > 0 && arr[i - 1][j] == '1') cnt++;
    if (i > 0 && j > 0 && arr[i - 1][j - 1] == '1') cnt++;
    if (j < m - 1 && arr[i][j + 1] == '1') cnt++;
    if (j > 0 && arr[i][j - 1] == '1') cnt++;
    if (i < n - 1 && j < m - 1 && arr[i + 1][j + 1] == '1') cnt++;
    if (i < n - 1 && arr[i + 1][j] == '1') cnt++;
    if (i < n - 1 && j > 0 && arr[i + 1][j - 1] == '1') cnt++;
    return cnt;
}

vector<vector<char>> conway_life(vector<vector<char>> arr, int n, int m) {
    vector<vector<char>> ans(n, vector<char>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int nei = getCnt(i, j, arr, n, m);
            if (arr[i][j] == '0') {
                if (nei == 3)
                    ans[i][j] = '1';
                else
                    ans[i][j] = '0';
            } else if (arr[i][j] == '1') {
                if (nei == 2 || nei == 3)
                    ans[i][j] = '1';
                else
                    ans[i][j] = '0';
            }
        }
    }
    return ans;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
};

class my_app: public OpenGLApp {
    GLuint program;
    QuadRenderer myRenderer;

    vector<vector<char>> curConway;
    float startTime;

public:
    void init() {
        info.width = screen_width;
        info.height = screen_height;
        info.MajorVersion = 4;
        info.MinorVersion = 5;
        info.title = "Conway Game of Life";
        info.color = new float[4] {0.0f, 0.0f, 0.0f, 0.0f};
        info.fullscreen = false;
        info.resize = false;
    }

    void shaderCompile() {
        // vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        string trial = parse("src/vs.shader");
        const GLchar* vsSource = trial.c_str();
        glShaderSource(vs, 1, &vsSource, NULL);
        glCompileShader(vs);

        int success;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vs, 512, NULL, infoLog);
            cout << "Vertex Shader Error\n" << infoLog << endl;
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
            cout << "Frag Shader Error\n" << infoLog << endl;
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
            cout << "Shader Linking Error\n" << infoLog << endl;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void startup() {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, keyboard_callback);

        myRenderer.init(100);

        int n = 50;
        int m = 50;
        vector<vector<char>> temp(n, vector<char>(m));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i % 2 == 0) {
                    if (j % 2 == 0)
                        temp[i][j] = '0';
                    else
                        temp[i][j] = '1';
                } else {
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

    void render(double currentTime) {
        glUseProgram(program);

        myRenderer.beginBatch();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 100.0f);
        setMat4(program, "view_matrix", view);
        setMat4(program, "projection_matrix", projection);
        setMat4(program, "model_matrix", model);

        int n = (int)curConway.size();
        int m = (int)curConway[0].size();

        float stepTime = 1;
        if (currentTime > startTime) {
            curConway = conway_life(curConway, n, m);
            startTime += stepTime;
        }
        // curConway = conway_life(curConway, n, m);
        float side = 2.0f / n;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (curConway[i][j] == '1') {
                    myRenderer.drawQuadColor(program, Quad(-1.0f + i * side, -1.0f + j * side, side), glm::vec4(1.0f));
                }
            }
        }

        myRenderer.endBatch();
        myRenderer.flush(program);
    }

    void shutdown() {
        glDeleteProgram(program);
        myRenderer.shutdown();
    }
};

int main() {
    my_app* app = new my_app;
    app->run();
    delete app;
    return 0;
}
