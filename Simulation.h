#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "settings.h"
#include "FluidGrid.h"
#include "ToggleButton.h"
#include <memory>

class Simulation {
public:
    Simulation(); // constructor for simulation
    void run(); // starts the simulation and starts the main loop
    
private:
    std::vector<std::uint8_t> pixelBuffer; // vector storing each R, G, B, and A value for each pixel
    sf::Texture gridTexture; // texture created that can convert the pixel buffer data into an actual visible grid
    std::unique_ptr<sf::Sprite> gridTextureSpritePtr; // pointer to the grid sprite as the sprite within sfml has no default constructor
    FluidGrid fluidGrid; // fluid grid that runs the backend ( mathematical operations ect. ) of the fluid simulation
    toggleButton showVelocityButton; // button that alters the rendering process to show the velocity through colours
    toggleButton addDensityButton; // button that controls whether density should be added on click
    toggleButton drawObstacleButton; // button that enables the user to draw obstacles into the fluid simulation
    button testButton;
    std::vector<std::uint8_t> pixelColourValues; // initialize temporary vector to store R, G, B, and A values

    void assignDensityToPixelBuffer(float density, int x, int y); // method that takes a density value and assigns it corresponding R, G, B, and A values to a return a tone of grey
    void assignVelocityAndDensityToPixelBuffer(float density, float velocityX, float velocityY, int x, int y); // method that takes density and velocity values and assigns it corresponding R, G, B, and A values
    void assignObstacleToPixelBuffer(bool obstacle, int x, int y);
    void updateGridTexture(); // method that converts the pixel buffer to the 2d grid and assigns it to a sprite
    void updatePixelBuffer(); // method that takes the density values from the fluid grid and assigns them to the pixel buffer
    void checkForMouseInput(sf::RenderWindow& window); // method that checks if there is any valid input such as clicking a button or drawing to the fluid grid
};
