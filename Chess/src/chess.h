#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

char getColor(char piece);
std::string placeToString(int place);
int placeToInt(std::string place);
std::string getPointer(int xpos, int ypos, int scrw, int scrh);  // for mouse picking

class ChessTable {
public:
    ChessTable();
    void set(std::string place, char piece);  // dead or alive at position
    char positonStatus(std::string positon);  // empty or piece occupying

private:
    std::vector<char> arr;
};

class MyGame {
public:
    void importFEN(std::string fen);
    std::string getFEN();
    std::string getNotation();
    void setPosition(std::string place, char piece);
    char getPositonStatus(std::string positon);
    char getTurn() { return turn; };
    int getFullMoves() { return fullMoves; };
    std::vector<std::pair<std::string, int>> getPieceMoves(std::string place);
    int processMove(std::string fir, std::string sec);  // update table and notation here

private:
    char turn;              // w or b
    std::string castling;   // KQkq
    std::string enPessant;  // square right before advance
    int halfMoves;
    int fullMoves;
    bool check;
    std::vector<std::string> moves;
    ChessTable chessTable;

    std::vector<std::pair<std::string, int>> getPawnMoves(std::string place);
    std::vector<std::pair<std::string, int>> getRookMoves(std::string place);
    std::vector<std::pair<std::string, int>> getKnightMoves(std::string place);
    std::vector<std::pair<std::string, int>> getBishopMoves(std::string place);
    std::vector<std::pair<std::string, int>> getQueenMoves(std::string place);
    std::vector<std::pair<std::string, int>> getKingMoves(std::string place);
};
