#pragma once
#include <SFML/Graphics.hpp>

class button {
private:
    sf::Texture textureActive;
    sf::Texture textureInactive;
    bool isPressed;
    int xPos;
    int yPos;
    int xSize;
    int ySize;
public:
    button(int xPos, int yPos, int xSize, int ySize);
    void render(sf::RenderWindow& window);
    void checkIfPressed(int mousePosX, int mousePosY);
};