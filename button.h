#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class button {
private:
    sf::Texture textureActive;
    sf::Texture textureInactive;
    float xPos;
    float yPos;
    int xSize;
    int ySize;
    std::unique_ptr<sf::Sprite> buttonSprite;
public:
    sf::Clock elapsedTime;
    button(float xPos, float yPos, int xSize, int ySize);
    void render(sf::RenderWindow& window);
    bool checkIfHoveringOver(int mousePosX, int mousePosY);
    bool isPressed;
    bool stateChanged;
};