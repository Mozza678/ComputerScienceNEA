#include <vector>
#include <SFML/Graphics.hpp>
#include <settings.h>

class Simulation {
public:
    
private:
    std::uint8_t pixelBuffer[4 * gridWidth * gridWidth];
    std::vector<std::uint8_t> convertDensityToPixelColourValues(float density);
    void assignPixelColourValuesToPixelBuffer(std::vector<std::uint8_t> pixelColourValues, int x, int y);
};
