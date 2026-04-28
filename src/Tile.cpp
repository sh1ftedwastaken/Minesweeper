#include "Tile.hpp"
#include "Board.hpp"
using namespace std;

Tile::Tile(): 
    isMine(false),
    isRevealed(false),
    isFlagged(false),
    adjacentMines(0),
    hiddenTexture(nullptr),
    revealedTexture(nullptr),
    flagTexture(nullptr),
    mineTexture(nullptr)
{
}

void Tile::setMine(bool b){isMine = b;}

void Tile::setRevealed(bool b){isRevealed = b;}

void Tile::setFlagged(bool b){isFlagged = b;}

void Tile::setAdjacentMines(int adj_mines){adjacentMines = adj_mines;}

void Tile::setPosition(float x, float y){
    position = {x, y};
}

void Tile::setTextures(
    sf::Texture* hidden, 
    sf::Texture* revealed,
    sf::Texture* flag, 
    sf::Texture* mine,
    std::vector<sf::Texture*> numbers
){
    hiddenTexture = hidden;
    revealedTexture = revealed;
    flagTexture = flag;
    mineTexture = mine;
    numberTextures = numbers;
}

bool Tile::getMine() const {return isMine;}

bool Tile::getRevealed() const {return isRevealed;}

bool Tile::getFlagged() const {return isFlagged;}

int Tile::getAdjacentMines() const {return adjacentMines;}

sf::Vector2f Tile::getPosition() const {return position;}

void Tile::draw(sf::RenderWindow& window, bool debugMode, bool pause, bool leaderboard){
    sf::Texture* baseTexture = nullptr;
    sf::Texture* overlayTexture = nullptr;

    if (leaderboard){
        baseTexture = revealedTexture;
        overlayTexture = nullptr;
    } else if (isRevealed or pause){
        baseTexture = revealedTexture;
    } else{
        baseTexture = hiddenTexture;
    }

    if (leaderboard or pause){
        overlayTexture = nullptr;
    } else if (debugMode and isMine and not isFlagged and not isRevealed){
        overlayTexture = mineTexture;
    } else if (isFlagged){
        overlayTexture = flagTexture;
    } else if (isMine and isRevealed){
        overlayTexture = mineTexture;
    } else if (isRevealed and adjacentMines > 0){
        overlayTexture = numberTextures[adjacentMines - 1];
    }

    sf::Sprite base(*baseTexture);
    base.setPosition(position);
    window.draw(base);

    if (overlayTexture != nullptr){
        sf::Sprite overlay(*overlayTexture);
        overlay.setPosition(position);
        window.draw(overlay);
    }
}

