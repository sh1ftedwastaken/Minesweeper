#include "GameWindow.hpp"
using namespace std;

GameWindow::GameWindow(int c, int r, int m, string uname):
    cols(c), rows(r), mines(m),
    username(uname),
    window(sf::VideoMode({cols * 32u, rows * 32u + 100u}), "Minesweeper", sf::Style::Close),
    board(cols, rows, mines)
{
    loadTextures();
    setupSprites();
    updateMineCounter(mines);
    updateTimer();
}

GameWindow::~GameWindow() {
    delete faceButton;
    delete debugButton;
    delete pauseButton;
    delete leaderboardButton;

    delete minusSprite;

    for (int i = 0; i < 3; i++){
        delete mineDigits[i];
    }
    
    for (int i = 0; i < 4; i++){
        delete timer[i];
    }
}


void GameWindow::loadTextures(){
    if (not digitsTexture.loadFromFile("files/images/digits.png")){
        cerr << "Error: Digits texture not loaded\n";
    }
    if (not faceTexture.loadFromFile("files/images/face_happy.png")){
        cerr << "Error: Face texture not loaded\n";
    }
    if (not faceWinTexture.loadFromFile("files/images/face_win.png")){
        cerr << "Error: Face win texture not loaded\n";
    }
    if (not faceLoseTexture.loadFromFile("files/images/face_lose.png")){
        cerr << "Error: Face lose texture not loaded\n";
    }
    if (not debugTexture.loadFromFile("files/images/debug.png")){
        cerr << "Error: Debug texture not loaded\n";
    }
    if (not pauseTexture.loadFromFile("files/images/pause.png")){
        cerr << "Error: Pause texture not loaded\n";
    }
    if (not playTexture.loadFromFile("files/images/play.png")){
        cerr << "Error: Play texture not loaded\n";
    }
    if (not leaderboardTexture.loadFromFile("files/images/leaderboard.png")){
        cerr << "Error: Leaderboard texture not loaded\n";
    }
}

void GameWindow::setupSprites(){
    float yPos = 32 * (rows + 0.5f);

    faceButton        = new sf::Sprite(faceTexture);
    debugButton       = new sf::Sprite(debugTexture);
    pauseButton       = new sf::Sprite(pauseTexture);
    leaderboardButton = new sf::Sprite(leaderboardTexture);

    faceButton->setPosition({(cols * 32.0f / 2.0f) - 32, yPos});
    debugButton->setPosition({(cols * 32.0f) - 304, yPos});
    pauseButton->setPosition({(cols * 32.0f) - 240, yPos});
    leaderboardButton->setPosition({(cols * 32.0f) - 176, yPos});

    faceBounds = faceButton->getGlobalBounds();
    debugBounds = debugButton->getGlobalBounds();
    pauseBounds = pauseButton->getGlobalBounds();
    leaderboardBounds = leaderboardButton->getGlobalBounds();

    float digitXPos = 33;
    for (int i = 0; i < 3; i++){
        mineDigits[i] = new sf::Sprite(digitsTexture);
        mineDigits[i]->setPosition({digitXPos + 21 * i, yPos + 16});
    }

        minusSprite = new sf::Sprite(digitsTexture);
    minusSprite->setPosition({12, yPos + 16});

    timer[0] = new sf::Sprite(digitsTexture);
    timer[1] = new sf::Sprite(digitsTexture);
    timer[2] = new sf::Sprite(digitsTexture);
    timer[3] = new sf::Sprite(digitsTexture);

    timer[0]->setPosition({(cols * 32.0f) - 97, yPos + 16});
    timer[1]->setPosition({(cols * 32.0f) - 76, yPos + 16});
    timer[2]->setPosition({(cols * 32.0f) - 54, yPos + 16});
    timer[3]->setPosition({(cols * 32.0f) - 33, yPos + 16});
}


void GameWindow::updateMineCounter(int value) {
    negative = value < 0;
    value = std::abs(value);

    int ones     = value % 10;
    int tens     = (value / 10) % 10;
    int hundreds = (value / 100) % 10;

    mineDigits[0]->setTextureRect(sf::IntRect({hundreds * 21, 0}, {21, 32}));
    mineDigits[1]->setTextureRect(sf::IntRect({tens     * 21, 0}, {21, 32}));
    mineDigits[2]->setTextureRect(sf::IntRect({ones     * 21, 0}, {21, 32}));

    minusSprite->setTextureRect(sf::IntRect({10 * 21, 0}, {21, 32}));
}

void GameWindow::setTimerDisplay(int seconds) {
    int ones      = seconds % 10;
    int tens      = (seconds / 10) % 10;
    int hundreds  = (seconds / 100) % 10;
    int thousands = (seconds / 1000) % 10;

    timer[0]->setTextureRect(sf::IntRect({thousands * 21, 0}, {21, 32}));
    timer[1]->setTextureRect(sf::IntRect({hundreds  * 21, 0}, {21, 32}));
    timer[2]->setTextureRect(sf::IntRect({tens      * 21, 0}, {21, 32}));
    timer[3]->setTextureRect(sf::IntRect({ones      * 21, 0}, {21, 32}));
}

void GameWindow::updateTimer(){
    if (board.getOver() or board.getWon()) {
        if (timerStarted){
            time = clock.getElapsedTime().asSeconds();
        }
        timerStarted = false;
        setTimerDisplay(time);
        return;
    }

    if (board.getPause() or board.getLeaderboard()) {
        setTimerDisplay(time);
        return;
    }

    if (not timerStarted) {
        setTimerDisplay(time);
        return;
    }

    time = clock.getElapsedTime().asSeconds();
    setTimerDisplay(time);

    int ones      = time % 10;
    int tens      = (time / 10)   % 10;
    int hundreds  = (time / 100)  % 10;
    int thousands = (time / 1000) % 10;

    str_time = to_string(thousands) + 
               to_string(hundreds)  + 
               ":"                  + 
               to_string(tens)      + 
               to_string(ones);
}


void GameWindow::updateLeaderboard(){
    ifstream fr("files/leaderboard.txt");
    if (not fr.is_open()){
        cerr << "Error: Could not open leaderboard.txt\n";
        return;
    }

    map<string, string> board;

    board[str_time] = username;

    string line;
    while (getline(fr, line)){
        stringstream ss(line);
        string t, n;

        getline(ss, t, ',');
        getline(ss, n);

        n.erase(remove_if(n.begin(), n.end(), [](char& c){
            return c == ' ' or c == '*';
        }), n.end());

        cout << n;
        board[t] = n;
    }
    fr.close();

    ofstream fw("files/leaderboard.txt");
    int i = 0;
    for (const auto& [time, username] : board){
        if (i > 5) break;
        fw << time + ", " + username + "\n";
        i++;
    }
}


void GameWindow::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();

        if (board.getLeaderboard()) continue;

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){
            if (mousePressed->button == sf::Mouse::Button::Left){
                int mx = mousePressed->position.x;
                int my = mousePressed->position.y;

                // Update tiles
                if (my < rows * 32 and board.getPause() != true){
                    int tileX = mx / 32;
                    int tileY = my / 32;

                    if (board.getOver() or board.getWon()) continue;
                    board.revealTile(tileX, tileY);

                    if (not timerStarted and not board.getOver() and not board.getWon()){
                        timerStarted = true;
                        clock.restart();
                    }

                    if (board.getWon()){
                        updateMineCounter(0);
                        updateLeaderboard();
                    }
                }

                // Extra buttons
                // Face Button
                if (faceBounds.contains({float(mx), float(my)})){
                    restartGame();
                    return;
                }
                
                // Leaderboard Button
                else if (leaderboardBounds.contains({float(mx), float(my)})){
                    bool wasPaused = board.getPause();
                    if (not wasPaused){
                        board.togglePause();
                        if (timerStarted){
                            clock.stop();
                            pauseButton->setTexture(playTexture);
                        }
                    }

                    board.toggleLeaderboard();                    
                    render();
                    Leaderboard lb(username, str_time);
                    lb.run(cols, rows, board);
                    board.toggleLeaderboard();

                    while (window.pollEvent()) {
                    }

                    if (not wasPaused){
                        board.togglePause();
                        if (timerStarted){
                            clock.start();
                            pauseButton->setTexture(pauseTexture);
                        }
                    }
                    return;
                }

                else if (board.getWon() or board.getOver()){
                    continue;
                }

                // Debug Button
                else if (debugBounds.contains({float(mx), float(my)}) and board.getPause() != true){
                    board.toggleDebug();
                }

                // Pause Button
                else if (pauseBounds.contains({float(mx), float(my)})){ 
                    board.togglePause();

                    if (board.getPause()){
                        pauseButton->setTexture(playTexture);
                        clock.stop();
                    }else{
                        pauseButton->setTexture(pauseTexture);
                        clock.start();
                    }
                }
            }
            
            if (mousePressed->button == sf::Mouse::Button::Right){
                int mx = mousePressed->position.x;
                int my = mousePressed->position.y;

                if (my < rows * 32){
                    int tileX = mx / 32;
                    int tileY = my / 32;

                    if (board.getOver() or board.getWon()) continue;

                    board.toggleFlag(tileX, tileY);
                    updateMineCounter(board.getMinesNum() - board.countFlags());
                }
            }
        }
    }
}

void GameWindow::render() {
    if (board.getOver())
        faceButton->setTexture(faceLoseTexture);
    else if (board.getWon())
        faceButton->setTexture(faceWinTexture);
    else 
        faceButton->setTexture(faceTexture);

    window.clear(sf::Color::White);

    board.draw(window);

    window.draw(*faceButton);
    window.draw(*debugButton);
    window.draw(*pauseButton);
    window.draw(*leaderboardButton);

    if (negative)
        window.draw(*minusSprite);

    for (auto& d : mineDigits)
        window.draw(*d);

    for (auto& t : timer)
        window.draw(*t);

    window.display();
}


void GameWindow::restartGame(){
    board.initialize();

    timerStarted = false;
    time = 0;

    pauseButton->setTexture(pauseTexture);

    setTimerDisplay(time);
    updateMineCounter(mines);
}


void GameWindow::run() {
    while (window.isOpen()) {
        processEvents();

        updateTimer();

        render();
    }
}



int secToMinConverter(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    return (minutes * 100) + seconds;
}
