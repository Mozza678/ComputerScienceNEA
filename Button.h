#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class button {
protected:
    sf::Texture idleTexture;
    sf::Texture pressedTexture;
    std::unique_ptr<sf::Sprite> buttonSprite;

    float xPos; // stores the x co-ordinate of the origin of the button ( top left corner )
    float yPos; // stores the y co-ordinate of the origin of the button ( top left corner )
    int xSize; // stores the size along the x axis of the button, measured in pixels
    int ySize; // stores the size along the y axis of the button, measured in pixels
    sf::Clock elapsedTime; // stores the time passed since the last time the button was pressed

    

public:
    button(float xPos, float yPos, int xSize, int ySize, std::filesystem::__cxx11::path pressedTexture, std::filesystem::__cxx11::path idleTexture);
    
    bool isPressed; // stores the state of the button ( whether it is active or inactive, "on" or "off" )
    bool stateChanged; // true if the button has been pressed, set back to false after the relevant actions have been taken
    virtual void render(sf::RenderWindow& window); // draws the button to the screen and changes the sprite/texture used to draw it depending on its state
    bool checkIfHoveringOver(int mousePosX, int mousePosY); // checks if the mouse is within the bounds of the button, returns true if it is
    float getElapsedTime();
};