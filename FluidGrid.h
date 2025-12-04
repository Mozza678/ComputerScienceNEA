#include "settings.h"
#include <vector>

class FluidGrid {
private:
    std::vector<float> densityGrid;
public:
    FluidGrid(int gridSize);
    float getValue(int x, int y);
    void setValue(int x, int y, float newValue);
};