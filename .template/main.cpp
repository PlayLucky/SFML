#include <iostream>
#include <SFML/Graphics.hpp>

sf::Vector2i dimensions(800, 600);

int main() {
  sf::RenderWindow window(sf::VideoMode(dimensions.x, dimensions.y), "");
  window.setFramerateLimit(30);

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        default: break;
      }
    }

    window.clear(sf::Color::Black);

    window.display();
  }
  return 0;
}
