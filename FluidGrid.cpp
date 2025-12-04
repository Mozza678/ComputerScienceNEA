#include "fluidGrid.h"

FluidGrid::FluidGrid(int gridSize)
    : densityGrid(gridSize * gridSize)
    {

    };

float FluidGrid::getValue(int x, int y) {
    return densityGrid[x + gridWidth * y];
};

void FluidGrid::setValue(int x, int y, float newValue) {
    densityGrid[x + gridWidth * y] = newValue;
};