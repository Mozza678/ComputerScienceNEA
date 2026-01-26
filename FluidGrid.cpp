#include "FluidGrid.h"

FluidGrid::FluidGrid(int gridSize)
    : densityGrid(gridSize * gridSize),
      tempDensityGrid(gridSize * gridSize),
      horizontalVelocity(gridSize * gridSize, 0.0f),
      verticalVelocity(gridSize * gridSize, 0.0f)
    {

    };

void FluidGrid::advect() {
    float scaleFactor = deltaTime * (gridWidth - 2);

    for (int y = 1; y < gridWidth - 1; y++) {
        for (int x = 1; x < gridWidth - 1; x++) {

            //go back to find where the density came from
            float oldX = x - scaleFactor * horizontalVelocity[x + gridWidth * y];
            float oldY = y - scaleFactor * verticalVelocity[x + gridWidth * y];

            //check co ordinates
            if (oldX < 0.5f) {
                oldX = 0.5f; 
            } else if (oldX > gridWidth - 1.51f) {
                oldX = gridWidth - 1.51f;
            };
            if (oldY < 0.5f) {
                oldY = 0.5f; 
            } else if (oldY > gridWidth - 1.51f) {
                oldY = gridWidth - 1.51f;
            };
            
            //assign values for the co-ordinates of the neighbours
            int neighbourLeft = static_cast<int>(oldX);
            int neighbourRight = neighbourLeft + 1;
            int neighbourDown = static_cast<int>(oldY);
            int neighbourUp = neighbourDown + 1;

            //assign values for the weight of the fluid taken from each neighbour depending on position
            float neighbourRightWeight = oldX - static_cast<float>(neighbourLeft);
            float neighbourLeftWeight = 1.0f - neighbourRightWeight;
            float neighbourUpWeight = oldY - static_cast<float>(neighbourDown);
            float neighbourDownWeight = 1.0f - neighbourUpWeight;

            //assign new density to position based on the density within the grids where fluid is being pulled from
            densityGrid[x + gridWidth * y] = 
                neighbourLeftWeight * (neighbourDownWeight * tempDensityGrid[neighbourLeft + gridWidth * neighbourDown] + 
                                       neighbourUpWeight   * tempDensityGrid[neighbourLeft + gridWidth * neighbourUp]) +
                neighbourRightWeight * (neighbourDownWeight * tempDensityGrid[neighbourRight + gridWidth * neighbourDown] + 
                                        neighbourUpWeight   * tempDensityGrid[neighbourRight + gridWidth * neighbourUp]);
        }
    }
}

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

void FluidGrid::addVelocity(int x, int y, float velocityX, float velocityY) {
    int position = x + gridWidth * y;
    horizontalVelocity[position] += velocityX;
    verticalVelocity[position] += velocityY;
}

void FluidGrid::copyDensityGrid() {
    tempDensityGrid = densityGrid;
};