#include "FluidGrid.h"
#include "math.h"

FluidGrid::FluidGrid(int gridSize)
    : densityGrid(gridSize * gridSize),
      tempDensityGrid(gridSize * gridSize),
      xvelocityGrid(gridSize * gridSize),
      yvelocityGrid(gridSize * gridSize)
    {

    };

float FluidGrid::getValue(std::vector<float>& grid, int x, int y) {
    return grid[x + gridWidth * y];
};

void FluidGrid::setValue(std::vector<float>& grid, int x, int y, float newValue) {
    grid[x + gridWidth * y] = newValue;
};

void FluidGrid::diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt) {
    float a = dt * diff * (gridWidth - 2) * (gridWidth - 2);
    for (int k = 0; k < 20; k++) {
        for (int y = 1; y < gridWidth - 1; y++) {
            for (int x_idx = 1; x_idx < gridWidth - 1; x_idx++) {
                // Use the passed in 'x' and 'x0' instead of hardcoded densityGrid
                x[x_idx + gridWidth * y] = (x0[x_idx + gridWidth * y] + a * (
                    x[x_idx - 1 + gridWidth * y] + x[x_idx + 1 + gridWidth * y] +
                    x[x_idx + gridWidth * (y - 1)] + x[x_idx + gridWidth * (y + 1)]
                )) / (1 + 4 * a);
            }
        }
        setBoundaries(b, x); 
    }
}

void FluidGrid::step() {
    // --- VELOCITY STEP ---
    // 1. Snapshot for Velocity Diffusion
    tempxVelocityGrid = xvelocityGrid;
    tempyVelocityGrid = yvelocityGrid;
    diffuse(1, xvelocityGrid, tempxVelocityGrid, diffRate, deltaTime);
    diffuse(2, yvelocityGrid, tempyVelocityGrid, diffRate, deltaTime);

    // 2. Snapshot for Velocity Advection (Velocity moves itself)
    tempxVelocityGrid = xvelocityGrid;
    tempyVelocityGrid = yvelocityGrid;
    advect(1, xvelocityGrid, tempxVelocityGrid);
    advect(2, yvelocityGrid, tempyVelocityGrid);

    // --- DENSITY STEP ---
    // 3. Snapshot for Density Diffusion
    tempDensityGrid = densityGrid;
    diffuse(0, densityGrid, tempDensityGrid, diffRate, deltaTime);

    // 4. Snapshot for Density Advection (Smoke moves along the wind)
    tempDensityGrid = densityGrid; 
    advect(0, densityGrid, tempDensityGrid);
}

void FluidGrid::setup() {
};

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

void FluidGrid::advect(int boundary, std::vector<float>& grid, std::vector<float>& tempGrid) {

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

    setBoundaries(boundary, grid);
}