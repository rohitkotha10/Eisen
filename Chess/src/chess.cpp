#include "chess.h"

using namespace std;

char getColor(char piece) {
    if (piece == 'e') return 'e';
    if (piece == 'P' || piece == 'R' || piece == 'N' || piece == 'B' || piece == 'Q' || piece == 'K')
        return 'w';
    else
        return 'b';
}

string placeToString(int place) {
    char file = ((place - 1) / 8) + 1 + 'a' - 1;
    char rank = ((place - 1) % 8) + 1 + '1' - 1;
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

    if (rank < 1 || file < 1 || rank > 8 || file > 8) { return "BAD"; }

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
    vector<pair<string, int>> temp;

    int pos = placeToInt(place);
    int file = ((pos - 1) / 8) + 1;
    int rank = ((pos - 1) % 8) + 1;

    if (turn == 'w') {
        int tempFile = file;
        int tempRank = rank + 1;
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') { temp.emplace_back(next, 0); }
        }

        if (rank == 2) {
            tempFile = file;
            tempRank = rank + 2;
            cur = tempRank + 8 * (tempFile - 1);
            if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
                string next = placeToString(cur);
                char nextPiece = getPositonStatus(next);

                if (nextPiece == 'e') { temp.emplace_back(next, 0); }
            }
        }

        tempFile = file + 1;
        tempRank = rank + 1;
        cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if ((getColor(nextPiece) != turn && getColor(nextPiece) != 'e') || enPessant == next) {
                temp.emplace_back(next, 1);
            }
        }

        tempFile = file - 1;
        tempRank = rank + 1;
        cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if ((getColor(nextPiece) != turn && getColor(nextPiece) != 'e') || enPessant == next) {
                temp.emplace_back(next, 1);
            }
        }

    }
    // for black
    else {
        int tempFile = file;
        int tempRank = rank - 1;
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') { temp.emplace_back(next, 0); }
        }

        if (rank == 7) {
            tempFile = file;
            tempRank = rank - 2;
            cur = tempRank + 8 * (tempFile - 1);
            if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
                string next = placeToString(cur);
                char nextPiece = getPositonStatus(next);

                if (nextPiece == 'e') { temp.emplace_back(next, 0); }
            }
        }

        tempFile = file + 1;
        tempRank = rank - 1;
        cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if ((getColor(nextPiece) != turn && getColor(nextPiece) != 'e') || enPessant == next) {
                temp.emplace_back(next, 1);
            }
        }

        tempFile = file - 1;
        tempRank = rank - 1;
        cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if ((getColor(nextPiece) != turn && getColor(nextPiece) != 'e') || enPessant == next) {
                temp.emplace_back(next, 1);
            }
        }
    }

    return temp;
}

vector<pair<string, int>> MyGame::getRookMoves(string place) {
    vector<pair<string, int>> temp;
    int pos = placeToInt(place);
    int file = ((pos - 1) / 8) + 1;
    int rank = ((pos - 1) % 8) + 1;

    int tempFile = file + 1;
    int tempRank = rank;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempFile++;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }

    tempFile = file - 1;
    tempRank = rank;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempFile--;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }

    tempFile = file;
    tempRank = rank + 1;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempRank++;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }

    tempFile = file;
    tempRank = rank - 1;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempRank--;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }
    return temp;
}

vector<pair<string, int>> MyGame::getKnightMoves(string place) {
    vector<pair<string, int>> temp;
    int pos = placeToInt(place);
    int file = ((pos - 1) / 8) + 1;
    int rank = ((pos - 1) % 8) + 1;

    int tempFile = file - 1;
    int tempRank = rank - 2;
    int cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file - 1;
    tempRank = rank + 2;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file - 2;
    tempRank = rank - 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file - 2;
    tempRank = rank + 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file + 1;
    tempRank = rank - 2;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file + 1;
    tempRank = rank + 2;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file + 2;
    tempRank = rank - 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file + 2;
    tempRank = rank + 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    return temp;
}

vector<pair<string, int>> MyGame::getBishopMoves(string place) {
    vector<pair<string, int>> temp;

    int pos = placeToInt(place);
    int file = ((pos - 1) / 8) + 1;
    int rank = ((pos - 1) % 8) + 1;

    int tempFile = file + 1;
    int tempRank = rank + 1;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempFile++;
                tempRank++;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }
    tempFile = file - 1;
    tempRank = rank - 1;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempFile--;
                tempRank--;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }

    tempFile = file + 1;
    tempRank = rank - 1;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempFile++;
                tempRank--;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }

    tempFile = file - 1;
    tempRank = rank + 1;
    while (tempFile <= 8 && tempFile >= 1 && tempRank <= 8 && tempRank >= 1) {
        int cur = tempRank + 8 * (tempFile - 1);
        if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
            string next = placeToString(cur);
            char nextPiece = getPositonStatus(next);

            if (nextPiece == 'e') {
                temp.emplace_back(next, 0);
                tempFile--;
                tempRank++;
            } else if (getColor(nextPiece) != turn) {
                temp.emplace_back(next, 1);
                break;
            } else
                break;
        }
    }
    return temp;
}

vector<pair<string, int>> MyGame::getQueenMoves(string place) {
    vector<pair<string, int>> temp;
    vector<pair<string, int>> tempBish = getBishopMoves(place);
    vector<pair<string, int>> tempRook = getRookMoves(place);

    for (auto i: tempBish) temp.push_back(i);
    for (auto i: tempRook) temp.push_back(i);
    return temp;
}

vector<pair<string, int>> MyGame::getKingMoves(string place) {
    vector<pair<string, int>> temp;

    int pos = placeToInt(place);
    int file = ((pos - 1) / 8) + 1;
    int rank = ((pos - 1) % 8) + 1;

    int tempFile = file;
    int tempRank = rank - 1;
    int cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file;
    tempRank = rank + 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file - 1;
    tempRank = rank - 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file - 1;
    tempRank = rank + 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file + 1;
    tempRank = rank - 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file + 1;
    tempRank = rank + 1;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file - 1;
    tempRank = rank;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    tempFile = file + 1;
    tempRank = rank;
    cur = tempRank + 8 * (tempFile - 1);
    if (tempFile >= 1 && tempFile <= 8 && tempRank >= 1 && tempRank <= 8) {
        string next = placeToString(cur);
        char nextPiece = getPositonStatus(next);

        if (nextPiece == 'e') {
            temp.emplace_back(next, 0);
        } else if (getColor(nextPiece) != turn) {
            temp.emplace_back(next, 1);
        }
    }

    return temp;
}

vector<pair<string, int>> MyGame::getPieceMoves(string place) {
    char piece = getPositonStatus(place);
    if (piece == 'e') {
        vector<pair<string, int>> temp;
        return temp;
    }
    if (piece == 'p' || piece == 'P') return getPawnMoves(place);
    if (piece == 'r' || piece == 'R') return getRookMoves(place);
    if (piece == 'n' || piece == 'N') return getKnightMoves(place);
    if (piece == 'b' || piece == 'B') return getBishopMoves(place);
    if (piece == 'q' || piece == 'Q') return getQueenMoves(place);
    if (piece == 'k' || piece == 'K') return getKingMoves(place);
}

int MyGame::processMove(string fir, string sec) {
    // 0 process move
    // 1 checkmate game over show winner
    // 2 draw
    // -1 comrade
    // -2 reject

    char piece = getPositonStatus(fir);

    vector<pair<string, int>> nextMoves = getPieceMoves(fir);

    for (pair<string, int> i: nextMoves) {
        if (i.first == sec) {
            if (i.second == 0 || i.second == 1) {
                chessTable.set(sec, piece);
                chessTable.set(fir, 'e');

                if (turn == 'w')
                    turn = 'b';
                else {
                    turn = 'w';
                    fullMoves++;
                }

                // en Pessant check
                if (piece == 'p' || piece == 'P') {
                    int rank1 = ((placeToInt(fir) - 1) % 8) + 1;
                    int file1 = ((placeToInt(fir) - 1) / 8) + 1;

                    int rank2 = ((placeToInt(sec) - 1) % 8) + 1;
                    int file2 = ((placeToInt(sec) - 1) / 8) + 1;

                    if ((rank2 - rank1) == 2)
                        enPessant = placeToString((rank1 + 1) + 8 * (file1 - 1));
                    else if ((rank1 - rank2) == 2)
                        enPessant = placeToString((rank1 - 1) + 8 * (file1 - 1));
                }

                // castling check
                if (piece == 'r' || piece == 'k' || piece == 'R' || piece == 'K') {}

                // halfmove update
                if (piece == 'p' || piece == 'P' || i.second == 1)
                    halfMoves = 0;
                else
                    halfMoves++;

                return 0;
            }
        }
    }
    return -1;
}
