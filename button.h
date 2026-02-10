#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class button {
private:
    sf::Texture textureActive; // stores the texture to be shown when the button is in its "off" state
    sf::Texture textureInactive; // stores the texture to be shown when the button is in its "on" state
    std::unique_ptr<sf::Sprite> buttonSprite; // creates a pointer to a sprite object, this is done as the sprite object in sfml doesn't have a default constructor

    float xPos; // stores the x co-ordinate of the origin of the button ( top left corner )
    float yPos; // stores the y co-ordinate of the origin of the button ( top left corner )
    int xSize; // stores the size along the x axis of the button, measured in pixels
    int ySize; // stores the size along the y axis of the button, measured in pixels
    sf::Clock elapsedTime; // stores the time passed since the last time the button was pressed
public:
    button(float xPos, float yPos, int xSize, int ySize); // constructor for button

    float getElapsedTime(); // returns the elapsed time since the button was last pressed
    void render(sf::RenderWindow& window); // draws the button to the screen and changes the sprite/texture used to draw it depending on its state
    bool checkIfHoveringOver(int mousePosX, int mousePosY); // checks if the mouse is within the bounds of the button, returns true if it is
    bool isPressed; // stores the state of the button ( whether it is active or inactive, "on" or "off" )
    bool stateChanged; // true if the button has been pressed, set back to false after the relevant actions have been taken
};