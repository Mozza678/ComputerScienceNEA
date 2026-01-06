#include "Simulation.h"
#include "FluidGrid.h"
#include "settings.h"
#include <vector>
#include <algorithm>

Simulation::Simulation() // simulation constructor
    : pixelBuffer(4 * gridWidth * gridWidth), // define the pixel buffer
      gridTexture({static_cast<unsigned int>(gridWidth), static_cast<unsigned int>(gridWidth)}), // define the grid texture
      fluidGrid(gridWidth)
    {
        std::fill(pixelBuffer.begin(), pixelBuffer.end(), 255); // fill the pixel buffer with 255
        gridTextureSpritePtr = std::make_unique<sf::Sprite>(gridTexture); // create pointer to the grid texture sprite
        (*gridTextureSpritePtr).setScale({static_cast<float>(scale), static_cast<float>(scale)}); // set the scale of the sprite to ensure it fills the screen
    }

std::vector<std::uint8_t> Simulation::assignDensityToPixelBuffer(float density, int x, int y){
    std::vector<std::uint8_t> pixelColourValues; // create single dimension vector of pixel colour values in the format RGBA
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 255); // convert the float density value into a value inbetween 0 and 255
    for (int i = 0; i < 3; i++) {
        pixelColourValues.push_back(integerDensity); // add the corresponding values to the vector to create a colour between black and white that corresponds to the density of that box
    }
    pixelColourValues.push_back(255); // set the A value to 255 to ensure visibility
    for (int i = 0; i < pixelColourValues.size(); i++) {
        pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i]; // add the pixel colour values found from the function above to the correct box located through cartesian co-ordinates
    }
    return pixelColourValues;
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
        checkForMouseInput(window);
        fluidGrid.copyDensityGrid();
        fluidGrid.diffuse(diffRate, deltaTime);
        updatePixelBuffer();
        updateGridSprite();
        window.clear({0,255,0}); // wipe the previous frame
        window.draw(*gridTextureSpritePtr); // draw the sprite to the screen
        window.display(); // display new updates
    }
};

void Simulation::updatePixelBuffer() {
    for (int y = 0; y < gridWidth; ++y) {   // loop through all y co-ordinates
        for (int x = 0; x < gridWidth; ++x) {   // loop through all x co-ordinates
            float density = fluidGrid.getValue(x, y);    // retrieve density value from fluid grid at that specific co-ordinate
            assignDensityToPixelBuffer(density, x, y);
        }
    }
}

void Simulation::checkForMouseInput(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int gridX = mousePos.x / scale;
            int gridY = mousePos.y / scale;
            if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridWidth) {
                fluidGrid.setValue(gridX, gridY, 100.0f);
            }
        }
}