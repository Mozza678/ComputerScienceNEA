#pragma once

const int gridWidth = 50; // Width and height of the simulation grid, measured in the amount of cells.
                          // If this is increased the area where the fluid can flow will be larger.
const int scale = 10; // Width and height of each cell, measured in the amount of pixels.
                      // If this is increased the fluid will appear more blurry and the area where the fluid can flow will be bigger with the same gridWidth.
const float deltaTime = 0.04f; // A scaling factor for both the speed of the fluid when being moved by velocity and diffusion.
                               // If this is increased the fluid will appear to both move and spread out faster.
const float diffRate = 0.0001f; // A multiplyer used to alter the amount of diffusion that takes place per time step.
                                // If this is increased the amount the fluid spreads per frame will be increased.
const float fluidAddedOnClick = 0.03f; // A multiplyer to alter the amount of density that is added per click in each cell.
const float velocityAddedOnClick = 5.0f; // A multiplyer to alter the amount of velocity that is added per click in each cell.
const int brushSize = 4; // The distance that the brush will spread from the centre cell when drawing density onto the screen (Doesn't effect the obstacle drawing mode).
                         // If the brush size is one it will cover a 3 x 3 grid.
const int buttonPanelSize = 10; // The amount of cells that is extended from the bottom of the grid to create a panel for the buttons to be placed.