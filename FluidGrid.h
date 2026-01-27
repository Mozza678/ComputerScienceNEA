#pragma once
#include "settings.h"
#include <vector>

class FluidGrid {
private:
    std::vector<float> tempDensityGrid;
    std::vector<float> horizontalVelocity;
    std::vector<float> verticalVelocity;
    std::vector<float> tempHorizontalVelocity;
    std::vector<float> tempVerticalVelocity;
    std::vector<float> divergence;
    std::vector<float> pressure;
    void diffuse(float diffRate, float deltaTime);
    void project();
    void advect(std::vector<float>& grid, std::vector<float>& tempGrid);
    void setBoundaries(int b, std::vector<float>& x);
public:
    FluidGrid(int gridSize);
    std::vector<float> densityGrid;
    float getValue(std::vector<float>& grid, int x, int y);
    void setValue(std::vector<float>& grid, int x, int y, float newValue);
    void addVelocity(int x, int y, float velocityX, float velocityY);
    void step();
};