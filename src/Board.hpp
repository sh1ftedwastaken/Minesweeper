#pragma once
#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>


class Board{
private:
    int rows;
    int cols;
    int mineCount;

    bool gameOver        = false;
    bool gameWon         = false;
    bool debugMode       = false;
    bool pause           = false;
    bool leaderboard     = false;

    std::vector<std::vector<Tile>> grid;

    sf::Texture hiddenTexture;
    sf::Texture revealedTexture;
    sf::Texture flagTexture;
    sf::Texture mineTexture;
    std::vector<sf::Texture> numberTextures;

    void loadTextures();
    void loadTexturesAndPosToTiles();
    void placeMines();
    void calculateAdjacentMines();

public:
    Board(int c, int r, int m);

    void initialize();

    void revealTile(int x, int y);
    void toggleFlag(int x, int y);
    void toggleDebug();
    void togglePause();
    void toggleLeaderboard();

    int countFlags() const;
    int countRevealed() const;

    void draw(sf::RenderWindow& window);

    int getRows() const;
    int getCols() const;
    int getMinesNum() const;

    bool getWon() const;
    bool getOver() const;
    bool getDebug() const;
    bool getPause() const;
    bool getLeaderboard() const;
};



