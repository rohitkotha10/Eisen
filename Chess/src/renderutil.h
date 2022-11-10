#pragma once
#include "Eisen.h"
#include "chess.h"

#include <map>

using namespace Eisen;

glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPos);          // also facing the scene
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));  // camera is in left hand system
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));

GLuint getPieceTex(char piece) {
    return 2;
}

void drawPieces(GLuint& program, MyGame myChess) {
    for (int rank = 8; rank >= 0; rank--) {
        for (int file = 1; file <= 8; file++) {
            string place = placeToString(rank + 8 * (file - 1));
            char piece = myChess.getPositonStatus(place);
            GLuint pieceTex = getPieceTex(piece);
            if (piece != 'e') { renderPiece(program, pieceTex, place); }
        }
    }
}

void drawBoard(GLuint& program, glm::vec4 colorBlack, glm::vec4 colorWhite) {
    glm::mat4 model = glm::mat4(1.0f);
    int n = 10;

    float scale = 1.0f / n;
    float incr = scale;
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            if ((i + j) % 2 == 0)
                setVec4(program, "color", colorBlack);
            else
                setVec4(program, "color", colorWhite);
            float xshift = -1.0f + (2 * i + 1) * incr;
            float yshift = -1.0f + (2 * j + 1) * incr;
            model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
            model = glm::scale(model, glm::vec3(scale));

            setInt(program, "choice", 0);
            setInt(program, "outChoice", 0);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
            glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
            glm::mat4 mvp = projection * view * model;

            setMat4(program, "mvp_matrix", mvp);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}

string getPointer(int xpos, int ypos)  // for mouse picking
{
    int file = 'a' - 1;  // 0 in alphabet
    int rank = 0;
    file += xpos / 80;
    rank += ypos / 80;
    rank = 8 - rank;
    rank += (int)'1';
    string s;
    s.push_back((char)file);
    s.push_back((char)rank);
    return s;
}

void paintMoveOptions(GLuint& program, string place, int occupy) {
    // for occupy 0 empty, 2 for enemy, 1 for comrade
    int i = place[0] - ('a' - 1);
    int j = place[1] - ('1' - 1);
    glm::mat4 model = glm::mat4(1.0f);
    int n = 10;

    float scale = 1.0f / n;
    float incr = scale;

    float xshift = -1.0f + (2 * i + 1) * incr;
    float yshift = -1.0f + (2 * j + 1) * incr;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
    model = glm::scale(model, glm::vec3(scale));

    setInt(program, "choice", 2);
    setInt(program, "outChoice", 2);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * model;

    setMat4(program, "mvp_matrix", mvp);
    setInt(program, "occupy", occupy);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void paintBox(GLuint& program, string place) {
    int i = place[0] - ('a' - 1);
    int j = place[1] - ('1' - 1);
    glm::mat4 model = glm::mat4(1.0f);
    int n = 10;

    float scale = 1.0f / n;
    float incr = scale;

    float xshift = -1.0f + (2 * i + 1) * incr;
    float yshift = -1.0f + (2 * j + 1) * incr;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
    model = glm::scale(model, glm::vec3(scale));

    setInt(program, "choice", 1);
    setInt(program, "outChoice", 1);
    setVec4(program, "color", glm::vec4(1.0f, 1.0f, 0.0f, 0.6f));
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * model;

    setMat4(program, "mvp_matrix", mvp);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

vector<string> getPositions(string player, string place) {
    vector<string> ans;
    int file = place[0] - 'a' + 1;
    int rank = place[1] - '1' + 1;
    if (player[1] == 'p') {
        if (player[0] == 'w') {
            if (rank + 1 <= 8) {
                string temp;
                temp.push_back(file + 'a' - 1);
                temp.push_back(rank + 1 + '1' - 1);
                ans.push_back(temp);
            }
            if (rank + 2 <= 8 && rank == 2) {
                string temp;
                temp.push_back(file + 'a' - 1);
                temp.push_back(rank + 2 + '1' - 1);
                ans.push_back(temp);
            }
        } else if (player[0] == 'b') {
            if (rank - 1 >= 0) {
                string temp;
                temp.push_back(file + 'a' - 1);
                temp.push_back(rank - 1 + '1' - 1);
                ans.push_back(temp);
            }
            if (rank - 2 >= 0 && rank == 7) {
                string temp;
                temp.push_back(file + 'a' - 1);
                temp.push_back(rank - 2 + '1' - 1);
                ans.push_back(temp);
            }
        }
    } else if (player[1] == 'r') {
    } else if (player[1] == 'n') {
    } else if (player[1] == 'b') {
    } else if (player[1] == 'q') {
    } else if (player[1] == 'k') {
    }
    return ans;
}

void getPreview(GLuint& program, map<string, string>& chessTable, string place, vector<string>& allowed) {
    paintBox(program, place);
    allowed.clear();
    string player = chessTable[place];
    vector<string> positions = getPositions(player, place);
    char curColor = chessTable[place][0];
    // for occupy 0 empty, 2 for enemy, 1 for comrade
    for (int i = 0; i < positions.size(); i++) {
        if (chessTable[positions[i]].length() != 2) {
            paintMoveOptions(program, positions[i], 0);
            allowed.push_back(positions[i]);
        } else if (chessTable[positions[i]][0] == curColor) {
            paintMoveOptions(program, positions[i], 1);
        } else if (chessTable[positions[i]][0] != curColor) {
            paintMoveOptions(program, positions[i], 2);
            allowed.push_back(positions[i]);
        }
    }
}

int processMove(map<string, string>& table, string move, vector<string>& allowed) {
    string fir = move.substr(0, 2);
    string sec = move.substr(2, 2);
    int flag = 0;
    for (int i = 0; i < allowed.size(); i++) {
        if (allowed[i] == sec) {
            flag = 1;
            break;
        }
    }
    if (flag == 0) return -1;
    if (table[fir] == "empty" || table[fir] == "" || table[fir] == table[sec])  // check valid here
        return -1;
    table[sec] = table[fir];
    table[fir] = "empty";
    return 0;
}

void renderPiece(GLuint& program, GLuint& pieceTexture, string place) {
    int i = place[0] - ('a' - 1);
    int j = place[1] - ('1' - 1);
    glm::mat4 model = glm::mat4(1.0f);
    int n = 10;

    float scale = 1.0f / n;
    float incr = scale;

    float xshift = -1.0f + (2 * i + 1) * incr;
    float yshift = -1.0f + (2 * j + 1) * incr;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(xshift, yshift, 0.0f));
    model = glm::scale(model, glm::vec3(scale));

    setInt(program, "choice", 3);
    setInt(program, "outChoice", 3);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * model;

    setMat4(program, "mvp_matrix", mvp);
    setVec4(program, "color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pieceTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void load_texture(GLuint& tex, string path) {
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = 3;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
}
