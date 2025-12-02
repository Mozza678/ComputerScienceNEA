#include <fluidGrid.h>

float FluidGrid::getValue(int x, int y) {
    return densityGrid[x + gridWidth * y];
};

void FluidGrid::setValue(int x, int y, float newValue) {
    densityGrid[x + gridWidth * y] = newValue;
}