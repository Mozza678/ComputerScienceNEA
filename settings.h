#pragma once

const int gridWidth = 100; // size of the grid both horizontally and vertically, measured in the amount of cells
const int scale = 10; // pixels per cell width
const float deltaTime = 0.04f; // ??
const float diffRate = 0.0001f; // a multiplyer used to alter the amount of diffusion that takes place per time step
const float fluidAddedOnClick = 0.01f; // a multiplyer to alter the amount of density that is added per click
const float velocityAddedOnClick = 5.0f; // a multiplyer to alter the amount of velocity that is added per click
const int brushSize = 4; // the distance that the brush will spread from the centre cell ( if the brush size is one it will cover a 3 x 3 grid )
const int buttonPanelSize = 10; // the amount of cells that is extended from the bottom of the grid to create a panel for the buttons to be placed
