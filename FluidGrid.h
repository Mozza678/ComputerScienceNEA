#pragma once
#include "settings.h"
#include <vector>

class FluidGrid {
private:
    std::vector<float> tempDensityGrid;
    std::vector<float> tempxVelocityGrid;
    std::vector<float> tempyVelocityGrid;
    std::vector<float> divergenceGrid;
    std::vector<float> pressureGrid;
    void diffuse(int boundaryType, std::vector<float>& x, std::vector<float>& x0);
    void setBoundaries(int boundaryType, std::vector<float>& x);
    void advect(int boundaryType, std::vector<float>& grid, std::vector<float>& tempGrid);
    void project(std::vector<float>& xvelocityGrid, std::vector<float>& yvelocityGrid, std::vector<float>& divergenceGrid, std::vector<float>& pressureGrid);
public:
    FluidGrid();
    std::vector<float> densityGrid;
    std::vector<float> xvelocityGrid;
    std::vector<float> yvelocityGrid;
    float getValue(std::vector<float>& grid, int x, int y);
    void setValue(std::vector<float>& grid, int x, int y, float newValue);
    void setup();
    void step();
};
