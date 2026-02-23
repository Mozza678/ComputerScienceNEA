#include "button.h"
#include <memory>
#include <iostream>

button::button(float xPos, float yPos, int xSize, int ySize, std::filesystem::__cxx11::path pressedPath, std::filesystem::__cxx11::path idlePath)
    {

    // all default and specified values are assigned to each of the attributes

    this->xPos = xPos;
    this->yPos = yPos;
    this->xSize = xSize;
    this->ySize = ySize;
    this->elapsedTime.restart();

    if (this->pressedTexture.loadFromFile(pressedPath) && this->idleTexture.loadFromFile(idlePath)) { // attempts to load the texture and returns true or false depending on if they loaded succesfully
        std::cout << "textures loaded"; // prints success message if the textures load
    } else {
        std::cout << "textures failed to load"; // prints error message if the textures fail to load
    };
    
    buttonSprite = std::make_unique<sf::Sprite>(idleTexture); // sets the initial sprite for the button to the "off" texture to match the isPressed variable, this uses a pointer as the sprite object within sfml has no default constructor
    (*buttonSprite).setPosition({xPos, yPos}); // sets the position of the sprite
    };

void button::render(sf::RenderWindow& window) { // draws the button to the screen and changes the sprite if necessary, ran every frame
    if (isPressed) { // checks if the button has been clicked since the last frame
        if (getElapsedTime() > 1.0f) { // checks if the button is currently active
            (*buttonSprite).setTexture(idleTexture); // sets the button sprite to the active texture if the condition above is true
            isPressed = false;
            elapsedTime.restart(); // restarts the time since last pressed to create a half second window where the button can't be pressed again
        } else {
            (*buttonSprite).setTexture(pressedTexture); // sets the button sprite to the inactive texture if the condition above is false
        }
    }
    window.draw((*buttonSprite)); // draws the button sprite to the screen
};

float button::getElapsedTime() {
    return elapsedTime.getElapsedTime().asSeconds();
}

void button::restartElapsedTime() {
    elapsedTime.restart();
}

bool button::checkIfHoveringOver(int mousePosX, int mousePosY) { // takes the mouse position as parameters to be compared to button bounds, the pixel position is used to create more precise bounds for clicking the button
    if (mousePosX > xPos && mousePosY > yPos && mousePosX < xPos + xSize && mousePosY < yPos + ySize) { // checks if the mouse is within the edges of the button
        return true;
    } else {
        return false;
    }
};