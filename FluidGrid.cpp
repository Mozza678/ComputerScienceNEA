#include "FluidGrid.h"

FluidGrid::FluidGrid(int gridSize)
    : densityGrid(gridSize * gridSize),
      tempDensityGrid(gridSize * gridSize)
    {

    };

float FluidGrid::getValue(std::vector<float>& grid, int x, int y) {
    return grid[x + gridWidth * y];
};

void FluidGrid::setValue(std::vector<float>& grid, int x, int y, float newValue) {
    grid[x + gridWidth * y] = newValue;
};

void FluidGrid::diffuse(float diffRate, float deltaTime) {
    float a = deltaTime * diffRate * (gridWidth - 2) * (gridWidth - 2);
    for (int k = 0; k < 20; k++) {
        for (int y = 1; y < gridWidth - 1; y++) {
            for (int x = 1; x < gridWidth - 1; x++) {
                float neighbors = getValue(densityGrid, x - 1, y) + getValue(densityGrid, x + 1, y) +
                                  getValue(densityGrid, x, y - 1) + getValue(densityGrid, x, y + 1);
                float nextVal = (tempDensityGrid[x + gridWidth * y] + a * neighbors) / (1 + 4 * a);
                densityGrid[x + gridWidth * y] = nextVal;
            }
        }
    }
    setBoundaries(0, densityGrid);
};

void FluidGrid::step() {
    tempDensityGrid = densityGrid;
    diffuse(diffRate, deltaTime);
}

void FluidGrid::setBoundaries(int b, std::vector<float>& x) {
    for (int i = 1; i < gridWidth - 1; i++) {
        x[0 + gridWidth * i] = (b == 1) ? -x[1 + gridWidth * i] : x[1 + gridWidth * i];
        x[(gridWidth - 1) + gridWidth * i] = (b == 1) ? -x[(gridWidth - 2) + gridWidth * i] : x[(gridWidth - 2) + gridWidth * i];
        x[i + gridWidth * 0] = (b == 2) ? -x[i + gridWidth * 1] : x[i + gridWidth * 1];
        x[i + gridWidth * (gridWidth - 1)] = (b == 2) ? -x[i + gridWidth * (gridWidth - 2)] : x[i + gridWidth * (gridWidth - 2)];
    }
    // Corners
    x[0 + gridWidth * 0] = 0.5f * (x[1 + gridWidth * 0] + x[0 + gridWidth * 1]);
    x[0 + gridWidth * (gridWidth - 1)] = 0.5f * (x[1 + gridWidth * (gridWidth - 1)] + x[0 + gridWidth * (gridWidth - 2)]);
    x[(gridWidth - 1) + gridWidth * 0] = 0.5f * (x[(gridWidth - 2) + gridWidth * 0] + x[(gridWidth - 1) + gridWidth * 1]);
    x[(gridWidth - 1) + gridWidth * (gridWidth - 1)] = 0.5f * (x[(gridWidth - 2) + gridWidth * (gridWidth - 1)] + x[(gridWidth - 1) + gridWidth * (gridWidth - 2)]);
}

