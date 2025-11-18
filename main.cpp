#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

struct Cell {
    float density = 1.0f;
    float velX = 0.0f;
    float velY = 0.0f;
};

int main() {
    const int gridHeight = 100;
    const int gridWidth = 100;
    const float cellSize = 5.f;

    std::vector<std::vector<Cell>> grid(gridHeight, std::vector<Cell>(gridWidth));

    sf::RenderWindow window(sf::VideoMode({500, 500}), "Fluid Simulator");
    window.setFramerateLimit(60); // call it once after creating the window

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear(sf::Color(64,64,64));

        grid[50][50].density = 0.0f;


        for(int y = 0; y < gridHeight; y++) {
            for(int x = 0; x < gridWidth; x++) {
                sf::RectangleShape rectangle;
                float cellDensity = grid[y][x].density;
                cellDensity = std::clamp(cellDensity * 255.f, 0.f, 255.f);
                std::uint8_t intCellDensity = static_cast<std::uint8_t>(cellDensity);
                float floatX = static_cast<float>(x);
                float floatY = static_cast<float>(y);

                rectangle.setSize({cellSize, cellSize});
                rectangle.setPosition({floatX * cellSize, floatY * cellSize});

                sf::Color cellDensityColour = {intCellDensity, intCellDensity, intCellDensity};
                rectangle.setFillColor(cellDensityColour);
                window.draw(rectangle);
            }
        }
        window.display();
    }

    return 0;
}
