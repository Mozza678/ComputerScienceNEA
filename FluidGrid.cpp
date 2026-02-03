#include "FluidGrid.h"
#include "math.h"

FluidGrid::FluidGrid(int gridSize)
    : densityGrid(gridSize * gridSize),
      tempDensityGrid(gridSize * gridSize),
      xvelocityGrid(gridSize * gridSize),
      yvelocityGrid(gridSize * gridSize),
      divergenceGrid(gridSize * gridSize), 
      pressureGrid(gridSize * gridSize)
    {

    };

float FluidGrid::getValue(std::vector<float>& grid, int x, int y) {
    return grid[x + gridWidth * y];
};

void FluidGrid::setValue(std::vector<float>& grid, int x, int y, float newValue) {
    grid[x + gridWidth * y] = newValue;
};

void FluidGrid::diffuse(int boundaryType, std::vector<float>& grid, std::vector<float>& tempGrid) {
    float spreadingFactor = deltaTime * diffRate * (gridWidth - 2) * (gridWidth - 2);

    for (int pass = 0; pass < 20; pass++) {
        for (int y = 1; y < gridWidth - 1; y++) {
            for (int x = 1; x < gridWidth - 1; x++) {
                grid[x + gridWidth * y] = (tempGrid[x + gridWidth * y] + 
                    spreadingFactor * (
                        grid[x - 1 + gridWidth * y] + 
                        grid[x + 1 + gridWidth * y] +
                        grid[x + gridWidth * (y - 1)] + 
                        grid[x + gridWidth * (y + 1)]))
                    / (1 + 4 * spreadingFactor);
            }
        }
        setBoundaries(boundaryType, grid); 
    }
}

void FluidGrid::step() {
    tempxVelocityGrid = xvelocityGrid;
    tempyVelocityGrid = yvelocityGrid;
    diffuse(1, xvelocityGrid, tempxVelocityGrid);
    diffuse(2, yvelocityGrid, tempyVelocityGrid);
    project(xvelocityGrid, yvelocityGrid, divergenceGrid, pressureGrid);

    tempxVelocityGrid = xvelocityGrid;
    tempyVelocityGrid = yvelocityGrid;
    advect(1, xvelocityGrid, tempxVelocityGrid);
    advect(2, yvelocityGrid, tempyVelocityGrid);
    project(xvelocityGrid, yvelocityGrid, divergenceGrid, pressureGrid);

    tempDensityGrid = densityGrid;
    diffuse(0, densityGrid, tempDensityGrid);

    tempDensityGrid = densityGrid; 
    advect(0, densityGrid, tempDensityGrid);
}

void FluidGrid::setup() {
};

void FluidGrid::setBoundaries(int boundaryType, std::vector<float>& grid) {
    for (int i = 1; i < gridWidth - 1; i++) {
        if (boundaryType == 1) {
            grid[0 + gridWidth * i] = -grid[1 + gridWidth * i];
        } else {
            grid[0 + gridWidth * i] = grid[1 + gridWidth * i];
        };
        if (boundaryType == 1) {
            grid[(gridWidth - 1) + gridWidth * i] = -grid[(gridWidth - 2) + gridWidth * i];
        } else {
            grid[(gridWidth - 1) + gridWidth * i] = grid[(gridWidth - 2) + gridWidth * i];
        };
        if (boundaryType == 2) {
            grid[i] = -grid[i + gridWidth];
        } else {
            grid[i] =  grid[i + gridWidth];
        };
        if (boundaryType == 2) {
            grid[i + gridWidth * (gridWidth - 1)] = -grid[i + gridWidth * (gridWidth - 2)];
        } else {
            grid[i + gridWidth * (gridWidth - 1)] = grid[i + gridWidth * (gridWidth - 2)];
        }
    }
    grid[0] = 0.5f * (grid[1] + grid[gridWidth]);
    grid[gridWidth * (gridWidth - 1)] = 0.5f * (grid[1 + gridWidth * (gridWidth - 1)] + grid[gridWidth * (gridWidth - 2)]);
    grid[(gridWidth - 1)] = 0.5f * (grid[(gridWidth - 2) + gridWidth * 0] + grid[(gridWidth - 1) + gridWidth * 1]);
    grid[(gridWidth - 1) + gridWidth * (gridWidth - 1)] = 0.5f * (grid[(gridWidth - 2) + gridWidth * (gridWidth - 1)] + grid[(gridWidth - 1) + gridWidth * (gridWidth - 2)]);
}

void FluidGrid::project(std::vector<float>& xvelocityGrid, std::vector<float>& yvelocityGrid, std::vector<float>& divergenceGrid, std::vector<float>& pressureGrid) {

    float cellSize = 1.0f / gridWidth;

    for (int y = 1; y < gridWidth - 1; y++) {
        for (int x = 1; x < gridWidth - 1; x++) {
            divergenceGrid[x + gridWidth * y] = -0.5f * cellSize * (
                                                xvelocityGrid[x + 1 + gridWidth * y] - xvelocityGrid[x - 1 + gridWidth * y] +
                                                yvelocityGrid[x + gridWidth * (y + 1)] - yvelocityGrid[x + gridWidth * (y - 1)]);
            pressureGrid[x + gridWidth * y] = 0;
        }
    }

    setBoundaries(0, divergenceGrid);
    setBoundaries(0, pressureGrid);

    for (int repetition = 0; repetition < 20; repetition++) {
        for (int y = 1; y < gridWidth - 1; y++) {
            for (int x = 1; x < gridWidth - 1; x++) {
                pressureGrid[x + gridWidth * y] = (divergenceGrid[x + gridWidth * y] +
                                                   pressureGrid[x + 1 + gridWidth * y] + pressureGrid[x - 1 + gridWidth * y] +
                                                   pressureGrid[x + gridWidth * (y + 1)] + pressureGrid[x + gridWidth * (y - 1)]) / 4.0f;
            }
        }
        setBoundaries(0, pressureGrid);
    }

    for (int y = 1; y < gridWidth - 1; y++) {
        for (int x = 1; x < gridWidth - 1; x++) {
            xvelocityGrid[x + gridWidth * y] -= 0.5f * (pressureGrid[x + 1 + gridWidth * y] - pressureGrid[x - 1 + gridWidth * y]) / cellSize;
            yvelocityGrid[x + gridWidth * y] -= 0.5f * (pressureGrid[x + gridWidth * (y + 1)] - pressureGrid[x + gridWidth * (y - 1)]) / cellSize;
        }
    }
    setBoundaries(1, xvelocityGrid);
    setBoundaries(2, yvelocityGrid);
}

void FluidGrid::advect(int boundaryType, std::vector<float>& grid, std::vector<float>& tempGrid) {

    float neighbourLeft, neighbourRight, neighbourAbove, neighbourBelow;
    float neighbourLeftWeight, neighbourRightWeight, neighbourAboveWeight, neighbourBelowWeight;

    float speed = deltaTime * (gridWidth - 2);

    float gridWidthFloat = gridWidth;

    int x, y;
    float xFloat, yFloat;
    
    float xPrev, yPrev;

    float xDisplacement, yDisplacement;

    for(x = 1, xFloat = 1; x < gridWidth - 1; x++, xFloat++) {
        for(y = 1, yFloat = 1; y < gridWidth - 1; y++, yFloat++) {
            xDisplacement = speed * xvelocityGrid[x + gridWidth * y];
            yDisplacement = speed * yvelocityGrid[x + gridWidth * y];
            xPrev = xFloat - xDisplacement;
            yPrev = yFloat - yDisplacement;

            if (xPrev < 0.5f) xPrev = 0.5f;
            if (yPrev < 0.5f) yPrev = 0.5f;
            if (xPrev > gridWidthFloat - 1.5f) xPrev = gridWidthFloat - 1.5f;
            if (yPrev > gridWidthFloat - 1.5f) yPrev = gridWidthFloat - 1.5f;

            neighbourLeft = ::floorf(xPrev);
            neighbourRight = neighbourLeft + 1;
            neighbourBelow = ::floorf(yPrev);
            neighbourAbove = neighbourBelow + 1;

            int intNeighbourLeft = (int)neighbourLeft;
            int intNeighbourRight = (int)neighbourRight;
            int intNeighbourBelow = (int)neighbourBelow;
            int intNeighbourAbove = (int)neighbourAbove;

            neighbourRightWeight = xPrev - neighbourLeft;
            neighbourLeftWeight = 1.0f - neighbourRightWeight;
            neighbourAboveWeight = yPrev - neighbourBelow;
            neighbourBelowWeight = 1.0f - neighbourAboveWeight;

            grid[x + gridWidth * y] = 
                neighbourLeftWeight  * (neighbourBelowWeight * tempGrid[intNeighbourLeft + gridWidth * intNeighbourBelow] + 
                                        neighbourAboveWeight * tempGrid[intNeighbourLeft + gridWidth * intNeighbourAbove]) +
                neighbourRightWeight * (neighbourBelowWeight * tempGrid[intNeighbourRight + gridWidth * intNeighbourBelow] + 
                                        neighbourAboveWeight * tempGrid[intNeighbourRight + gridWidth * intNeighbourAbove]);
        }
    }

    setBoundaries(boundaryType, grid);
}