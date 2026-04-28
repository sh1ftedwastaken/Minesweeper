#include "Board.hpp"
using namespace std;

Board::Board(int c, int r, int m)
     : cols(c), rows(r), mineCount(m)
{
    initialize();
}

void Board::initialize() {
    gameOver = false;
    gameWon = false;
    debugMode = false;
    pause = false;

    loadTextures();

    grid.assign(rows, std::vector<Tile>(cols));
    loadTexturesAndPosToTiles();
    placeMines();
    calculateAdjacentMines();
}


void Board::loadTextures() {
    if (not hiddenTexture.loadFromFile("files/images/tile_hidden.png")){
        cerr << "Error: Could not load images/tile_hidden.png\n";
    }
    if (not revealedTexture.loadFromFile("files/images/tile_revealed.png")){
        cerr << "Error: Could not load images/tile_revealed.png\n";
    }
    if (not flagTexture.loadFromFile("files/images/flag.png")){
        cerr << "Error: Could not load images/flag.png\n";
    }
    if (not mineTexture.loadFromFile("files/images/mine.png")){
        cerr << "Error: Could not load images/mine.png\n";
    }
    
    numberTextures.clear();
    for (unsigned int i = 1; i <= 8; i++){
        sf::Texture numTexture;
        string num_path = "files/images/number_" + to_string(i) + ".png";
        if (not numTexture.loadFromFile(num_path)){
            cerr << "Error: Could not load " << num_path << "\n";
        }
        numberTextures.push_back(numTexture);
    }
}


void Board::loadTexturesAndPosToTiles() {
    sf::Texture* hidden = &hiddenTexture;
    sf::Texture* revealed = &revealedTexture;
    sf::Texture* flag = &flagTexture;
    sf::Texture* mine = &mineTexture;
    vector<sf::Texture*> numbers;

    for (sf::Texture& num : numberTextures){
        numbers.push_back(&num);
    }

    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            grid[r][c].setTextures(hidden, revealed, flag, mine, numbers);
            grid[r][c].setPosition(c * 32, r * 32);
        }
    }
}


void Board::placeMines() {
    vector<int> indices;

    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            int index = r * cols + c;
            indices.push_back(index);
        }
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(indices.begin(), indices.end(), g);

    for (int i = 0; i < mineCount; i++){
        int row = indices[i] / cols;
        int col = indices[i] % cols;

        grid[row][col].setMine(true);
    }
}


void Board::calculateAdjacentMines() {
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            if (grid[r][c].getMine()) continue;

            int mines_count = 0;
            for (int dr : {-1, 0 , 1}){
                for (int dc : {-1, 0, 1}){
                   if (dr == 0 and dc == 0) continue;

                    int neighbor_r = r + dr;
                    int neighbor_c = c + dc;

                    if (neighbor_r < 0 or neighbor_r >= rows) continue;
                    if (neighbor_c < 0 or neighbor_c >= cols) continue;

                    if (grid[neighbor_r][neighbor_c].getMine()) mines_count++;
                }
            }
            grid[r][c].setAdjacentMines(mines_count);
        }
    }
}


void Board::revealTile(int x, int y) {
    if (x < 0 or x >= cols) return;
    if (y < 0 or y >= rows) return;

    Tile& tile = grid[y][x];

    if (tile.getFlagged() or tile.getRevealed()) return;

    tile.setRevealed(true);

    if (tile.getMine()) {
        gameOver = true;

        for (int r = 0; r < rows; r++){
            for (int c = 0; c < cols; c++){
                if (grid[r][c].getMine()) 
                    grid[r][c].setRevealed(true);
            }
        }

        return;
    }

    if (tile.getAdjacentMines() > 0) {
        if (countRevealed() == rows * cols - mineCount){
            gameWon = true;

            for (int r = 0; r < rows; r++){
                for (int c = 0; c < cols; c++){
                    if (grid[r][c].getMine()){
                        grid[r][c].setFlagged(true);
                    }
                }
            }
        }
        return;
    }

    if (tile.getAdjacentMines() == 0){
        for (int dy : {-1, 0 , 1}){
            for (int dx : {-1, 0, 1}){
                if (dx == 0 and dy == 0) continue;

                int neighbor_y = y + dy;
                int neighbor_x = x + dx;

                if (neighbor_y < 0 or neighbor_y >= rows) continue;
                if (neighbor_x < 0 or neighbor_x >= cols) continue;

                if (grid[neighbor_y][neighbor_x].getFlagged()) continue;

                revealTile(neighbor_x, neighbor_y);
            }
        }
    }

    if (countRevealed() == rows * cols - mineCount){
        gameWon = true;

        for (int r = 0; r < rows; r++){
            for (int c = 0; c < cols; c++){
                if (grid[r][c].getMine()){
                    grid[r][c].setFlagged(true);
                }
            }
        }
    }
}


void Board::toggleFlag(int x, int y) {
    if (x < 0 or x >= cols) return;
    if (y < 0 or y >= rows) return;

    Tile& tile = grid[y][x];

    if (tile.getRevealed()) return;
    
    if (not tile.getFlagged()){
        tile.setFlagged(true);
    }else{
        tile.setFlagged(false);
    }
}


void Board::toggleDebug(){
    debugMode = not debugMode;
}


void Board::togglePause(){
    pause = not pause;
}

void Board::toggleLeaderboard(){
    leaderboard = not leaderboard;
}


int Board::countFlags() const{
    int flags = 0;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            if (grid[r][c].getFlagged()){
                flags += 1;
            }
        }
    }
    return flags;
}


int Board::countRevealed() const{
    int reveled = 0;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            if (grid[r][c].getRevealed() and not grid[r][c].getMine()){
                reveled += 1;
            }
        }
    }
    return reveled;
}


void Board::draw(sf::RenderWindow& window) {
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            grid[r][c].draw(window, debugMode, pause, leaderboard);
        }
    }
}

int Board::getRows() const {return rows;}
int Board::getCols() const {return cols;}
int Board::getMinesNum() const {return mineCount;}

bool Board::getWon() const {return gameWon;}
bool Board::getOver() const {return gameOver;}
bool Board::getDebug() const {return debugMode;}
bool Board::getPause() const {return pause;}
bool Board::getLeaderboard() const {return leaderboard;}