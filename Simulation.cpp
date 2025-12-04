#include "Simulation.h"
#include <vector>
#include <algorithm>

Simulation::Simulation() // simulation constructor
    : pixelBuffer(4 * gridWidth * gridWidth), // define the pixel buffer
      gridTexture({static_cast<unsigned int>(gridWidth), static_cast<unsigned int>(gridWidth)}) // define the grid texture
    {
        std::fill(pixelBuffer.begin(), pixelBuffer.end(), 255); // fill the pixel buffer with 255
        gridTextureSpritePtr = std::make_unique<sf::Sprite>(gridTexture); // create pointer to the grid texture sprite
        (*gridTextureSpritePtr).setScale({static_cast<float>(scale), static_cast<float>(scale)}); // set the scale of the sprite to ensure it fills the screen
    }

std::vector<std::uint8_t> Simulation::convertDensityToPixelColourValues(float density){
    std::vector<std::uint8_t> pixelColourValues; // create single dimension vector of pixel colour values in the format RGBA
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 255); // convert the float density value into a value inbetween 0 and 255
    for (int i = 0; i < 3; i++) {
        pixelColourValues.push_back(integerDensity); // add the corresponding values to the vector to create a colour between black and white that corresponds to the density of that box
    }
    pixelColourValues.push_back(255); // set the A value to 255 to ensure visibility
    return pixelColourValues;
};

void Simulation::assignPixelColourValuesToPixelBuffer(std::vector<std::uint8_t> pixelColourValues, int x, int y){
    for (int i = 0; i < pixelColourValues.size(); i++) {
        pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i]; // add the pixel colour values found from the function above to the correct box located through cartesian co-ordinates
    }
};

void Simulation::updateGridSprite() {
    gridTexture.update(pixelBuffer.data()); // update the texture with the pixel buffer data
};

void Simulation::run(){

    sf::RenderWindow window(sf::VideoMode({500, 500}), "Fluid Simulator"); // set up the window
    window.setFramerateLimit(60); // set frame rate to 60

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) { // check for SFML events
            if (event->is<sf::Event::Closed>()) {
                window.close(); // close window if requested by user
            }
        }
        window.clear({0,255,0}); // wipe the previous frame
        updateGridSprite();
        window.draw(*gridTextureSpritePtr); // draw the sprite to the screen
        window.display(); // display new updates
    }
};