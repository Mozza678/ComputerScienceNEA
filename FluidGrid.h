#pragma once
#include "settings.h"
#include <vector>

class FluidGrid {
private:
    std::vector<float> tempDensityGrid;
    void diffuse(float diffRate, float deltaTime);
    void setBoundaries(int b, std::vector<float>& x);
public:
    FluidGrid(int gridSize);
    std::vector<float> densityGrid;
    float getValue(std::vector<float>& grid, int x, int y);
    void setValue(std::vector<float>& grid, int x, int y, float newValue);
    void step();
};