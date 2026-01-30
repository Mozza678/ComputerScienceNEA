#pragma once
#include "settings.h"
#include <vector>

class FluidGrid {
private:
    std::vector<float> tempDensityGrid;
    std::vector<float> tempxVelocityGrid;
    std::vector<float> tempyVelocityGrid;
    void diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt);
    void setBoundaries(int b, std::vector<float>& x);
    void advect(int b, std::vector<float>& grid, std::vector<float>& tempGrid);
public:
    FluidGrid(int gridSize);
    std::vector<float> densityGrid;
    std::vector<float> xvelocityGrid;
    std::vector<float> yvelocityGrid;
    float getValue(std::vector<float>& grid, int x, int y);
    void setValue(std::vector<float>& grid, int x, int y, float newValue);
    void setup();
    void step();
};