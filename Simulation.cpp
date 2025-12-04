#include <Simulation.h>
#include <vector>
#include <algorithm>
#include <settings.h>

Simulation::Simulation() 
    : pixelBuffer(4 * gridWidth * gridWidth){}

std::vector<std::uint8_t> Simulation::convertDensityToPixelColourValues(float density){
    std::vector<std::uint8_t> pixelColourValues;
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 255);
    for (int i = 1; i < 4; i++) {
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

void Simulation::createGridSprite() {
    sf::Vector2u textureSize = {static_cast<unsigned int>(gridWidth), static_cast<unsigned int>(gridWidth)};
    sf::Vector2f scaleVector = {static_cast<unsigned int>(scale), static_cast<unsigned int>(scale)};
    sf::Texture gridTexture(textureSize);
    gridTexture.update(pixelBuffer.data());
    sf::Sprite gridTextureSprite(gridTexture);
    gridTextureSprite.setScale(scaleVector);
};

void Simulation::run(){
    createGridSprite();

    sf::RenderWindow window(sf::VideoMode({500, 500}), "Fluid Simulator");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.draw(gridTextureSprite); // to fix this create a seperate texture function and then call the texture function and then the sprite function within the constructor for simulation
        window.display();
    }
};