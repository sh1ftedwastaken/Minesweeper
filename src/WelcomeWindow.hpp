#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include "Board.hpp"

void static setText(sf::Text &text, float x, float y);

int clampCursorPos(int cursorPos, int usernameSize);

std::string showWelcomeWindow(int colCount, int rowCount);