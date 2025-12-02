#include <Simulation.h>
#include <vector>

std::vector<std::uint8_t> Simulation::convertDensityToPixelColourValues(float density){
    std::vector<std::uint8_t> pixelColourValues;
    for (int i = 1; i < 4; i++) {
        pixelColourValues.push_back(static_cast<int>(density) * 255);
    }
    return pixelColourValues;
};

void Simulation::assignPixelColourValuesToPixelBuffer(std::vector<std::uint8_t> pixelColourValues, int x, int y){
    for (int i = 0; i < pixelColourValues.size(); i++) {
        pixelBuffer[(4 * (x + gridWidth * y)) + i] = pixelColourValues[i];
    }
};