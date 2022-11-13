#include "chess.h"

using namespace std;

bool isWhite(char piece) {
    if (piece == 'P' || piece == 'R' || piece == 'N' || piece == 'B' || piece == 'Q' || piece == 'K') return true;
}

string placeToString(int place) {
    char file = ((place - 1) / 8) + 1 + 'a' - 1;
    char rank = ((place - 1) % 8 + 1) + '1' - 1;
    string temp = {file, rank};
    return temp;
}

int placeToInt(string place) {
    int file = place[0] - ('a' - 1);
    int rank = place[1] - ('1' - 1);
    return rank + 8 * (file - 1);
}

string getPointer(int xpos, int ypos, int scrw, int scrh)  // for mouse picking
{
    float temp = (float)xpos / ((float)scrw / 10.0f);
    int file = temp;

    temp = (float)ypos / ((float)scrw / 10.0f);
    int rank = 10 - temp;

    if (rank < 1 || file < 1 || rank > 8 || file > 8) {
        cout << "Invalid Select";
        return "BAD";
    }

    return placeToString(rank + 8 * (file - 1));
}

ChessTable::ChessTable() {
    arr.resize(65);
    for (int i = 1; i <= 64; i++) { arr[i] = 'e'; }
}

void ChessTable::set(string place, char piece) {
    int cur = placeToInt(place);
    arr[cur] = piece;
}

char ChessTable::positonStatus(string position) {
    int cur = placeToInt(position);
    return arr[cur];
}

char MyGame::getPositonStatus(string position) {
    return chessTable.positonStatus(position);
}

void MyGame::setPosition(string place, char piece) {
    chessTable.set(place, piece);
}

void MyGame::importFEN(string fen) {
    int option = 0;
    // 0 positions
    // 1 turn
    // 2 castling
    // 3 en Pessant
    // 4 halfmoves
    // 5 fullmoves
    int curRank = 8;
    int curFile = 1;
    for (int i = 0; i < fen.length(); i++) {
        if (option == 0) {
            if (isdigit(fen[i])) {
                int incr = fen[i] - '0';
                curFile += incr;
            } else if (fen[i] == '/') {
                curRank--;
                curFile = 1;
            } else {
                string cur = placeToString(curRank + 8 * (curFile - 1));
                // cout << i << ' ' << cur << endl;
                setPosition(cur, fen[i]);
                curFile++;
            }

            if (curRank == 1 && curFile == 9) {
                option = 1;
                i++;  // skip space
            }
        }

        else if (option == 1) {
            turn = fen[i];
            option = 2;
            i++;  // skip space
        } else if (option == 2) {
            if (fen[i] == '-') {
                castling = "-";
                i++;  // skip space
            } else {
                string temp;
                while (isalpha(fen[i])) {  // implicit space skip
                    temp.push_back(fen[i]);
                    i++;
                }
                castling = temp;
            }
            option = 3;
        } else if (option == 3) {
            if (fen[i] == '-') {
                enPessant = "-";
                i++;  // skip space
            } else {
                string temp = {fen[i], fen[i + 1]};
                enPessant = temp;
                i += 2;  // skip space
            }
            option = 4;
        } else if (option == 4) {
            string temp;
            while (isdigit(fen[i])) {  // implicit space skip
                temp.push_back(fen[i]);
                i++;
            }
            halfMoves = stoi(temp);
            option = 5;
        } else if (option == 5) {
            string temp;
            while (isdigit(fen[i])) {  // implicit space skip
                temp.push_back(fen[i]);
                i++;
            }
            fullMoves = stoi(temp);
            option = 5;
        }
    }
}

string MyGame::getFEN() {
    string fen;
    char temp = 0;
    for (int rank = 8; rank >= 0; rank--) {
        for (int file = 1; file <= 8; file++) {
            string cur = placeToString(rank + 8 * (file - 1));
            char piece = getPositonStatus(cur);
            if (piece == 'e') {
                temp++;
            } else {
                if (temp != 0) {
                    temp = temp + '0';
                    fen.push_back(temp);
                    temp = 0;
                }
                fen.push_back(piece);
            }
        }
        fen.push_back('/');
    }

    fen += " ";
    fen.push_back(turn);
    fen += " ";
    fen += castling;
    fen += " ";
    fen += enPessant;
    fen += " ";
    fen += to_string(halfMoves);
    fen += " ";
    fen += to_string(fullMoves);

    return fen;
}

string MyGame::getNotation() {
    return "nothing";
}

vector<pair<string, int>> MyGame::getPawnMoves(string place) {
}
vector<pair<string, int>> MyGame::getRookMoves(string place) {
}
vector<pair<string, int>> MyGame::getKnightMoves(string place) {
}
vector<pair<string, int>> MyGame::getBishopMoves(string place) {
}
vector<pair<string, int>> MyGame::getQueenMoves(string place) {
}
vector<pair<string, int>> MyGame::getKingMoves(string place) {
}

vector<pair<string, int>> MyGame::getPieceMoves(string place, char piece) {
    if (piece == 'p' || piece == 'P') return getPawnMoves(place);
    if (piece == 'r' || piece == 'R') return getRookMoves(place);
    if (piece == 'n' || piece == 'N') return getKnightMoves(place);
    if (piece == 'b' || piece == 'B') return getBishopMoves(place);
    if (piece == 'q' || piece == 'Q') return getQueenMoves(place);
    if (piece == 'k' || piece == 'K') return getKingMoves(place);
}
int MyGame::processMove(string fir, string sec) {
    // 0 process move
    // 1 game over show winner
    // 2 draw
    // -1 reject move
    if (fir == "BAD" || sec == "BAD") return -1;
    char piece = getPositonStatus(fir);
    bool color = isWhite(piece);
    if ((color == true && turn == 'b') || (color == false && turn == 'w') || piece == 'e') return -1;
    vector<pair<string, int>> nextMoves = getPieceMoves(fir, piece);

    for (auto i: nextMoves) {
        if (i.first == sec) {
            if (i.second == 0) //comrade
                return -1;
            else {}
        }
    }
    return -1;
}
