#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

bool isWhite(char piece);
string placeToString(int place);
int placeToInt(string place);

class ChessTable {
public:
    ChessTable();
    void set(string place, char piece);   // dead or alive at position
    char positonStatus(string positon);  // empty or piece occupying

private:
    vector<char> arr;
};

class MyGame {
public:
    void importFEN(string fen);
    string getFEN();
    string getNotation();
    void setPosition(string place, char piece);
    char getPositonStatus(string positon);
    char getTurn() { return turn; };
    char getFullMoves() { return fullMoves; };
    int processMove(string move);  // update table and notation here

private:
    char turn;         // w or b
    string castling;   // KQkq
    string enPessant;  // square right before advance
    int halfMoves;
    int fullMoves;
    bool check;
    bool checkMate;
    vector<string> moves;
    ChessTable chessTable;
};
