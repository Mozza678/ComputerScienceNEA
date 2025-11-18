#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({2560, 1440}), "My Window", sf::Style::Default, sf::State::Fullscreen);
    window.setFramerateLimit(60); // call it once after creating the window

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
