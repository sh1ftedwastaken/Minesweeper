#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <sstream>
#include "Board.hpp"
#include "LeaderboardWindow.hpp"

class GameWindow {
private:
    int cols, rows, mines;
    std::string username;

    sf::Clock clock;
    bool timerStarted    = false;
    int time             = 0;
    std::string str_time = "00:00";

    sf::RenderWindow window;

    sf::Texture digitsTexture;
    sf::Texture faceTexture;
    sf::Texture faceWinTexture;
    sf::Texture faceLoseTexture;
    sf::Texture debugTexture;
    sf::Texture pauseTexture;
    sf::Texture playTexture;
    sf::Texture leaderboardTexture;

    sf::Sprite* faceButton;
    sf::Sprite* debugButton;
    sf::Sprite* pauseButton;
    sf::Sprite* leaderboardButton;
    sf::Sprite* mineDigits[3];
    sf::Sprite* timer[4];
    sf::Sprite* minusSprite;

    bool negative = false;

    sf::FloatRect faceBounds;
    sf::FloatRect debugBounds;
    sf::FloatRect pauseBounds;
    sf::FloatRect leaderboardBounds;

    Board board;  

    void loadTextures();
    void setupSprites();
    void updateMineCounter(int value);
    void updateTimer();
    void setTimerDisplay(int seconds);
    void updateLeaderboard();

    void processEvents();
    void render();

    void restartGame();

public:
    GameWindow(int c, int r, int m, std::string uname);
    ~GameWindow();
    void run();
};

int secToMinConverter(int seconds);
