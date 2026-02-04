#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "settings.h"
#include "FluidGrid.h"

class Simulation {
public:
    Simulation();
    void run();
    
private:
    std::vector<std::uint8_t> pixelBuffer;
    std::vector<std::uint8_t> assignDensityToPixelBuffer(float density, int x, int y);
    sf::Texture gridTexture;
    std::unique_ptr<sf::Sprite> gridTextureSpritePtr;
    FluidGrid fluidGrid;

    void updateGridTexture();
    void updatePixelBuffer();
    void checkForMouseInput(sf::RenderWindow& window);
};
