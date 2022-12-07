#include <windows.h>
extern "C" {
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}  // force GPU use

#include "Eisen.h"
#include "chess.h"

using namespace Eisen;
using namespace std;

int xmouse, ymouse;
int press = 0;

int screen_width = 800;
int screen_height = 800;
float screen_aspect = (float)screen_width / (float)screen_height;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) press = 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
};

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    xmouse = (int)xpos;
    ymouse = (int)ypos;
}

class my_app: public OpenGLApp {
    Program program;
    QuadRenderer myRenderer;

    Texture pTex;
    Texture rTex;
    Texture nTex;
    Texture bTex;
    Texture qTex;
    Texture kTex;

    Texture PTex;
    Texture RTex;
    Texture NTex;
    Texture BTex;
    Texture QTex;
    Texture KTex;

    MyGame myChess;
    string fir;
    string sec;

    string hisFir;
    string hisSec;
    bool preview;
    vector<pair<string, int>> previewTable;

public:
    void init() {
        info.width = screen_width;
        info.height = screen_height;
        info.MajorVersion = 4;
        info.MinorVersion = 5;
        info.title = "Chess";
        info.fullscreen = false;
        info.resize = false;
    }

    void shaderCompile() { program.create("src/vs.shader", "src/fs.shader"); }

    void startup() {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, keyboard_callback);
        glfwSetCursorPosCallback(window, cursor_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);

        myRenderer.init(1000);
        preview = 0;

        PTex.loadTexture("../media/chess/wpawn.png", "chess");
        RTex.loadTexture("../media/chess/wrook.png", "chess");
        NTex.loadTexture("../media/chess/wknight.png", "chess");
        BTex.loadTexture("../media/chess/wbishop.png", "chess");
        QTex.loadTexture("../media/chess/wqueen.png", "chess");
        KTex.loadTexture("../media/chess/wking.png", "chess");

        pTex.loadTexture("../media/chess/bpawn.png", "chess");
        rTex.loadTexture("../media/chess/brook.png", "chess");
        nTex.loadTexture("../media/chess/bknight.png", "chess");
        bTex.loadTexture("../media/chess/bbishop.png", "chess");
        qTex.loadTexture("../media/chess/bqueen.png", "chess");
        kTex.loadTexture("../media/chess/bking.png", "chess");
        myChess.importFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        // myChess.importFEN("r2q3k/1p1nb1Q1/p6P/4p2N/2p1P3/2N5/PPP2P2/R1B1K3 b Q - 2 22");
    }

    void render(double currentTime) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();

        myRenderer.beginBatch();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 100.0f);
        program.setMat4("view_matrix", view);
        program.setMat4("projection_matrix", projection);
        program.setMat4("model_matrix", model);

        glm::vec4 colorBlack(0.5f, 0.6f, 0.3f, 1.0f);
        glm::vec4 colorWhite(1.0f, 1.0f, 1.0f, 1.0f);

        float side = 0.2f;

        // board draw
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                if ((i + j) % 2 == 0) {
                    myRenderer.drawQuadColor(program, Quad(-1.0f + i * side, -1.0f + j * side, side), colorBlack);
                } else {
                    myRenderer.drawQuadColor(program, Quad(-1.0f + i * side, -1.0f + j * side, side), colorWhite);
                }
            }
        }

        // history
        if (!(myChess.getTurn() == 'w' && myChess.getFullMoves() == 1)) {
            int pos = placeToInt(hisFir);
            int file = ((pos - 1) / 8) + 1;
            int rank = ((pos - 1) % 8) + 1;

            myRenderer.drawQuadColor(
                program, Quad(-1.0f + file * side, -1.0f + rank * side, side), glm::vec4(1.0f, 1.0f, 0.0f, 0.6f));

            pos = placeToInt(hisSec);
            file = ((pos - 1) / 8) + 1;
            rank = ((pos - 1) % 8) + 1;

            myRenderer.drawQuadColor(
                program, Quad(-1.0f + file * side, -1.0f + rank * side, side), glm::vec4(1.0f, 1.0f, 0.0f, 0.6f));
        }

        // previews
        if (preview) {
            int pos = placeToInt(fir);
            int file = ((pos - 1) / 8) + 1;
            int rank = ((pos - 1) % 8) + 1;
            myRenderer.drawQuadColor(
                program, Quad(-1.0f + file * side, -1.0f + rank * side, side), glm::vec4(1.0f, 1.0f, 0.0f, 0.6f));

            for (auto i: previewTable) {
                int tpos = placeToInt(i.first);
                int tfile = ((tpos - 1) / 8) + 1;
                int trank = ((tpos - 1) % 8) + 1;
                if (i.second == 0) {
                    myRenderer.drawCircle(
                        program,
                        Quad(-1.0f + tfile * side, -1.0f + trank * side, side),
                        0.3f,
                        0.0f,
                        glm::vec4(0.0f, 0.0f, 0.0f, 0.1f));
                } else {
                    myRenderer.drawCircle(
                        program,
                        Quad(-1.0f + tfile * side, -1.0f + trank * side, side),
                        0.95f,
                        0.8f,
                        glm::vec4(0.0f, 0.0f, 0.0f, 0.1f));
                }
            }
        }

        // pieces draw
        for (int rank = 8; rank >= 1; rank--) {
            for (int file = 1; file <= 8; file++) {
                string pos = placeToString(rank + 8 * (file - 1));
                char piece = myChess.getPositonStatus(pos);
                if (piece == 'e')
                    continue;
                else if (piece == 'p')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), pTex);
                else if (piece == 'r')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), rTex);
                else if (piece == 'n')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), nTex);
                else if (piece == 'b')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), bTex);
                else if (piece == 'q')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), qTex);
                else if (piece == 'k')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), kTex);

                else if (piece == 'P')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), PTex);
                else if (piece == 'R')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), RTex);
                else if (piece == 'N')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), NTex);
                else if (piece == 'B')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), BTex);
                else if (piece == 'Q')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), QTex);
                else if (piece == 'K')
                    myRenderer.drawQuadTexture(program, Quad(-1.0f + file * side, -1.0f + rank * side, side), KTex);
            }
        }

        myRenderer.endBatch();
        myRenderer.flush(program);

        if (press == 1) {
            onClick();
            press = 0;
        }
    }

    void onClick() {
        string cur = getPointer(xmouse, ymouse, screen_width, screen_height);
        if (cur == "BAD") return;
        if (!preview) {
            if (myChess.getTurn() != getColor(myChess.getPositonStatus(cur)) || myChess.getPositonStatus(cur) == 'e')
                return;
            fir = cur;
            preview = true;
            previewTable = myChess.getPieceMoves(fir);
        } else {
            if (myChess.getTurn() == getColor(myChess.getPositonStatus(cur))) {
                fir = cur;
                preview = true;
                previewTable = myChess.getPieceMoves(fir);
                return;
            }
            sec = cur;
            preview = false;
            int status = myChess.processMove(fir, sec);
            if (status == 0) {
                hisFir = fir;
                hisSec = sec;
            }
        }
    }

    void shutdown() {
        program.shutdown();
        myRenderer.shutdown();
    }
};

int main() {
    my_app* app = new my_app;
    app->run();
    delete app;
    return 0;
}
