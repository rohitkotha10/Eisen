#include <windows.h>
extern "C" {
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}  // force GPU use

#include "renderutil.h"
#include "chess.h"

int xmouse, ymouse;
int press = 0;

int screen_width = 800;
int screen_height = 800;
float screen_aspect = (float)screen_width / (float)screen_height;
float fov = 45.0f;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) press = 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    xmouse = (int)xpos;
    ymouse = (int)ypos;
}

class my_app: public OpenGLApp {
    GLuint program;
    GLuint vao;
    GLuint vertBuffer;
    GLuint indBuffer;

    GLuint texKingWhite;
    GLuint texQueenWhite;
    GLuint texRookWhite;
    GLuint texKnightWhite;
    GLuint texBishopWhite;
    GLuint texPawnWhite;

    GLuint texKingBlack;
    GLuint texQueenBlack;
    GLuint texRookBlack;
    GLuint texKnightBlack;
    GLuint texBishopBlack;
    GLuint texPawnBlack;

    MyGame myChess;
    int startGame;
    int preview;        // is move display on or off
    string curPreview;  // current preview position
    string prevStart;
    string prevEnd;

public:
    void init() {
        info.width = screen_width;
        info.height = screen_height;
        info.MajorVersion = 4;
        info.MinorVersion = 5;
        info.title = "Chess";
        info.color = new float[4] {0.1f, 0.1f, 0.1f, 1.0f};
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
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);

        static const GLfloat vertices[] = {
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f};
        static const int indices[] = {0, 1, 3, 1, 2, 3};

        glGenVertexArrays(1, &vao);

        glGenBuffers(1, &vertBuffer);
        glGenBuffers(1, &indBuffer);

        glGenTextures(1, &texKingWhite);
        glGenTextures(1, &texQueenWhite);
        glGenTextures(1, &texRookWhite);
        glGenTextures(1, &texKnightWhite);
        glGenTextures(1, &texBishopWhite);
        glGenTextures(1, &texPawnWhite);

        glGenTextures(1, &texKingBlack);
        glGenTextures(1, &texQueenBlack);
        glGenTextures(1, &texRookBlack);
        glGenTextures(1, &texKnightBlack);
        glGenTextures(1, &texBishopBlack);
        glGenTextures(1, &texPawnBlack);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        load_texture(texKingWhite, "../media/chess/wking.png");
        load_texture(texQueenWhite, "../media/chess/wqueen.png");
        load_texture(texRookWhite, "../media/chess/wrook.png");
        load_texture(texKnightWhite, "../media/chess/wknight.png");
        load_texture(texBishopWhite, "../media/chess/wbishop.png");
        load_texture(texPawnWhite, "../media/chess/wpawn.png");

        load_texture(texKingBlack, "../media/chess/bking.png");
        load_texture(texQueenBlack, "../media/chess/bqueen.png");
        load_texture(texRookBlack, "../media/chess/brook.png");
        load_texture(texKnightBlack, "../media/chess/bknight.png");
        load_texture(texBishopBlack, "../media/chess/bbishop.png");
        load_texture(texPawnBlack, "../media/chess/bpawn.png");

        cout << "Welcome to CHESS!!!\n";
        cout << "White to Start\n1: ";
    }

    void render(double currentTime) {
        glUseProgram(program);
        glBindVertexArray(vao);

        drawBoard(program, glm::vec4(0.5f, 0.6f, 0.3f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        // if (preview == 1) { getPreview(program, chessTable, curPreview, allowed); }

        if (startGame == 1) {  // Recent History track
            paintBox(program, prevStart);
            paintBox(program, prevEnd);
        }
        drawPieces(program, myChess);

        if (press == 1) {
            onClick();
            press = 0;
        }
    }

    void onClick() {
        string curSel = getPointer(xmouse, ymouse);
        string curPiece = chessTable[curSel];
        if ((curPiece[0] == 'w' && turn == 1) || (curPiece[0] == 'b' && turn == 0)) {
            preview = 1;
            curPreview = curSel;
        } else {
            if (preview == 1) {
                // check if move is valid and proceed
                // in the end put preview 0
                string query = curPreview + curSel;

                if (processMove(chessTable, query, allowed) == 0) {
                    startGame = 1;
                    cout << query << ' ';
                    if (turn == 0) {
                        counter++;
                        cout << endl;
                        cout << counter << ": ";
                    }
                    turn = 1 - turn;
                    prevStart = curPreview;
                    prevEnd = curSel;
                }
            }
            preview = 0;
        }
    }

    void shutdown() {
        glDeleteProgram(program);
        glDeleteBuffers(1, &vertBuffer);
        glDeleteBuffers(1, &indBuffer);
        glDeleteVertexArrays(1, &vao);

        glDeleteTextures(1, &texKingWhite);
        glDeleteTextures(1, &texQueenWhite);
        glDeleteTextures(1, &texRookWhite);
        glDeleteTextures(1, &texKnightWhite);
        glDeleteTextures(1, &texBishopWhite);
        glDeleteTextures(1, &texPawnWhite);

        glDeleteTextures(1, &texKingBlack);
        glDeleteTextures(1, &texQueenBlack);
        glDeleteTextures(1, &texRookBlack);
        glDeleteTextures(1, &texKnightBlack);
        glDeleteTextures(1, &texBishopBlack);
        glDeleteTextures(1, &texPawnBlack);
    }
};

int main() {
    my_app* app = new my_app;
    app->run();
    delete app;
    return 0;
}
