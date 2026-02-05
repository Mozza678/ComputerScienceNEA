#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "settings.h"
#include "FluidGrid.h"
#include "button.h"
#include <memory>

class Simulation {
public:
    Simulation();
    void run();
    button showVelocityButton;
    
private:
    std::vector<std::uint8_t> pixelBuffer;
    void assignDensityToPixelBuffer(float density, int x, int y);
    void assignVelocityAndDensityToPixelBuffer(float density, float velocityX, float velocityY, int x, int y);
    sf::Texture gridTexture;
    std::unique_ptr<sf::Sprite> gridTextureSpritePtr;
    FluidGrid fluidGrid;

    void updateGridTexture();
    void updatePixelBuffer();
    void checkForMouseInput(sf::RenderWindow& window);
};
