#pragma once
#include "settings.h"
#include <vector>

class FluidGrid {
private:
    std::vector<float> densityGrid;
    std::vector<float> tempDensityGrid;
    std::vector<float> horizontalVelocity;
    std::vector<float> verticalVelocity;
    std::vector<float> tempHorizontalVelocity;
    std::vector<float> tempVerticalVelocity;
public:
    FluidGrid(int gridSize);
    float getValue(int x, int y);
    void setValue(int x, int y, float newValue);
    void diffuse(float diffRate, float deltaTime);
    void copyDensityGrid();
};