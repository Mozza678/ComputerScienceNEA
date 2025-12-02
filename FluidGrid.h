#include <settings.h>
#include <vector>

class FluidGrid {
private:
    int N;

    float densityGrid [gridWidth * gridWidth];
public:
    float getValue(int x, int y);
    void setValue(int x, int y, float newValue);
};