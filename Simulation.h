#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "settings.h"
#include "FluidGrid.h"
#include "ToggleButton.h"
#include <memory>
#include <fstream>

class Simulation {
public:
    Simulation(); // Constructor for simulation.
    void run(); // Starts the simulation and starts the render loop.
    
private:
    FluidGrid fluidGrid; // Fluid grid that runs the backend ( mathematical operations ect. ) of the fluid simulation.
    std::vector<std::uint8_t> pixelBuffer; // Vector storing each R, G, B, and A value for each pixel.
    std::vector<std::uint8_t> pixelColourValues; // A temporary vector to store R, G, B, and A values for a single pixel.
    sf::Texture gridTexture; // Texture created that converts the pixel buffer data into an actual visible grid.
    sf::Sprite gridSprite; // Sprite for the texture to be assigned so that it can be shown to the screen.
    toggleButton showVelocityButton; // Button that alters the rendering process to show the velocity through a colour gradient.
    toggleButton addDensityButton; // Button that controls whether density should be added on clicking of the left mouse button.
    toggleButton drawObstacleButton; // Button that enables the user to draw obstacles into the fluid simulation.
    button saveButton; // Button that saves the current postitions of all obstacles to an external text file.
    button loadButton; // Button that loads obstacle positions from an external text file.

    void assignDensityToPixelBuffer(float density, int x, int y); // Method that takes a density value and assigns it corresponding R, G, B, and A that represent a tone of grey that corresponds to the density value.
    void assignObstacleToPixelBuffer(bool obstacle, int x, int y); // Method that assigns the obstacle positions as white and the remaining cells as black.
                                                                   // This creates a clutter free screen making it easy to draw obstacles.
    void updateGridTexture(); // Method that converts the pixel buffer to the 2d grid and assigns it to a sprite.
    void updatePixelBuffer(); // Method that takes the grid values from the fluid grid and assigns them to the pixel buffer.
                              // This method behaves differently depending on which mode the user is in.
    void checkForMouseInput(sf::RenderWindow& window); // Method that checks if there is any valid input such as clicking a button or drawing to the fluid grid and then alters the simulation to reflect the input.
    void saveObstaclePlacement(); // Method that saves the positions of all obstacles to "ObstaclePositionSaveFile.txt".
    void loadObstaclePlacement(); // Method that loads the positions of all obstacles to "ObstaclePositionSaveFile.txt".
};
