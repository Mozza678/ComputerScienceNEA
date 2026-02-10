#include "button.h"
#include <memory>
#include <iostream>

button::button(float xPos, float yPos, int xSize, int ySize)
    {

    // all default and specified values are assigned to each of the attributes

    this->isPressed = false;
    this->xPos = xPos;
    this->yPos = yPos;
    this->xSize = xSize;
    this->ySize = ySize;
    this->stateChanged = false;
    this->elapsedTime.restart();

    if (this->textureActive.loadFromFile("ShowVelocityGreen.png") && this->textureInactive.loadFromFile("ShowVelocityRed.png")) { // attempts to load the texture and returns true or false depending on if they loaded succesfully
        std::cout << "textures loaded"; // prints success message if the textures load
    } else {
        std::cout << "textures failed to load"; // prints error message if the textures fail to load
    };
    
    buttonSprite = std::make_unique<sf::Sprite>(textureInactive); // sets the initial sprite for the button to the "off" texture to match the isPressed variable, this uses a pointer as the sprite object within sfml has no default constructor
    (*buttonSprite).setPosition({xPos, yPos}); // sets the position of the sprite
    };

void button::render(sf::RenderWindow& window) { // draws the button to the screen and changes the sprite if necessary, ran every frame
    if (stateChanged) { // checks if the button has been clicked since the last frame
        if (isPressed) { // checks if the button is currently active
            (*buttonSprite).setTexture(textureActive); // sets the button sprite to the active texture if the condition above is true
        } else {
            (*buttonSprite).setTexture(textureInactive); // sets the button sprite to the inactive texture if the condition above is false
        }
        stateChanged = false; // sets the state changed to false to avoid setting the button sprite every frame if it hasn't changed
        elapsedTime.restart(); // restarts the time since last pressed to create a half second window where the button can't be pressed again
    }
    window.draw((*buttonSprite)); // draws the button sprite to the screen
};

float button::getElapsedTime() {
    return elapsedTime.getElapsedTime().asSeconds();
}

bool button::checkIfHoveringOver(int mousePosX, int mousePosY) { // takes the mouse position as parameters to be compared to button bounds, the pixel position is used to create more precise bounds for clicking the button
    if (mousePosX > xPos && mousePosY > yPos && mousePosX < xPos + xSize && mousePosY < yPos + ySize) { // checks if the mouse is within the edges of the button
        return true;
    } else {
        return false;
    }
};