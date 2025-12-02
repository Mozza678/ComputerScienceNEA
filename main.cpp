#include <Simulation.h>

int main() {
    

    sf::RenderWindow window(sf::VideoMode({500, 500}), "Fluid Simulator");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }








        window.clear(sf::Color(64,64,64));
        window.display();
    }

    return 0;
}
