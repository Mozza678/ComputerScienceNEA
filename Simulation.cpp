#include "Simulation.h"
#include "FluidGrid.h"
#include "settings.h"
#include "ToggleButton.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <math.h>

Simulation::Simulation()
    : pixelBuffer(4 * gridWidth * gridWidth), // Initialize the pixel buffer with size that allows for 4 values for every pixel.
      gridTexture({static_cast<unsigned int>(gridWidth), static_cast<unsigned int>(gridWidth)}), // Initialize the grid texture with size gridWidth x gridWidth.
      fluidGrid(), // Initialize a fluid grid with size gridWidth x gridWidth.
      // Buttons constructed in the initializer list as button has no default constructor.
      showVelocityButton(static_cast<float>(1 * scale), static_cast<float>(1 * scale + scale * gridWidth), 200, 80, "ShowVelocityGreen.png", "ShowVelocityRed.png"), // Construct the showVelocityButton with the correct position, size, and textures.
      addDensityButton(static_cast<float>((2 * scale) + 200), static_cast<float>(1 * scale + scale * gridWidth), 200, 80, "AddDensityGreen.png", "AddDensityRed.png"), // Construct the addDensityButton with the correct position, size, and textures.
      drawObstacleButton(static_cast<float>((3 * scale) + 400), static_cast<float>(1 * scale + scale * gridWidth), 200, 80, "DrawObstacleGreen.png", "DrawObstacleRed.png"), // Construct the drawObstacle Button with the correct position, size, and textures.
      saveButton(static_cast<float>((4 * scale) + 600), static_cast<float>(1 * scale + scale * gridWidth), 120, 80, "SaveGreen.png", "SaveRed.png"), // Construct the saveButton Button with the correct position, size, and textures.
      loadButton(static_cast<float>((5 * scale) + 720), static_cast<float>(1 * scale + scale * gridWidth), 120, 80, "LoadGreen.png", "LoadRed.png"), // Construct the loadButton Button with the correct position, size, and textures.
      gridSprite(gridTexture) // GridSprite constructed in initializer list as grid sprite has no default constructor.
    {
        std::fill(pixelBuffer.begin(), pixelBuffer.end(), 255); // Fill the pixel buffer with 255.
        gridSprite.setScale({static_cast<float>(scale), static_cast<float>(scale)}); // Set the scale of the sprite to ensure it fills the screen.
    }

void Simulation::assignDensityToPixelBuffer(float density, int x, int y){

    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 250); // Convert the float density value into a value inbetween 0 and 255.
    for (int i = 0; i < 3; i++) { // Loop through R, G, and B.
        pixelColourValues.push_back(integerDensity); // Add the corresponding values to the vector to create a grey that corresponds to the density of that cell.
    }
    pixelColourValues.push_back(255); // Set the A (opacity) value to 255 to ensure visibility.
};

void Simulation::assignVelocityAndDensityToPixelBuffer(float density, float velocityX, float velocityY, int x, int y) {
    int integerDensity = static_cast<int>(std::clamp(density, 0.0f, 1.0f) * 250); // Initialize temporary variable that contains a density value mapped to a value between 0 - 255.
    int velocityMagnitude = std::clamp(static_cast<int>((pow(pow(velocityX, 2) + pow(velocityY, 2), 0.5f)) / pow(2, 0.5) * 255), 0, 63) * 4; // Uses pythagorous to calculate the velocity magnitude and then clamp that value between 0 and 252.

    // The following code represents a mathematical equation that converts from a numerical value between 0 and 255 to a colour between red and blue.

    if (velocityMagnitude < 128) { 
        pixelColourValues.push_back(255 - 2 * velocityMagnitude);
        pixelColourValues.push_back(2 * velocityMagnitude);
        pixelColourValues.push_back(0);
        pixelColourValues.push_back(integerDensity);
    } else if (velocityMagnitude >= 128) {
        pixelColourValues.push_back(0);
        pixelColourValues.push_back(255 - (velocityMagnitude - 128) * 2);
        pixelColourValues.push_back((velocityMagnitude - 128) * 2);
        pixelColourValues.push_back(integerDensity);
    };
}

void Simulation::assignObstacleToPixelBuffer(bool obstacle, int x, int y) {
    if (obstacle) {
        // Sets obstacle cells to White for drawing/edit modes.
        for (int i = 0; i < 4; i++) {
            pixelColourValues.push_back(255);
        }
    } else {
        // Sets non-obstacle cells to the background color (Black/Empty).
        for (int i = 0; i < 3; i++) {
            pixelColourValues.push_back(0);
        }
        pixelColourValues.push_back(255);
    }
}

void Simulation::updateGridTexture() {
    gridTexture.update(pixelBuffer.data()); // Update the texture with the pixel buffer data.
};

void Simulation::run(){

    sf::RenderWindow window(sf::VideoMode({1000, gridWidth * scale + 100}), "Fluid Simulator"); // Intitialize the window with correct size and name, width hard-coded at 1000 pixels to make space for buttons.
    window.setFramerateLimit(60); // Set maximum frame rate to 60 to avoid high usage of hardware that could lead to overheating.

    gridTexture.setSmooth(true); // Setting within sfml to enable texture smoothing, very similiar to anti-aliasing. Reduces the pixelated look of the fluid simulation slightly.

    while (window.isOpen()) { // Loop while the window is still open.
        while (auto event = window.pollEvent()) { // Check for SFML events.
            if (event->is<sf::Event::Closed>()) { // Checks if the window is requested to be closed.
                window.close(); // Close window if the user requested it.
            }
        }
        checkForMouseInput(window); // Checks for any form of valid mouse input and acts accordingly.
        fluidGrid.step(); // Move the fluid simulator forward by one step.
        updatePixelBuffer();
        updateGridTexture();
        window.clear({255,255,255}); // Wipe the previous frame.
        window.draw(gridSprite); // Draw the sprite to the screen.
        showVelocityButton.render(window); // Draw the showVelocityButton to the screen.
        addDensityButton.render(window); // Draw the addDensityButton to the screen.
        drawObstacleButton.render(window); // Draw the drawObstacleButton to the screen.
        saveButton.render(window); // Draw the saveButton to the screen.
        loadButton.render(window); // Draw the loadButton to the screen.
        window.display(); // Display the new window after all entities have been rendered.
    }
};

void Simulation::updatePixelBuffer() {
    for (int y = 0; y < gridWidth; y++) {
        for (int x = 0; x < gridWidth; x++) {
            pixelColourValues.clear(); // Resets the temporary vector so that it can be written to.
            float density = fluidGrid.getValue(0, x, y); // Retrieves the current density value from the fluid grid for this cell.
            bool obstacle = fluidGrid.getObstacleGridValue(x, y); // Retrieves the current obstacle data for this cell.

            if (drawObstacleButton.isPressed) {
                // if in drawing mode, obstacles appear white and the backround appears black
                assignObstacleToPixelBuffer(obstacle, x, y);
            } else if (obstacle) {
                // In normal mode and showVelocityMode, obstacles appear blue
                pixelColourValues.push_back(0);   // Red
                pixelColourValues.push_back(0);   // Green
                pixelColourValues.push_back(255); // Blue
                pixelColourValues.push_back(255); // Opacity
            } else if (showVelocityButton.isPressed) {
                // show velocity as a colour gradient
                float velocityX = fluidGrid.getValue(1, x, y);
                float velocityY = fluidGrid.getValue(2, x, y);
                assignVelocityAndDensityToPixelBuffer(density, velocityX, velocityY, x, y);
            } else {
                // Default: show fluid density as grayscale
                assignDensityToPixelBuffer(density, x, y);
            }

            for (int i = 0; i < pixelColourValues.size(); i++) {
                pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i]; // add the the pixelColourValues for the current cell to the pixelBuffer
            }
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

    int obstacleGridX = mousePos.x / (scale * 2); // use the scale to convert the mouse pixel co-ordinate to a cell x co-ordinate within the obstacle grid
    int obstacleGridY = mousePos.y / (scale * 2); // use the scale to convert the mouse pixel co-ordinate to a cell y co-ordinate within the obstacle grid

    if (gridX > 0 && gridX < gridWidth - 1 && gridY > 0 && gridY < gridWidth - 1) { // checks if the mouse is within the bounds of the fluid grid
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !drawObstacleButton.isPressed) { // checks if the left mouse button is pressed
            if (addDensityButton.isPressed) {
                for (int xSize = -brushSize; xSize <= brushSize; xSize++) { // loops through the neighbours from the left to the right allowing multiple cells to be painted with each click, this creates more of a paint brush feel
                    for (int ySize = -brushSize; ySize <= brushSize; ySize++) { // loops through the neighbours from the bottom to the top
                        float currentDensity = fluidGrid.getValue(0, gridX + xSize, gridY + ySize); // assigns the current density of each cell within the brushes limits to a temporary variable
                        if (currentDensity + fluidAddedOnClick <= 1.0f) { // ensures that a density value greater than 1.0f isn't added
                            fluidGrid.setValue(0, gridX + xSize, gridY + ySize, (currentDensity + fluidAddedOnClick)); // adds the density value in settings to the current grid cell
                        } else {
                            fluidGrid.setValue(0, gridX + xSize, gridY + ySize, (1.0f)); // adds the maximum density value (1.0f) if the density wouldve exceeded this value
                        };                    
                    }
                }
            };

            float currentVX = fluidGrid.getValue(1, gridX, gridY); // assigns the current x velocity to a temporary variable
            float currentVY = fluidGrid.getValue(2, gridX, gridY); // assigns the current y velocity to a temporary variable
                
            fluidGrid.setValue(1, gridX, gridY, currentVX + (static_cast<float>(deltaX) * velocityAddedOnClick)); // adds x velocity at the current cell depending on the speed of the mouse ( calculated based on the distance moved since last frame)
            fluidGrid.setValue(2, gridX, gridY, currentVY + (static_cast<float>(deltaY) * velocityAddedOnClick)); // adds y velocity at the current cell depending on the speed of the mouse ( calculated based on the distance moved since last frame)
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && drawObstacleButton.isPressed) { // checks if the user is in drawObstacle mode and attempting to add new obstacles
            // adds a two by two square onto the obstacle grid
            // it is a two by two square due to the logic of the setBoundaries function within the fluidGrid class
            fluidGrid.setObstacleGridValue(obstacleGridX * 2, obstacleGridY * 2, true);
            fluidGrid.setObstacleGridValue(obstacleGridX * 2 + 1, obstacleGridY * 2, true);
            fluidGrid.setObstacleGridValue(obstacleGridX * 2, obstacleGridY * 2 + 1, true);
            fluidGrid.setObstacleGridValue(obstacleGridX * 2 + 1, obstacleGridY * 2 + 1, true);
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && drawObstacleButton.isPressed) { // checks if the user is in drawObstacle mode and attempting to remove obstacles
            // removes a two by two square from the obstacle grid
            fluidGrid.setObstacleGridValue(obstacleGridX * 2, obstacleGridY * 2, false);
            fluidGrid.setObstacleGridValue(obstacleGridX * 2 + 1, obstacleGridY * 2, false);
            fluidGrid.setObstacleGridValue(obstacleGridX * 2, obstacleGridY * 2 + 1, false);
            fluidGrid.setObstacleGridValue(obstacleGridX * 2 + 1, obstacleGridY * 2 + 1, false);
        };

    } else { // checks to see if the mouse is within the bounds of the button section
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { // checks to see if the left mouse button is clicked
            if (showVelocityButton.checkIfHoveringOver(mousePos.x, mousePos.y) && showVelocityButton.getElapsedTime() > 0.5f) { // checks to see if the mouse is within the bounds of the show velocity button and if the button wasn't pressed within the last half second
                showVelocityButton.isPressed = !showVelocityButton.isPressed; // sets the button to the opposite of itself ( on to off, off to on )
                showVelocityButton.stateChanged = true; // tells the button class that the button has been pressed causing it to carry out the relevant action
                drawObstacleButton.isPressed = false; // turns off the draw obstacle button as they can't be active at the same time
                drawObstacleButton.stateChanged = true; // tells the draw obstacle button that the state may have changed
            } else if (addDensityButton.checkIfHoveringOver(mousePos.x, mousePos.y) && addDensityButton.getElapsedTime() > 0.5f) {
                addDensityButton.isPressed = !addDensityButton.isPressed; // sets the button to the opposite of itself ( on to off, off to on )
                addDensityButton.stateChanged = true; // tells the button class that the button has been pressed causing it to carry out the relevant action
                drawObstacleButton.isPressed = false; // turns off the draw obstacle button as they can't be active at the same time
                drawObstacleButton.stateChanged = true; // tells the draw obstacle button that the state may have changed
            } else if (drawObstacleButton.checkIfHoveringOver(mousePos.x, mousePos.y) && drawObstacleButton.getElapsedTime() > 0.5f) {
                if (drawObstacleButton.isPressed) { // checks if the drawObstacleButton is currently active and sets it to idle if so
                    drawObstacleButton.isPressed = false;
                    drawObstacleButton.stateChanged = true;
                } else { // if the drawObstacleButton is idle it turns it to active and turns both other buttons off
                    drawObstacleButton.isPressed = true;
                    drawObstacleButton.stateChanged = true;
                    addDensityButton.isPressed = false;
                    addDensityButton.stateChanged = true;
                    showVelocityButton.isPressed = false;
                    showVelocityButton.stateChanged = true;
                }
            } else if (saveButton.checkIfHoveringOver(mousePos.x, mousePos.y) && saveButton.getElapsedTime() > 2.0f) {
                saveButton.restartElapsedTime();
                saveButton.isPressed = true;
                saveObstaclePlacement();
            } else if (loadButton.checkIfHoveringOver(mousePos.x, mousePos.y) && loadButton.getElapsedTime() > 2.0f){
                loadButton.restartElapsedTime();
                loadButton.isPressed = true;
                loadObstaclePlacement();
            }
        }
    };

    lastMousePos = mousePos; // sets the static variable to the mouse position for this frame allowing it to referenced in the next frame
};

void Simulation::saveObstaclePlacement() {
    std::ofstream saveFile;
    saveFile.open("ObstaclePositionSaveFile.txt", std::ios::out);
    if (saveFile.is_open()) {
        for (int y = 0; y < gridWidth; y++) {
            for (int x = 0; x < gridWidth; x++) {
                saveFile << fluidGrid.getObstacleGridValue(x, y) << '\n';
            }
        }
    }
    saveFile.close();
}

void Simulation::loadObstaclePlacement() {
    std::ifstream saveFile;
    saveFile.open("ObstaclePositionSaveFile.txt", std::ios::in);
    std::string line;
    int count = 0;
    if (saveFile.is_open()) {
        while (std::getline(saveFile, line)) {
            if (std::stoi(line) == 1) {
                fluidGrid.setObstacleGridValue((count % gridWidth), (count / gridWidth), true);
            } else {
                fluidGrid.setObstacleGridValue((count % gridWidth), (count / gridWidth), false);
            }
            count++;
        }
    }
    saveFile.close();
}