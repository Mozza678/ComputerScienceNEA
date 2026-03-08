#include "button.h"
#include <memory>
#include <iostream>

button::button(float xPos, float yPos, int xSize, int ySize, std::filesystem::path pressedTexturePath, std::filesystem::path idleTexturePath)
    : buttonSprite(idleTexture) // Sets the initial sprite for the button to the "idle" texture to match the isPressed variable, this is done in the intializer list as sprite has no default constructor.
    {

    // All default and specified values are assigned to each of the attributes.

    this->xPos = xPos;
    this->yPos = yPos;
    this->xSize = xSize;
    this->ySize = ySize;
    elapsedTime.restart();
    isPressed = false;

    if (this->pressedTexture.loadFromFile(pressedTexturePath) && this->idleTexture.loadFromFile(idleTexturePath)) { // Attempts to load the texture and returns true or false depending on if they loaded succesfully.
        std::cout << "textures loaded"; // Prints success message to the terminal if the textures load.
    } else {
        std::cout << "textures failed to load"; // Prints error message to the terminal if the textures fail to load.
    };
    
    buttonSprite.setTexture(idleTexture, true); // Sets the texture again as it has now been loaded from memory.
                                                // The second boolean parameter here forces the sprite to update its dimensions as it was set to zero on initial construction.
    buttonSprite.setPosition({xPos, yPos}); // Sets the position of the sprite.
    };

void button::render(sf::RenderWindow& window) { // Draws the button to the screen and changes the sprite if necessary, ran every frame.
    if (isPressed) { // Checks if the button is currently pressed.
        if (getElapsedTime() > 1.0f) { // Checks if the button has been pressed for enough time.
            buttonSprite.setTexture(idleTexture); // Sets the button sprite to the idle texture as the button has been pressed for 1 second.
            isPressed = false; // Returns the button the idle state.
            elapsedTime.restart(); // Restarts the time since last pressed to create a time window where the button can't be pressed again.
        } else {
            buttonSprite.setTexture(pressedTexture); // Sets the button sprite to the inactive texture if the button isn't pressed.
        }
    }
    window.draw(buttonSprite); // Draws the button sprite to the screen.
};

float button::getElapsedTime() {
    return elapsedTime.getElapsedTime().asSeconds(); // Returns the elapsed time of the button (time since last being pressed) in seconds as a float.
}

void button::restartElapsedTime() {
    elapsedTime.restart(); // Restarts the elapsed time.
}

bool button::checkIfHoveringOver(int mousePosX, int mousePosY) { // Takes the mouse position as parameters to be compared to button bounds, the pixel position is used to create more precise bounds for clicking the button.
    if (mousePosX > xPos && mousePosY > yPos && mousePosX < xPos + xSize && mousePosY < yPos + ySize) { // Checks if the mouse is within the edges of the button.
        return true;
    } else {
        return false;
    }
};
