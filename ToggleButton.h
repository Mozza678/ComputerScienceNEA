#pragma once
#include "button.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class toggleButton : public button {
private:

public:
    toggleButton(float xPos, float yPos, int xSize, int ySize, std::filesystem::__cxx11::path activeTexture, std::filesystem::__cxx11::path inactiveTexture); // constructor for button
    virtual void render(sf::RenderWindow& window) override;
};