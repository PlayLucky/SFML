#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <gmpxx.h>

sf::Vector2i dimensions(500, 500);
float pixelPerUnit = 200;
mpf_t centerComplexX;
mpf_t centerComplexY;

sf::VertexArray pixels;

int maxIterations = 30;
bool valid = false;

void add(mpf_t& op1, double op2) {
  mpf_t op2Converted;
  mpf_init_set_d(op2Converted, op2);
  mpf_add(op1, op1, op2Converted);
}
void sub(mpf_t& op1, double op2) {
  mpf_t op2Converted;
  mpf_init_set_d(op2Converted, op2);
  mpf_sub(op1, op1, op2Converted);
}

void getComplexFromPixelX(int pixelX, mpf_t& complexX) {
  mpf_t complexX_temp;
  mpf_init_set_d(complexX_temp, double(pixelX) / pixelPerUnit);
  mpf_add(complexX, complexX_temp, centerComplexX);
  sub(complexX, dimensions.x/2.0/pixelPerUnit);
}

void getComplexFromPixelY(int pixelY, mpf_t& complexY) {
  mpf_t complexY_temp;
  mpf_init_set_d(complexY_temp, double(pixelY) / pixelPerUnit);
  mpf_add(complexY, complexY_temp, centerComplexY);
  sub(complexY, dimensions.y/2.0/pixelPerUnit);
}

int getIterations(mpf_t cX, mpf_t cY) {
  mpf_t zX;
  mpf_t zY;
  mpf_t zX_temp;
  mpf_t zY_temp;
  mpf_init(zX);
  mpf_init(zY);
  mpf_init(zX_temp);
  mpf_init(zY_temp);
  for(int iteration = 1; iteration <= maxIterations; iteration++) {
    mpf_mul(zX_temp, zX, zX);
    mpf_mul(zY_temp, zY, zY);
    mpf_sub(zX_temp, zX_temp, zY_temp);

    mpf_mul(zY_temp, zX, zY);
    mpf_add(zY_temp, zY_temp, zY_temp);

    mpf_add(zX, zX_temp, cX);
    mpf_add(zY, zY_temp, cY);

    mpf_mul(zX_temp, zX, zX);
    mpf_mul(zY_temp, zY, zY);
    mpf_add(zX_temp, zX_temp, zY_temp);
    if(mpf_cmp_d(zX_temp, 4) > 0)
      return iteration;
  }
  return maxIterations;
}

int getIterations(sf::Vector2i pixel) {
  mpf_t x;
  mpf_t y;
  mpf_init(x);
  mpf_init(y);
  getComplexFromPixelX(pixel.x, x);
  getComplexFromPixelY(pixel.y, y);
  return getIterations(x, y);
}

sf::Color getPixelColor(sf::Vector2i pixel) {
  int iterations = getIterations(pixel);
  if(iterations == maxIterations) return sf::Color::Black;
  int color = (float(iterations) / maxIterations) * 255;
  return sf::Color(color, color, color);
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
  mpf_init_set_d(centerComplexX, 0.001643721971153);
  mpf_init_set_d(centerComplexY, 0.822467633298876);


  sf::RenderWindow window(sf::VideoMode(dimensions.x, dimensions.y), "Mandelbrot");
  window.setFramerateLimit(0);

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
              getComplexFromPixelX(event.mouseButton.x, centerComplexX);
              getComplexFromPixelY(event.mouseButton.y, centerComplexY);
              valid = false;
              break;
            case sf::Mouse::Button::Right:
              maxIterations += 5;
              valid = false;
              break;
            default: break;
          }
          break;
        case sf::Event::KeyPressed:
          switch(event.key.code){
            case sf::Keyboard::Space:
            {
              std::string input = "";
              std::cout << "Move to:\na: ";
              std::cin >> input;
              mpf_set_str(centerComplexX, input.c_str(), 10);
              std::cout << "b: ";
              std::cin >> input;
              mpf_set_str(centerComplexY, input.c_str(), 10);
              valid = false;
              break;
            }
            case sf::Keyboard::Return:
              std::cout << "dimensions.x " << dimensions.x << std::endl
                << "dimensions.y " << dimensions.y << std::endl
                << "pixelPerUnit " << pixelPerUnit << std::endl
                << "maxIterations " << maxIterations << std::endl;
            default: break;
          }
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
  }

  return 0;
}
