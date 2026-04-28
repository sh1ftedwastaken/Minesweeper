#include <SFML/Graphics.hpp>
#include "WelcomeWindow.hpp"
#include "Board.hpp"
#include "GameWindow.hpp"
#include "LeaderboardWindow.hpp"
using namespace std;

vector<int> loadConfig(const string& path){
    string cols, rows, mines;
    ifstream config(path);
    if (not config.is_open()){
        cerr << "Error: Could not load files from " << path << endl;
        cerr << std::filesystem::current_path();
    }

    getline(config, cols);
    getline(config, rows);
    getline(config, mines);
    
    return {stoi(cols), stoi(rows), stoi(mines)};
}


int main(){   
    auto config = loadConfig("files/config.cfg");
    int cols = config[0];
    int rows = config[1];
    int mines = config[2];

    string username = showWelcomeWindow(cols, rows);
    if (username == ""){
        return 0;
    }

    GameWindow gameWindow(cols, rows, mines, username);
    gameWindow.run();

    return 0;
}