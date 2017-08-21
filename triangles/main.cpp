#include <iostream>
#include <SFML/Graphics.hpp>

void draw(sf::RenderWindow& window, int x, int y, int r, float m) {
  //std::cout << "draw" << std::endl;
  if(m >= 1 || m <= 0) return;
  
  if(r >= 3) {
    draw(window, x-r, y, r*m, m);
    draw(window, x+r, y, r*m, m);
    draw(window, x, y-r, r*m, m);
  }
  sf::CircleShape c(r);
  c.setOrigin(r,r);
  c.setPosition(x, y);
  c.setFillColor(sf::Color::Transparent);
  c.setOutlineColor(sf::Color::Red);
  c.setOutlineThickness(2);
  window.draw(c);
}

int main() {
  int width = 800;
  int height = 600;
  sf::RenderWindow window(sf::VideoMode(width, height), "Triangles");
  window.setFramerateLimit(10);

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
    draw(window, width/2, height-100*2, 100, float(sf::Mouse::getPosition(window).x)/width/2);

    window.display();
  }
  return 0;
}
