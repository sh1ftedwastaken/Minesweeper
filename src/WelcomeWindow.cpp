#include "WelcomeWindow.hpp"
using namespace std;

void static setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, 
                    textRect.position.y + textRect.size.y / 2.0f});
    text.setPosition(sf::Vector2f(x, y));
}

int clampCursorPos(int cursorPos, int usernameSize){
    if (cursorPos < 0) cursorPos = 0;
    if (cursorPos > usernameSize) cursorPos = usernameSize;
    return cursorPos;
}

string showWelcomeWindow(int colCount, int rowCount){
    unsigned int width  = colCount * 32;
    unsigned int height = rowCount * 32 + 100;

    sf::RenderWindow window(sf::VideoMode({width, height}), "Welcome Window", sf::Style::Close);
    
    sf::Font font;
    if (not font.openFromFile("files/font.ttf")){
        cerr << "Error: Could not load font\n"; 
    }

    sf::Text title(font, "WELCOME TO MINESWEEPER!", 40);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    setText(title, width / 2, height / 2 - 150);

    sf::Text askName(font, "Enter your name:", 20);
    askName.setStyle(sf::Text::Bold);
    askName.setFillColor(sf::Color::White);
    setText(askName, width / 2, height / 2 - 75);

    sf::Text inputText(font);
    inputText.setCharacterSize(18);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Yellow);
    setText(inputText, width / 2, height / 2 - 45);

    string username;
    int cursorPos = 0;

    while (window.isOpen()){
        while (const optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return "";
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
                    cursorPos = clampCursorPos(cursorPos - 1, username.size());
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                    cursorPos = clampCursorPos(cursorPos + 1, username.size());
                }
            }

            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()){
                auto character = textEntered->unicode;

                if (character == '\b'){
                    if (not username.empty() and cursorPos > 0) {
                        username.erase(cursorPos - 1, 1);
                        cursorPos = clampCursorPos(cursorPos - 1, username.size());
                    }

                }else if (character == '\n' or character == '\r'){
                    if (not username.empty()) return username;

                }else if (isalpha(static_cast<unsigned char>(character)) and username.length() < 10){
                    username.insert(cursorPos, 1, static_cast<char>(character));
                    cursorPos = clampCursorPos(cursorPos + 1, username.size());
                }

                if (not username.empty()){
                    for (auto& c: username){
                        c = tolower((unsigned char)c);
                    }
                    username[0] = toupper((unsigned char)username[0]);
                }
            }
        }
        string displayUsername = username;
        displayUsername.insert(clampCursorPos(cursorPos, username.size()), "|");
        inputText.setString(displayUsername);
        setText(inputText, width / 2, height / 2 - 45);

        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(askName);
        window.draw(inputText);

        window.display();
    }
    return "";
}