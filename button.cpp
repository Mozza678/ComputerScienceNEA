#include "button.h"
#include <memory>
#include <iostream>

button::button(float xPos, float yPos, int xSize, int ySize) {
    if (this->textureActive.loadFromFile("ShowVelocityGreen.png") && this->textureInactive.loadFromFile("ShowVelocityRed.png")) {
        std::cout << "textures loaded";
    } else {
        std::cout << "textures failed to load";
    };
    this->isPressed = false;
    this->xPos = xPos;
    this->yPos = yPos;
    this->xSize = xSize;
    this->ySize = ySize;
    this->stateChanged = false;
    this->elapsedTime.restart();
    buttonSprite = std::make_unique<sf::Sprite>(textureInactive);
    (*buttonSprite).setPosition({xPos, yPos});
};

void button::render(sf::RenderWindow& window) {
    if (stateChanged) {
        if (isPressed) {
            (*buttonSprite).setTexture(textureActive);
        } else {
            (*buttonSprite).setTexture(textureInactive);
        }
        stateChanged = false;
        elapsedTime.restart();
    }
    window.draw((*buttonSprite));
};

bool button::checkIfHoveringOver(int mousePosX, int mousePosY) {
    if (mousePosX > xPos && mousePosY > yPos && mousePosX < xPos + xSize && mousePosY < yPos + ySize) {
        return true;
    } else {
        return false;
    }
};