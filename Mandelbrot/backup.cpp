#include <iostream>
#include <SFML/Graphics.hpp>
#include <gmpxx.h>

sf::Vector2i dimensions(500, 500);
float pixelPerUnit(200);
float centerComplexX(0);
float centerComplexY(0);
sf::VertexArray pixels;

int maxIterations = 30;
bool valid = false;

float getComplexFromPixelX(int pixelX) {
  float complexX;
  complexX = float(pixelX) / pixelPerUnit;
  complexX += centerComplexX - dimensions.x/2.f/pixelPerUnit;
  return complexX;
}

float getComplexFromPixelY(int pixelY) {
  float complexY;
  complexY = float(pixelY) / pixelPerUnit;
  complexY += centerComplexY - dimensions.y/2.f/pixelPerUnit;
  return complexY;
}

int getIterations(float cX, float cY) {
  float zX(0);
  float zY(0);
  for(int iteration = 1; iteration <= maxIterations; iteration++) {
    float zTempX(zX);
    float zTempY(zY);
    zTempX = zX * zX - zY * zY;
    zTempY = 2 * zY * zY;

    zTempX += cX;
    zTempY += cY;

    zX = zTempX;
    zY = zTempY;

    if(zX * zX + zY * zY >= 4)
      return iteration;
  }
  return maxIterations;
}

int getIterations(sf::Vector2i pixel) {
  return getIterations(getComplexFromPixelX(pixel.x), getComplexFromPixelY(pixel.y));
}

sf::Color getPixelColor(sf::Vector2i pixel) {
  int iterations = getIterations(pixel);
  if(iterations == maxIterations) return sf::Color::Black;
  iterations *= 255/maxIterations;
  return sf::Color(iterations, iterations, iterations);
}

void getPixels() {
  if(valid) return;
  pixels = sf::VertexArray();
  for(int x = 0; x < dimensions.x; x++) {
    for(int y = 0; y < dimensions.y; y++) {
      sf::Color color = getPixelColor(sf::Vector2i(x, y));
      sf::Vertex pixel(sf::Vector2f(x, y), color);
      pixels.append(pixel);
    }
  }
  valid = true;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(dimensions.x, dimensions.y), "Mandelbrot");
  window.setFramerateLimit(1);

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::Resized:
          dimensions.x = event.size.width;
          dimensions.y = event.size.height;
          window.setView(sf::View(sf::FloatRect(0, 0, dimensions.x, dimensions.y)));
          valid = false;
          break;
        case sf::Event::MouseButtonPressed:
          switch(event.mouseButton.button) {
            case sf::Mouse::Button::Left:
              centerComplexX = getComplexFromPixelX(event.mouseButton.x);
              centerComplexY = getComplexFromPixelY(event.mouseButton.y);
              valid = false;
              break;
            case sf::Mouse::Button::Right:
              maxIterations += 5;
              valid = false;
              break;
            default: break;
          }
          break;
        case sf::Event::MouseWheelScrolled:
        {
          int delta = event.mouseWheelScroll.delta;
          if(delta == 0) break;
          else if(delta < 0) {
            pixelPerUnit /= -delta * 1.1;
          } else {
            pixelPerUnit *= delta * 1.1;
          }

          valid = false;
          break;
        }
        default: break;
      }
    }

    getPixels();
    window.clear(sf::Color::Black);
    window.draw(pixels);
    window.display();
    std::cout << "now" << std::endl;
  }

  return 0;
}
