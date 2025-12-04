#include "Simulation.h"
#include <vector>
#include <algorithm>

Simulation::Simulation() 
    : pixelBuffer(4 * gridWidth * gridWidth),
      gridTexture({static_cast<unsigned int>(gridWidth), static_cast<unsigned int>(gridWidth)})
    {
        gridTextureSpritePtr = std::make_unique<sf::Sprite>(gridTexture);
        (*gridTextureSpritePtr).setScale({static_cast<float>(scale), static_cast<float>(scale)});
    }

std::vector<std::uint8_t> Simulation::convertDensityToPixelColourValues(float density){
    std::vector<std::uint8_t> pixelColourValues;
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 255);
    for (int i = 0; i < 3; i++) {
        pixelColourValues.push_back(integerDensity);
    }
    pixelColourValues.push_back(255);
    return pixelColourValues;
};

void Simulation::assignPixelColourValuesToPixelBuffer(std::vector<std::uint8_t> pixelColourValues, int x, int y){
    for (int i = 0; i < pixelColourValues.size(); i++) {
        pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i];
    }
};

void Simulation::updateGridSprite() {
    gridTexture.update(pixelBuffer.data());
};

void Simulation::run(){

    sf::RenderWindow window(sf::VideoMode({500, 500}), "Fluid Simulator");
    //window.setFramerateLimit(60);

    pixelBuffer[20200] = 0;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear({0,255,0});
        updateGridSprite();
        window.draw(*gridTextureSpritePtr); // to fix this create a seperate texture function and then call the texture function and then the sprite function within the constructor for simulation
        window.display();
    }
};