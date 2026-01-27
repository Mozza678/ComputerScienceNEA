#include "FluidGrid.h"

FluidGrid::FluidGrid(int gridSize)
    : densityGrid(gridSize * gridSize),
      tempDensityGrid(gridSize * gridSize),
      horizontalVelocity(gridSize * gridSize, 0.0f),
      verticalVelocity(gridSize * gridSize, 0.0f),
      tempHorizontalVelocity(gridSize * gridSize, 0.0f),
      tempVerticalVelocity(gridSize * gridSize, 0.0f),
      divergence(gridSize * gridSize, 0.0f),
      pressure(gridSize * gridSize, 0.0f)
    {

    };

void FluidGrid::advect(std::vector<float>& grid, std::vector<float>& tempGrid) {
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

            //assign new value to position based on the values within the positions where fluid is being pulled from
            grid[x + gridWidth * y] = 
                neighbourLeftWeight * (neighbourDownWeight * tempGrid[neighbourLeft + gridWidth * neighbourDown] + 
                                       neighbourUpWeight   * tempGrid[neighbourLeft + gridWidth * neighbourUp]) +
                neighbourRightWeight * (neighbourDownWeight * tempGrid[neighbourRight + gridWidth * neighbourDown] + 
                                        neighbourUpWeight   * tempGrid[neighbourRight + gridWidth * neighbourUp]);
        }
    }
}

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
};

void FluidGrid::addVelocity(int x, int y, float velocityX, float velocityY) {
    int position = x + gridWidth * y;
    horizontalVelocity[position] += velocityX;
    verticalVelocity[position] += velocityY;
}

void FluidGrid::project() {
    for (int y = 1; y < gridWidth - 1; y++) {
        for (int x = 1; x < gridWidth - 1; x++) {
            divergence[x + gridWidth * y] = -0.5f * (
                horizontalVelocity[(x + 1) + gridWidth * y] - horizontalVelocity[(x - 1) + gridWidth * y] +
                verticalVelocity[x + gridWidth * (y + 1)] - verticalVelocity[x + gridWidth * (y - 1)]
            ) / gridWidth;
            pressure[x + gridWidth * y] = 0;
        }
    }
    for (int k = 0; k < 20; k++) {
        for (int y = 1; y < gridWidth - 1; y++) {
            for (int x = 1; x < gridWidth - 1; x++) {
                pressure[x + gridWidth * y] = (divergence[x + gridWidth * y] + 
                    pressure[(x - 1) + gridWidth * y] + pressure[(x + 1) + gridWidth * y] +
                    pressure[x + gridWidth * (y - 1)] + pressure[x + gridWidth * (y + 1)]) / 4.0f;
            }
        }
    }
    for (int y = 1; y < gridWidth - 1; y++) {
        for (int x = 1; x < gridWidth - 1; x++) {
            horizontalVelocity[x + gridWidth * y] -= 0.5f * (pressure[(x + 1) + gridWidth * y] - pressure[(x - 1) + gridWidth * y]) * gridWidth;
            verticalVelocity[x + gridWidth * y] -= 0.5f * (pressure[x + gridWidth * (y + 1)] - pressure[x + gridWidth * (y - 1)]) * gridWidth;
        }
    }
}

void FluidGrid::step() {
    tempHorizontalVelocity = horizontalVelocity;
    tempVerticalVelocity = verticalVelocity;
    tempDensityGrid = densityGrid;
    advect(horizontalVelocity, tempHorizontalVelocity);
    advect(verticalVelocity, tempVerticalVelocity);
    project();
    advect(densityGrid, tempDensityGrid);
    diffuse(diffRate, deltaTime);
};
