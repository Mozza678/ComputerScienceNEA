#include "FluidGrid.h"

FluidGrid::FluidGrid(int gridSize)
    : densityGrid(gridSize * gridSize),
      tempDensityGrid(gridSize * gridSize)
    {

    };

float FluidGrid::getValue(int x, int y) {
    return densityGrid[x + gridWidth * y];
};

void FluidGrid::setValue(int x, int y, float newValue) {
    densityGrid[x + gridWidth * y] = newValue;
};

void FluidGrid::diffuse(float diffRate, float deltaTime) {
    float a = deltaTime * diffRate * (gridWidth - 2) * (gridWidth - 2);
    for (int k = 0; k < 20; k++) {
        for (int y = 1; y < gridWidth - 1; y++) {
            for (int x = 1; x < gridWidth - 1; x++) {
                float neighbors = getValue(x - 1, y) + getValue(x + 1, y) +
                                  getValue(x, y - 1) + getValue(x, y + 1);
                float nextVal = (tempDensityGrid[x + gridWidth * y] + a * neighbors) / (1 + 4 * a);
                densityGrid[x + gridWidth * y] = nextVal;
            }
        }
    }
};

void FluidGrid::copyDensityGrid() {
    tempDensityGrid = densityGrid;
};