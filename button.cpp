#include "Button.h"

button::button(int xPos, int yPos, int xSize, int ySize) {
    this->textureActive.loadFromFile("ShowVelocityGreen.png");
    this->textureInactive.loadFromFile("ShowVelocityRed.png");
    this->isPressed = false;
    this->xPos = xPos;
    this->yPos = yPos;
    this->xSize = xSize;
    this->ySize = ySize;
};

void button::render(sf::RenderWindow& window) {

};

void button::checkIfPressed(int mousePosX, int mousePosY) {
    
};

