#include "Simulation.h"
#include "FluidGrid.h"
#include "settings.h"
#include "button.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <math.h>

Simulation::Simulation() // simulation constructor
    : pixelBuffer(4 * gridWidth * gridWidth), // initialize the pixel buffer
      gridTexture({static_cast<unsigned int>(gridWidth),
      static_cast<unsigned int>(gridWidth)}), // define the grid texture
      fluidGrid(),
      showVelocityButton(static_cast<float>(1 * scale), static_cast<float>(1 * scale + scale * gridWidth), 200, 80)
    {
        std::fill(pixelBuffer.begin(), pixelBuffer.end(), 255); // fill the pixel buffer with 255
        gridTextureSpritePtr = std::make_unique<sf::Sprite>(gridTexture); // create pointer to the grid texture sprite
        (*gridTextureSpritePtr).setScale({static_cast<float>(scale), static_cast<float>(scale)}); // set the scale of the sprite to ensure it fills the screen
    }

void Simulation::assignDensityToPixelBuffer(float density, int x, int y){
    std::vector<std::uint8_t> pixelColourValues; // create single dimension vector of pixel colour values in the format RGBA
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 255); // convert the float density value into a value inbetween 0 and 255
    for (int i = 0; i < 3; i++) {
        pixelColourValues.push_back(integerDensity); // add the corresponding values to the vector to create a colour between black and white that corresponds to the density of that box
    }
    pixelColourValues.push_back(255); // set the A value to 255 to ensure visibility
    for (int i = 0; i < pixelColourValues.size(); i++) {
        pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i]; // add the pixel colour values found from the function above to the correct box located through cartesian co-ordinates
    }
};

void Simulation::assignVelocityAndDensityToPixelBuffer(float density, float velocityX, float velocityY, int x, int y) {
    std::vector<std::uint8_t> pixelColourValues;
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 255);
    int velocityMagnitude = std::clamp(static_cast<int>((pow(pow(velocityX, 2) + pow(velocityY, 2), 0.5f)) / pow(2, 0.5) * 255), 0, 64) * 4;
    if (velocityMagnitude < 128) {
        pixelColourValues.push_back(255 - 2 * velocityMagnitude);
        pixelColourValues.push_back(2 * velocityMagnitude);
        pixelColourValues.push_back(0);
        pixelColourValues.push_back(integerDensity);
    } else if (velocityMagnitude >= 128) {
        pixelColourValues.push_back(0);
        pixelColourValues.push_back(256 - (velocityMagnitude - 128) * 2);
        pixelColourValues.push_back((velocityMagnitude - 128) * 2);
        pixelColourValues.push_back(integerDensity);
    };
    for (int i = 0; i < pixelColourValues.size(); i++) {
        pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i]; // add the pixel colour values found from the function above to the correct box located through cartesian co-ordinates
    };
}

void Simulation::updateGridTexture() {
    gridTexture.update(pixelBuffer.data()); // update the texture with the pixel buffer data
};

void Simulation::run(){

    sf::RenderWindow window(sf::VideoMode({gridWidth * scale, gridWidth * scale + 100}), "Fluid Simulator"); // set up the window
    window.setFramerateLimit(60); // set frame rate to 60

    gridTexture.setSmooth(true);

    fluidGrid.setup();

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) { // check for SFML events
            if (event->is<sf::Event::Closed>()) {
                window.close(); // close window if requested by user
            }
        }
        checkForMouseInput(window);
        fluidGrid.step();
        updatePixelBuffer();
        updateGridTexture();
        window.clear({255,255,255}); // wipe the previous frame
        window.draw(*gridTextureSpritePtr); // draw the sprite to the screen
        showVelocityButton.render(window);
        window.display(); // display new updates
    }
};

void Simulation::updatePixelBuffer() {
    for (int y = 0; y < gridWidth; ++y) {   // loop through all y co-ordinates
        for (int x = 0; x < gridWidth; ++x) {   // loop through all x co-ordinates
            float density = fluidGrid.getValue(fluidGrid.densityGrid, x, y);    // retrieve density value from fluid grid at that specific co-ordinate
            if (showVelocityButton.isPressed) {
                float velocityX = fluidGrid.getValue(fluidGrid.xvelocityGrid, x, y);
                float velocityY = fluidGrid.getValue(fluidGrid.yvelocityGrid, x, y);
                assignVelocityAndDensityToPixelBuffer(density, velocityX, velocityY, x, y);
            } else {
                assignDensityToPixelBuffer(density, x, y);
            }
        }
    }
}

void Simulation::checkForMouseInput(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    
    static sf::Vector2i lastMousePos;

    int deltaX = mousePos.x - lastMousePos.x;
    int deltaY = mousePos.y - lastMousePos.y;

    int gridX = mousePos.x / scale;
    int gridY = mousePos.y / scale;

    if (gridX > 0 && gridX < gridWidth - 1 && gridY > 0 && gridY < gridWidth - 1) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

            for (int xSize = -brushSize; xSize <= brushSize; xSize++) {
                for (int ySize = -brushSize; ySize <= brushSize; ySize++) {
                    float currentDensity = fluidGrid.getValue(fluidGrid.densityGrid, gridX + xSize, gridY + ySize);
                    if (currentDensity + fluidAddedOnClick <= 1.0f) {
                        fluidGrid.setValue(fluidGrid.densityGrid, gridX + xSize, gridY + ySize, (currentDensity + fluidAddedOnClick));   
                    } else {
                        fluidGrid.setValue(fluidGrid.densityGrid, gridX + xSize, gridY + ySize, (1.0f));  
                    };                    
                }
            }

            float currentVX = fluidGrid.getValue(fluidGrid.xvelocityGrid, gridX, gridY);
            float currentVY = fluidGrid.getValue(fluidGrid.yvelocityGrid, gridX, gridY);
            
            fluidGrid.setValue(fluidGrid.xvelocityGrid, gridX, gridY, currentVX + (static_cast<float>(deltaX) * velocityAddedOnClick));
            fluidGrid.setValue(fluidGrid.yvelocityGrid, gridX, gridY, currentVY + (static_cast<float>(deltaY) * velocityAddedOnClick));
        }

    } else if (gridX > 0 && gridX < gridWidth - 1 && gridY > gridWidth - 1 && gridY < gridWidth + buttonPanelSize) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { 
            if (showVelocityButton.checkIfHoveringOver(mousePos.x, mousePos.y) && showVelocityButton.elapsedTime.getElapsedTime().asSeconds() > 0.5f) {
                showVelocityButton.isPressed = !showVelocityButton.isPressed;
                showVelocityButton.stateChanged = true;
            }
        }
    };

    lastMousePos = mousePos; 
}