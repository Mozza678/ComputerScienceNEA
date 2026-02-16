#pragma once
#include "settings.h"
#include <vector>

class FluidGrid {
private:
    std::vector<float> tempDensityGrid; // temporary grid for current density values to be stored when performing the advect or diffuse methods on the density grid
    std::vector<float> tempxVelocityGrid; // temporary grid for current x velocity values to be stored when perfroming the advect, diffuse or project methods on the x velocity grid
    std::vector<float> tempyVelocityGrid; // temporary grid for current y velocity values to be stored when perfroming the advect, diffuse or project methods on the y velocity grid
    std::vector<float> divergenceGrid; // grid that stores the divergence in each cell
    std::vector<float> pressureGrid; // grid that stores the pressure in each cell
    std::vector<float> densityGrid; // grid that stores the density in each cell
    std::vector<float> xvelocityGrid; // grid that stores the x velocity in each cell
    std::vector<float> yvelocityGrid; // grid that stores the y velocity in each cell
    std::vector<bool> obstacleGrid; // grid that stores the positions of any obstacles
    std::vector<float> xemmiterGrid; // grid that stores the x velocity values of any horizontal emmiters
    std::vector<float> yemmiterGrid; // grid that stores the x velocity values of any vertical emmiters

    void diffuse(int boundaryType, std::vector<float>& x, std::vector<float>& x0); // method that "spreads" the fluid out
    void setBoundaries(int boundaryType, std::vector<float>& x); // method that locks off the walls to block density from escaping and reflect velocity
    void advect(int boundaryType, std::vector<float>& grid, std::vector<float>& tempGrid); // method that moves density or velocity based on the velocity in the grid
    void project(std::vector<float>& xvelocityGrid, std::vector<float>& yvelocityGrid, std::vector<float>& divergenceGrid, std::vector<float>& pressureGrid); // method that makes the simulation mass-conserving by "cleaning up" after the advect method
public:
    FluidGrid(); // constructor for the fluid grid

    float getValue(int grid, int x, int y); // getter method that can be used on any grid (0 - density grid, 1 - x velocity grid, 2 - y velocity grid)
    void setValue(int grid, int x, int y, float newValue); // setter method that can be used on any grid (0 - density grid, 1 - x velocity grid, 2 - y velocity grid)
    bool getObstacleGridValue(int x, int y);
    void setObstacleGridValue(int x, int y, bool newValue);
    void setup(); // funtion called at the start of a simulation to prepare the fluid grid ***** useless as of now
    void step(); // function that calls diffuse, project and advect in the correct order every time step
};
