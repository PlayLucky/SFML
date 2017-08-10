#include <iostream>
#include <SFML/Graphics.hpp>

void draw(sf::VertexArray& points, int x, int y, int r, float m) {
  //std::cout << "draw" << std::endl;
  if(m >= 1 || m <= 0) return;
  
  if(r >= 10) {
    draw(points, x-r, y, r*m, m);
    draw(points, x+r, y, r*m, m);
    draw(points, x, y-r, r*m, m);
  } else {
    sf::Vector2f point(x, y);
    points.append(point);
  }

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
    sf::VertexArray points(sf::Points);
    draw(points, width/2, height-100*2, 100, float(sf::Mouse::getPosition(window).x)/width);
    window.draw(points);

    window.display();
  }
  return 0;
}
