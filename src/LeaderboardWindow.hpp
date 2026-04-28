#pragma once
#include "Board.hpp"
#include <sfml/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

class Leaderboard{
private:
    std::string username;
    std::string str_time;

    std::string str_board;
    void loadFile(std::string filename);

public:
    Leaderboard(std::string currUsername, std::string strCurrTime): 
        username(currUsername),
        str_time(strCurrTime)
    {
    }

    void run(int colCount, int rowCount, Board& board);
};

void static setText(sf::Text &text, float x, float y);