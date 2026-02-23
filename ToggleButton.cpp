#include "ToggleButton.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

toggleButton::toggleButton(float xPos, float yPos, int xSize, int ySize, std::filesystem::__cxx11::path activeTexture, std::filesystem::__cxx11::path inactiveTexture)
    : button(xPos, yPos, xSize, ySize, activeTexture, inactiveTexture)
{
    // all default and specified values are assigned to each of the attributes

    this->isPressed = false;
    this->stateChanged = false;
};

void toggleButton::render(sf::RenderWindow& window) { // draws the button to the screen and changes the sprite if necessary, ran every frame
    if (stateChanged) { // checks if the button has been clicked since the last frame
        if (isPressed) { // checks if the button is currently active
            (*buttonSprite).setTexture(pressedTexture); // sets the button sprite to the active texture if the condition above is true
        } else {
            (*buttonSprite).setTexture(idleTexture); // sets the button sprite to the inactive texture if the condition above is false
        }
        stateChanged = false;
        elapsedTime.restart(); // restarts the time since last pressed to create a half second window where the button can't be pressed again
    }
    window.draw((*buttonSprite)); // draws the button sprite to the screen
};