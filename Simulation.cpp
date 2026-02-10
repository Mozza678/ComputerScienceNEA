#include "Simulation.h"
#include "FluidGrid.h"
#include "settings.h"
#include "button.h"
#include <vector>
#include <algorithm>
#include <memory>

Simulation::Simulation() // simulation constructor
    : pixelBuffer(4 * gridWidth * gridWidth), // initialize the pixel buffer
      gridTexture({static_cast<unsigned int>(gridWidth), static_cast<unsigned int>(gridWidth)}), // initialize the grid texture with size gridWidth x gridWidth 
      fluidGrid(gridWidth), // instantiate a fluid grid with size gridWidth x gridWidth
      showVelocityButton(static_cast<float>(1 * scale), static_cast<float>(1 * scale + scale * gridWidth), 200, 80) // construct the showVelocityButton with the correct position and size
    {
        std::fill(pixelBuffer.begin(), pixelBuffer.end(), 255); // fill the pixel buffer with 255
        gridTextureSpritePtr = std::make_unique<sf::Sprite>(gridTexture); // create pointer to the grid texture sprite
        (*gridTextureSpritePtr).setScale({static_cast<float>(scale), static_cast<float>(scale)}); // set the scale of the sprite to ensure it fills the screen
    }

std::vector<std::uint8_t> Simulation::assignDensityToPixelBuffer(float density, int x, int y){
    std::vector<std::uint8_t> pixelColourValues; // create single dimension vector of pixel colour values in the format RGBA
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 255); // convert the float density value into a value inbetween 0 and 255
    for (int i = 0; i < 3; i++) { // loop through R, G, and B
        pixelColourValues.push_back(integerDensity); // add the corresponding values to the vector to create a grey that corresponds to the density of that box
    }
    pixelColourValues.push_back(255); // set the A (opacity) value to 255 to ensure visibility
    for (int i = 0; i < pixelColourValues.size(); i++) { // loop through the temporary RGBA vector
        pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i]; // add the pixel colour values found from the function above to the correct box located through cartesian co-ordinates
    }
    return pixelColourValues;
};

void Simulation::updateGridTexture() {
    gridTexture.update(pixelBuffer.data()); // update the texture with the pixel buffer data
};

void Simulation::run(){

    sf::RenderWindow window(sf::VideoMode({gridWidth * scale, gridWidth * scale + 100}), "Fluid Simulator"); // intitialize the window with correct size and name
    window.setFramerateLimit(60); // set maximum frame rate to 60

    gridTexture.setSmooth(true); // setting within sfml to enable texture smoothing, very similiar to anti-aliasing

    while (window.isOpen()) { // loop while the window is still open
        while (auto event = window.pollEvent()) { // check for SFML events
            if (event->is<sf::Event::Closed>()) { // checks if the window is requested to be closed
                window.close(); // close window
            }
        }
        checkForMouseInput(window); // checks for any form of valid mouse input and acts accordingly
        fluidGrid.step(); // move the fluid simulator forward by one step (deltaTime unit)
        updatePixelBuffer();
        updateGridTexture();
        window.clear({0,255,0}); // wipe the previous frame
        window.draw(*gridTextureSpritePtr); // draw the sprite to the screen
        showVelocityButton.render(window);
        window.display(); // display new updates
    }
};

void Simulation::updatePixelBuffer() { // ***** OVERCOMPLICATED, FIX THIS ( MERGE UPDATEPIXELBUFFER AND ASSIGNDENSITYTOPIXELBUFFER )
    for (int y = 0; y < gridWidth; ++y) {   // loop through all y co-ordinates
        for (int x = 0; x < gridWidth; ++x) {   // loop through all x co-ordinates
            float density = fluidGrid.getValue(fluidGrid.densityGrid, x, y); // retrieve density value from fluid grid at that specific co-ordinate
            assignDensityToPixelBuffer(density, x, y);
        }
    }
}

void Simulation::checkForMouseInput(sf::RenderWindow& window) { 
    sf::Vector2i mousePos = sf::Mouse::getPosition(window); // creates a two element vector containing the x and y position of the mouse in the current frame
    
    static sf::Vector2i lastMousePos; // creates a static vector that outlives the function meaning the co-ordinates of the mouse from the last frame can be stored

    int deltaX = mousePos.x - lastMousePos.x; // the distance that the mouse has moved along the x axis from last frame is calculated
    int deltaY = mousePos.y - lastMousePos.y; // the distance that the mouse has moved along the y axis from last frame is calculated

    int gridX = mousePos.x / scale; // use the scale to convert the mouse pixel co-ordinate to a cell x co-ordinate within the fluid grid
    int gridY = mousePos.y / scale; // use the scale to convert the mouse pixel co-ordinate to a cell y co-ordinate within the fluid grid

    if (gridX > 0 && gridX < gridWidth - 1 && gridY > 0 && gridY < gridWidth - 1) { // checks if the mouse is within the bounds of the fluid grid
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { // checks if the left mouse button is pressed

            for (int xSize = -brushSize; xSize <= brushSize; xSize++) { // loops through the neighbours from the left to the right allowing multiple cells to be painted with each click, this creates more of a paint brush feel
                for (int ySize = -brushSize; ySize <= brushSize; ySize++) { // loops through the neighbours from the bottom to the top
                    float currentDensity = fluidGrid.getValue(fluidGrid.densityGrid, gridX + xSize, gridY + ySize); // assigns the current density of each cell within the brushes limits to a temporary variable
                    if (currentDensity + fluidAddedOnClick <= 1.0f) { // ensures that a density value greater than 1.0f isn't added
                        fluidGrid.setValue(fluidGrid.densityGrid, gridX + xSize, gridY + ySize, (currentDensity + fluidAddedOnClick)); // adds the density value in settings to the current grid cell
                    } else {
                        fluidGrid.setValue(fluidGrid.densityGrid, gridX + xSize, gridY + ySize, (1.0f)); // adds the maximum density value (1.0f) if the density wouldve exceeded this value
                    };                    
                }
            }

            float currentVX = fluidGrid.getValue(fluidGrid.xvelocityGrid, gridX, gridY); // assigns the current x velocity to a temporary variable
            float currentVY = fluidGrid.getValue(fluidGrid.yvelocityGrid, gridX, gridY); // assigns the current y velocity to a temporary variable
            
            fluidGrid.setValue(fluidGrid.xvelocityGrid, gridX, gridY, currentVX + (static_cast<float>(deltaX) * velocityAddedOnClick)); // adds x velocity at the current cell depending on the speed of the mouse ( calculated based on the distance moved since last frame)
            fluidGrid.setValue(fluidGrid.yvelocityGrid, gridX, gridY, currentVY + (static_cast<float>(deltaY) * velocityAddedOnClick)); // adds y velocity at the current cell depending on the speed of the mouse ( calculated based on the distance moved since last frame)
        }

    } else if (gridX > 0 && gridX < gridWidth - 1 && gridY > gridWidth - 1 && gridY < gridWidth + buttonPanelSize) { // checks to see if the mouse is within the bounds of the button section
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { // checks to see if the left mouse button is clicked
            if (showVelocityButton.checkIfHoveringOver(mousePos.x, mousePos.y) && showVelocityButton.elapsedTime.getElapsedTime().asSeconds() > 0.5f) { // checks to see if the mouse is within the bounds of the show velocity button and if the button wasn't pressed within the last half second
                showVelocityButton.isPressed = !showVelocityButton.isPressed; // sets the button to the opposite of itself ( on to off, off to on )
                showVelocityButton.stateChanged = true; // tells the button class that the button has been pressed telling it to carry out the relevant action
            }
        }
    };

    lastMousePos = mousePos; // sets the static variable to the mouse position for this frame allowing it to referenced in the next frame
}
