#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <filesystem>
#include <memory>

class button { // Base class for toggleButton. 
               // Protected attributes used instead of private so that toggleButton can inherit them.
protected:
    sf::Texture idleTexture; // Texture of the button when not pressed.
    sf::Texture pressedTexture; // Texture of the button for a small time when pressed. 
                                // This will behave differently for toggleButton and will instead flip between the two textures depending on whether the button has been activated by the user.
    sf::Sprite buttonSprite; // The texture of the button is assigned to the sprite so it can be drawn to the screen.

    float xPos; // Stores the x co-ordinate of the origin of the button (top left corner).
    float yPos; // Stores the y co-ordinate of the origin of the button (top left corner).
    int xSize; // Stores the size along the x axis of the button, measured in pixels.
    int ySize; // Stores the size along the y axis of the button, measured in pixels.
    sf::Clock elapsedTime; // Stores the time passed since the last time the button was pressed.

public:
    button(float xPos, float yPos, int xSize, int ySize, std::filesystem::path pressedTexturePath, std::filesystem::path idleTexturePath); // Constructor for the button, takes the size, positions and path for both textures as parameters.
    
    bool isPressed; // Stores the state of the button (whether it is active or inactive, "on" or "off").
    bool stateChanged; // True if the button has been pressed, set back to false after the relevant actions have been taken.
    virtual void render(sf::RenderWindow& window); // Draws the button to the screen and changes the sprite/texture used to draw it depending on its state.
    bool checkIfHoveringOver(int mousePosX, int mousePosY); // Checks if the mouse is within the bounds of the button, returns true if it is.
    float getElapsedTime(); // Returns the time since the button was last pressed in seconds as a float.
    void restartElapsedTime(); // Restarts the elapsed time. should be called after every button press.
};
