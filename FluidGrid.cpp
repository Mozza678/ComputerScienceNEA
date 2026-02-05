#include "FluidGrid.h"
#include "math.h"
#include "settings.h"

FluidGrid::FluidGrid()   // Fluid grid constructor, uses the gridWidth value stored in settings header file to construct all grids
    : densityGrid(gridWidth * gridWidth),
      tempDensityGrid(gridWidth * gridWidth),
      xvelocityGrid(gridWidth * gridWidth),
      yvelocityGrid(gridWidth * gridWidth),
      divergenceGrid(gridWidth * gridWidth), 
      pressureGrid(gridWidth * gridWidth)
    {

    };

float FluidGrid::getValue(std::vector<float>& grid, int x, int y) { // getter method that can be used on any grid
    return grid[x + gridWidth * y];
};

void FluidGrid::setValue(std::vector<float>& grid, int x, int y, float newValue) { // setter method that can be used on any grid
    grid[x + gridWidth * y] = newValue;
};

void FluidGrid::diffuse(int boundaryType, std::vector<float>& grid, std::vector<float>& tempGrid) { // 
    float spreadingFactor = deltaTime * diffRate * (gridWidth - 2) * (gridWidth - 2); // the amount the fluid spreads due to the diffusion
                                                                                      // this is proportional to the useful grid size, the time step and an arbitrary adjustable constant set in settings

    for (int pass = 0; pass < 20; pass++) { // the method for solving the equation for the diffusion is only an estimate and therefore is looped to increase accuracy
        for (int y = 1; y < gridWidth - 1; y++) { // loop through all rows
            for (int x = 1; x < gridWidth - 1; x++) { // loop through all columns
                grid[x + gridWidth * y] = (tempGrid[x + gridWidth * y] + 
                    spreadingFactor * (
                        grid[x - 1 + gridWidth * y] + 
                        grid[x + 1 + gridWidth * y] +
                        grid[x + gridWidth * (y - 1)] + 
                        grid[x + gridWidth * (y + 1)]))
                    / (1 + 4 * spreadingFactor); // equation for solving density taken from jon stams paper "Real Time Fluid Dynamics for Games"
            }
        }
        setBoundaries(boundaryType, grid); // set boundries fucntion called using the parameter passed in diffuse
                                           // this parameter is required as both the density and velocity grids are diffused
    }
}

void FluidGrid::step() {
    tempxVelocityGrid = xvelocityGrid; // the temporary velocity grids are set to be referenced within the diffuse function
    tempyVelocityGrid = yvelocityGrid;
    diffuse(1, xvelocityGrid, tempxVelocityGrid); // both velocity grids are diffused to "spread" the velocity
    diffuse(2, yvelocityGrid, tempyVelocityGrid);
    project(xvelocityGrid, yvelocityGrid, divergenceGrid, pressureGrid); // the velocity grids are projected to conserve mass, this is done after acting on the velocity grids in any way

    tempxVelocityGrid = xvelocityGrid; // the temporary velocity grids are set to be referenced within the advect function
    tempyVelocityGrid = yvelocityGrid;
    advect(1, xvelocityGrid, tempxVelocityGrid); // both velocity grid are advected causing the velocity to "pull" the velocity
    advect(2, yvelocityGrid, tempyVelocityGrid);
    project(xvelocityGrid, yvelocityGrid, divergenceGrid, pressureGrid); // both velocity grids are projected again to conserve mass

    tempDensityGrid = densityGrid; // the temporary velocity grids are set to be referenced within the diffuse function
    diffuse(0, densityGrid, tempDensityGrid); // the density grid is diffused, essentially spreading the fluid

    tempDensityGrid = densityGrid; // the temporary velocity grids are set to be referenced within the diffuse function
    advect(0, densityGrid, tempDensityGrid); // the density grid is advected causing the velocity to "pull" the density
}

void FluidGrid::setup() {
};

void FluidGrid::setBoundaries(int boundaryType, std::vector<float>& grid) { // this essentially creates a "wall" around the grid so fluid can't escape and is instead reflected
                                                                            // the boundaryType parameter has three settings : 0 - Mirroring density at walls
                                                                            //                                                 1 - Reflecting x velocity at left and right walls
                                                                            //                                                 2 - Reflecting y velocity at top and bottom walls
    
    for (int i = 1; i < gridWidth - 1; i++) { // loop to scan through all cells on the border. x and y not used as only those on the border need to be checked
        if (boundaryType == 1) { // This accesses the left wall
                                 // If in boundaryType 1 it will reflect the x velocity of the adjacent cell so that at the boundary between the two cells there is net zero x velocity meaning no fluid can escape
                                 // If in boundaryType 0 or 2 it will simply copy the contents of the adjacent cell. When performed on the density this means no diffusion will take place towards the wall
            grid[0 + gridWidth * i] = -grid[1 + gridWidth * i];
        } else {
            grid[0 + gridWidth * i] = grid[1 + gridWidth * i];
        };
        if (boundaryType == 1) { // This accesses the right wall            
            grid[(gridWidth - 1) + gridWidth * i] = -grid[(gridWidth - 2) + gridWidth * i];
        } else {
            grid[(gridWidth - 1) + gridWidth * i] = grid[(gridWidth - 2) + gridWidth * i];
        };
        if (boundaryType == 2) { // this accesses the top wall
                                 // If in boundaryType 2 it will reflect the y velocity of the adjacent cell so that at the boundary between the two cells there is net zero y velocity meaning no fluid can escape
                                 // If in boundaryType 0 or 2 it will simply copy the contents of the adjacent cell. When performed on the density this means no diffusion will take place towards the wall
            grid[i] = -grid[i + gridWidth];
        } else {
            grid[i] =  grid[i + gridWidth];
        };
        if (boundaryType == 2) { // this accesses the bottom wall
            grid[i + gridWidth * (gridWidth - 1)] = -grid[i + gridWidth * (gridWidth - 2)];
        } else {
            grid[i + gridWidth * (gridWidth - 1)] = grid[i + gridWidth * (gridWidth - 2)];
        }
    }

    // The following code sets the boundary values for the corners. The averages the two adjacent cells and sets the corners value to the result.
    
    grid[0] = 0.5f * (grid[1] + grid[gridWidth]); // top left corner
    grid[gridWidth * (gridWidth - 1)] = 0.5f * (grid[1 + gridWidth * (gridWidth - 1)] + grid[gridWidth * (gridWidth - 2)]); // bottom left corner
    grid[(gridWidth - 1)] = 0.5f * (grid[(gridWidth - 2) + gridWidth * 0] + grid[(gridWidth - 1) + gridWidth * 1]); // top right corner
    grid[(gridWidth - 1) + gridWidth * (gridWidth - 1)] = 0.5f * (grid[(gridWidth - 2) + gridWidth * (gridWidth - 1)] + grid[(gridWidth - 1) + gridWidth * (gridWidth - 2)]); // bottom right corner
}

void FluidGrid::project(std::vector<float>& xvelocityGrid, std::vector<float>& yvelocityGrid, std::vector<float>& divergenceGrid, std::vector<float>& pressureGrid) {

    float cellSize = 1.0f / gridWidth; // calculates the size of one cell relative to the grid

    for (int y = 1; y < gridWidth - 1; y++) { // loops through all rows
        for (int x = 1; x < gridWidth - 1; x++) { // loops through all columns
            divergenceGrid[x + gridWidth * y] = -0.5f * cellSize * (
                                                xvelocityGrid[x + 1 + gridWidth * y] - xvelocityGrid[x - 1 + gridWidth * y] +
                                                yvelocityGrid[x + gridWidth * (y + 1)] - yvelocityGrid[x + gridWidth * (y - 1)]); // calculates the divergence of each cell and assignes each value to the grid
                                                                                                                                  // formula taken from jon stams paper "Real Time Fluid Dynamics for Games"
            pressureGrid[x + gridWidth * y] = 0; // resets the pressure grid before calculation
        }
    }

    setBoundaries(0, divergenceGrid); // the divergence grid boundaries are set using the mirror mode as divergence can be treated the same as density when setting the boundaries
    setBoundaries(0, pressureGrid); // the pressure grid boundaries are set using the mirror mode as pressure can be treated the same as density when setting the boundary

    for (int repetition = 0; repetition < 20; repetition++) { // similiar method used below as the one in diffuse. the equation is repeated 20 times to increase accuracy
        for (int y = 1; y < gridWidth - 1; y++) { // loop through all rows
            for (int x = 1; x < gridWidth - 1; x++) { // loop through all columns
                pressureGrid[x + gridWidth * y] = (divergenceGrid[x + gridWidth * y] +
                                                   pressureGrid[x + 1 + gridWidth * y] + pressureGrid[x - 1 + gridWidth * y] +
                                                   pressureGrid[x + gridWidth * (y + 1)] + pressureGrid[x + gridWidth * (y - 1)]) / 4.0f; // pressure values for each cell are calculated using formula taken from jon stams paper "Real Time Fluid Dynamics for Games"
            }
        }
        setBoundaries(0, pressureGrid); // the pressure grid boundaries are set using the mirror mode as pressure can be treated the same as density when setting the boundary
    }

    for (int y = 1; y < gridWidth - 1; y++) { // loop through all rows
        for (int x = 1; x < gridWidth - 1; x++) { // loop through all columns
            xvelocityGrid[x + gridWidth * y] -= 0.5f * (pressureGrid[x + 1 + gridWidth * y] - pressureGrid[x - 1 + gridWidth * y]) / cellSize; // corrected values assigned to the x velocity grid meaning it is now mass conserving
            yvelocityGrid[x + gridWidth * y] -= 0.5f * (pressureGrid[x + gridWidth * (y + 1)] - pressureGrid[x + gridWidth * (y - 1)]) / cellSize; // corrected values assigned to the y velocity grid meaning it is now mass conserving
        }
    }
    setBoundaries(1, xvelocityGrid); // boundaries set on x velocity grid using mode 1 as it has just been altered by project
    setBoundaries(2, yvelocityGrid); // boundaries set on y velocity grid using mode 1 as it has just been altered by project
}

void FluidGrid::advect(int boundaryType, std::vector<float>& grid, std::vector<float>& tempGrid) {

    float neighbourLeft, neighbourRight, neighbourAbove, neighbourBelow;
    float neighbourLeftWeight, neighbourRightWeight, neighbourAboveWeight, neighbourBelowWeight;

    float speed = deltaTime * (gridWidth - 2); // speed value is calculated that is proportional to the size of the useful grid and the time step
                                               // essentially a scalar for the displacement that should be pulled from

    float gridWidthFloat = gridWidth; // grid width casted to float so more accurate positions within cells can be considered

    int x, y; // co-ordinates of cell that is pulling density/velocity, in simpler words this is the cell that is being modified
    float xFloat, yFloat; // co-ordinates of cell that is pulling density/velocity casted to float to allow operations involving previous co-ordinates
    
    float xPrev, yPrev; // co-ordinates that density/velocity is being pulled from

    float xDisplacement, yDisplacement; // distance between current cell and previous cell

    for(x = 1, xFloat = 1; x < gridWidth - 1; x++, xFloat++) { // loop through all columns
        for(y = 1, yFloat = 1; y < gridWidth - 1; y++, yFloat++) { // loop through all rows
            xDisplacement = speed * xvelocityGrid[x + gridWidth * y]; // calculate the distance required to go back based on the magnitude of the velocity
            yDisplacement = speed * yvelocityGrid[x + gridWidth * y];
            xPrev = xFloat - xDisplacement; // assigned values to the co-ordinates of the cell being pulled from based on the value of the displacement
            yPrev = yFloat - yDisplacement;

            // clamp values to the edge of the grid if they go out of bounds

            if (xPrev < 0.5f) xPrev = 0.5f; // clamp to left wall
            if (yPrev < 0.5f) yPrev = 0.5f; // clamp to bottom wall
            if (xPrev > gridWidthFloat - 1.5f) xPrev = gridWidthFloat - 1.5f; // clamp to the right wall
            if (yPrev > gridWidthFloat - 1.5f) yPrev = gridWidthFloat - 1.5f; // clamp to the top wall

            neighbourLeft = ::floorf(xPrev); // truncates the float co-ordinate to give the co-ordinate of the nearest left cell
            neighbourRight = neighbourLeft + 1; // adds one to the left neighbour to give the co-ordinate of the nearest right cell
            neighbourBelow = ::floorf(yPrev); // truncates the float co-ordinate to give the co-ordinate of the nearest cell below
            neighbourAbove = neighbourBelow + 1; // adds one to the neighbour below to give the co-ordinate of the nearest cell above

            // casts all neighbour co-ordinates to integers

            int intNeighbourLeft = (int)neighbourLeft; 
            int intNeighbourRight = (int)neighbourRight;
            int intNeighbourBelow = (int)neighbourBelow;
            int intNeighbourAbove = (int)neighbourAbove;

            // creates weights based on how close the co-ordinates being pulled from are to each neighbour
            
            neighbourRightWeight = xPrev - neighbourLeft;
            neighbourLeftWeight = 1.0f - neighbourRightWeight;
            neighbourAboveWeight = yPrev - neighbourBelow;
            neighbourBelowWeight = 1.0f - neighbourAboveWeight;

            // amount of density/velocity that needs to be pulled is calculated based on the amount in each neighbour and the weights of each neighbour

            grid[x + gridWidth * y] = 
                neighbourLeftWeight  * (neighbourBelowWeight * tempGrid[intNeighbourLeft + gridWidth * intNeighbourBelow] + 
                                        neighbourAboveWeight * tempGrid[intNeighbourLeft + gridWidth * intNeighbourAbove]) +
                neighbourRightWeight * (neighbourBelowWeight * tempGrid[intNeighbourRight + gridWidth * intNeighbourBelow] + 
                                        neighbourAboveWeight * tempGrid[intNeighbourRight + gridWidth * intNeighbourAbove]);
        }
    }

    setBoundaries(boundaryType, grid); // boundaries set for grid using the boundaryType parameter set when calling the advect function
}
