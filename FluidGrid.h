#pragma once
#include "settings.h"
#include <vector>
#include <algorithm>
#include <cstdint>

class FluidGrid {
private:
    std::vector<float> tempDensityGrid; // Temporary grid for the current density values to be stored when performing the advect or diffuse methods on the density grid.
                                        // Uses a float to represent the density in each cell.
    std::vector<float> tempxVelocityGrid; // Temporary grid for the current x velocity values to be stored when perfroming the advect, diffuse or project methods on the x velocity grid. 
                                          // Uses a float to represent the velocity in each cell (negative values represent left and posotive represent right).
    std::vector<float> tempyVelocityGrid; // Temporary grid for the current y velocity values to be stored when perfroming the advect, diffuse or project methods on the y velocity grid.
                                          // Uses a float to represent the velocity in each cell. (negative values represent down and posotive represent up).
    std::vector<float> divergenceGrid; // Grid that stores the divergence in each cell.
                                       // Uses a float to represent the divergence in each cell.
    std::vector<float> pressureGrid; // Grid that stores the pressure in each cell.
                                     // Uses a float to represent the pressure in each cell.
    std::vector<float> densityGrid; // Grid that stores the density in each cell.
                                    // Uses a float to represent the density in each cell.
    std::vector<float> xvelocityGrid; // Grid that stores the x velocity in each cell.
                                      // Uses a float to represent the velocity in each cell (negative values represent left and posotive represent right).
    std::vector<float> yvelocityGrid; // Grid that stores the y velocity in each cell.
                                      // Uses a float to represent the velocity in each cell (negative values represent down and posotive represent up).
    std::vector<bool> obstacleGrid; // Grid that stores the positions of any obstacles.
                                    // Uses a boolean to represent whether there is a obstacle in that cell or not.

    void diffuse(int boundaryType, std::vector<float>& x, std::vector<float>& x0); // Method that "spreads" the fluid or velocity out when called.
    void setBoundaries(int boundaryType, std::vector<float>& x); // Method that reflects velocity and blocks density at the outer "walls" and the obstacles.
                                                                 // This is generally called on a grid after it has been altered in any way.
    void advect(int boundaryType, std::vector<float>& grid, std::vector<float>& tempGrid); // Method that moves density or velocity based on the velocity in the grid.
    void project(std::vector<float>& xvelocityGrid, std::vector<float>& yvelocityGrid, std::vector<float>& divergenceGrid, std::vector<float>& pressureGrid); // Method that makes the simulation mass-conserving by "cleaning up" after the velocity grid's have been altered.
public:
    FluidGrid(); // Constructor for the fluid grid.

    float getValue(int grid, int x, int y); // Getter method that can be used on any grid (0 - density grid, 1 - x velocity grid, 2 - y velocity grid).
    void setValue(int grid, int x, int y, float newValue); // Setter method that can be used on any grid (0 - density grid, 1 - x velocity grid, 2 - y velocity grid).
    bool getObstacleGridValue(int x, int y); // Getter method that can only be used on the obstacle grid.
                                             // This is seperate from the getter method for the other grids as they are made up of floats whereas the obstacle grid is made up of booleans.
    void setObstacleGridValue(int x, int y, bool newValue); // Setter method that can only be used on the obstacle grid.
                                                            // This is seperate from the getter method for the other grids as they are made up of floats whereas the obstacle grid is made up of booleans.
    void step(); // Function that calls diffuse, project and advect in the correct order every time step. Called every frame.
};
