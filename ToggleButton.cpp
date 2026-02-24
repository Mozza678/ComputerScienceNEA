#include "ToggleButton.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

toggleButton::toggleButton(float xPos, float yPos, int xSize, int ySize, std::filesystem::path activeTexturePath, std::filesystem::path inactiveTexturePath)
    : button(xPos, yPos, xSize, ySize, activeTexturePath, inactiveTexturePath)
{
    // all default and specified values are assigned to each of the attributes

    this->stateChanged = false; // stateChanged set to false by default as the user hasn't interacted with the button yet
};

void toggleButton::render(sf::RenderWindow& window) { // overriden render function inherited from the "button" base class. performs a similiar function but alters the logic so the button remains on or off
    if (stateChanged) { // checks if the button has been clicked since the last frame
        if (isPressed) { // checks if the button is currently active
            (*buttonSprite).setTexture(pressedTexture); // sets the button sprite to the active texture if the condition above is true
        } else {
            (*buttonSprite).setTexture(idleTexture); // sets the button sprite to the inactive texture if the condition above is false
        }
        stateChanged = false; // sets the stateChanged variable back to false as we've now acted on the state change
        elapsedTime.restart(); // restarts the time since last pressed to create a half second window where the button can't be pressed again
    }
    window.draw((*buttonSprite)); // draws the button sprite to the screen
};
