#include <vector>
#include <SFML/Graphics.hpp>
#include <settings.h>

class Simulation {
public:
    Simulation();
    void run();
    
private:
    std::vector<std::uint8_t> pixelBuffer;
    std::vector<std::uint8_t> convertDensityToPixelColourValues(float density);
    sf::Texture gridTexture;

    void assignPixelColourValuesToPixelBuffer(std::vector<std::uint8_t> pixelColourValues, int x, int y);
    void createGridSprite();
};
