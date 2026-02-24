#pragma once
#include "button.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class toggleButton : public button { // derived class inherited from the base class "button"
private:

public:
    toggleButton(float xPos, float yPos, int xSize, int ySize, std::filesystem::path pressedTexturePath, std::filesystem::path idleTexturePath); // constructor for toggleButton
                                                                                                                                                 // takes the same parameters as button    
virtual void render(sf::RenderWindow& window) override; // virtual method render inherited from the base class is overriden
};
