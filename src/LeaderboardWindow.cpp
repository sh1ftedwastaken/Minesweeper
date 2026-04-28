#include "LeaderboardWindow.hpp"
using namespace std;

void Leaderboard::loadFile(string filename){
    ifstream file(filename);

    if (not file.is_open()){
        cerr << "Could not open leaderboard.txt\n";
        return;
    }

    vector<pair<string, string>> board;
    string line;
    while (getline(file, line)){
        stringstream ss(line);
        string str_time;
        string name;
        
        getline(ss, str_time, ',');
        getline(ss, name);

        board.push_back({name, str_time});
    }

    int n = min<int>(board.size(), 5);
    str_board.clear();
    cout << str_time << endl;
    for (int i = 0; i < n; i++){
        string n = board.at(i).first;
        string t = board.at(i).second;

        if (t == str_time) { 
            n += '*';
        }

        str_board += to_string(i + 1) + 
                     ".\t" + t + "\t" +
                     n + "\n\n";
    }
}


void Leaderboard::run(int colCount, int rowCount, Board& board){
    loadFile("files/leaderboard.txt");

    unsigned int width  = colCount * 16;
    unsigned int height = rowCount * 16 + 50;

    sf::RenderWindow window(sf::VideoMode({width, height}), "Leaderboard Window", sf::Style::Close);
    
    sf::Font font;
    if (not font.openFromFile("files/font.ttf")){
        cerr << "Error: Could not load font\n"; 
    }

    sf::Text title(font, "LEADERBOARD", 20);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    setText(title, width / 2, height / 2 - 120);

    sf::Text content(font, str_board, 18);
    content.setStyle(sf::Text::Bold);
    content.setFillColor(sf::Color::White);
    setText(content, width / 2, height / 2 + 20);

    while (window.isOpen()){
        while (const optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }

        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(content);

        window.display();
        }
    }
}


void static setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, 
                    textRect.position.y + textRect.size.y / 2.0f});
    text.setPosition(sf::Vector2f(x, y));
}
 