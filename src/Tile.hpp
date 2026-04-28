#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Tile{
private:
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
    sf::Vector2f position;
    
    sf::Texture* hiddenTexture;
    sf::Texture* revealedTexture;
    sf::Texture* flagTexture;
    sf::Texture* mineTexture;
    std::vector<sf::Texture*> numberTextures;

public: 
    Tile();

    void setMine(bool b);
    void setRevealed(bool b);
    void setFlagged(bool b);
    void setAdjacentMines(int adj_mines);
    void setPosition(float x, float y);

    void setTextures(
        sf::Texture* hidden, 
        sf::Texture* revealed,
        sf::Texture* flag, 
        sf::Texture* mine,
        std::vector<sf::Texture*> numbers
    );

    bool getMine() const;
    bool getRevealed() const;
    bool getFlagged() const;
    int getAdjacentMines() const;
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window, bool debugMode=false, bool pause=false, bool leaderboard=false);
};