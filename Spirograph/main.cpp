#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Window dimensions
sf::Vector2i dimensions(800, 600);

// Values for Spirograph
float ring = 105;
float wheel = 63;
float hole = 1;

int paletteIndex = 0;

std::vector<sf::Color> palette = {
  sf::Color(33, 60, 132),
  sf::Color(48, 88, 188),
  sf::Color(195, 175, 227),
  sf::Color(108, 86, 169),
  sf::Color(230, 102, 175),
  sf::Color(200, 85, 96),
  sf::Color(236, 102, 93),
  sf::Color(235, 73, 63),
  sf::Color(208, 128, 91),
  sf::Color(234, 180, 159),
  sf::Color(237, 211, 83)
};

sf::Color fgColor = palette[paletteIndex];
sf::Color bgColor = sf::Color::White;
sf::Color fxColor = sf::Color(200, 200, 200);

// Width of the lines
float lineWidth = 2;

// Change speed with mouse wheel
float speed = 30;
float speedStep = 5;
float maxSpeed = 360;

float lengthMultiplier;

sf::Vector2f getRotatedVector(sf::RectangleShape shape, int point);
int slowestLineIndex(std::vector<float> degreesPerSecond);
void updateLines(std::vector<sf::RectangleShape>& lines);
void drawCircleOutline(sf::RenderWindow& window, int x, int y, float r, sf::Color color);

int main() {
  lengthMultiplier = (dimensions.y/2 - 1)/ring;
  // Pixelarray for drawn pixels
  sf::VertexArray pixels(sf::Points);
  for(int y = 0; y < dimensions.y; y++) {
    for(int x = 0; x < dimensions.x; x++) {
      pixels.append(sf::Vertex(sf::Vector2f(x, y), bgColor));
    }
  }

  // Create window
  sf::RenderWindow window(sf::VideoMode(dimensions.x, dimensions.y), "Spirograph");

  std::vector<sf::RectangleShape> lines(3);

  for(uint i = 0; i < lines.size(); i++) {
    lines[i] = sf::RectangleShape();
    lines[i].setRotation(270);
    lines[i].setFillColor(fxColor);
  }
  updateLines(lines);


  // Move first line to screen center
  lines[0].setPosition(sf::Vector2f(dimensions.x/2, dimensions.y/2));
  
  bool linesVisible = true;

  // Create Clock to measure time between frames
  // And move accordingly
  sf::Clock clock;
  float elapsedTime;

  // MAINLOOP
  while(window.isOpen()) {
    // Get time since last check
    elapsedTime = clock.restart().asSeconds();

    // Check for events
    sf::Event event;
    while(window.pollEvent(event)) {
      // Check if event is one of the following types
      switch(event.type) {
        // Close window
        case sf::Event::Closed:
          window.close();
          break;
          // Scrollwheel to change speed
        case sf::Event::MouseWheelScrolled:
          {
            // Change speed by amount scrolled
            speed += event.mouseWheelScroll.delta * speedStep;
            // Check minimum and maximum
            if(speed > maxSpeed) speed = maxSpeed ;
            if(speed < 0) speed = 0;

            // Hide lines if speed is 0
            if(speed == 0) linesVisible = false;
            else linesVisible = true;
            break;
          }
        case sf::Event::KeyPressed:
          switch(event.key.code) {
            case sf::Keyboard::Space:
            {
              std::cout << "Please input: ring wheel hole" << std::endl;
              std::cin >> ring;
              std::cin >> wheel;
              std::cin >> hole;
              updateLines(lines);
            }
            case sf::Keyboard::R:
            {
              std::cout << "Please input: ring" << std::endl;
              std::cin >> ring;
              updateLines(lines);
            }
            case sf::Keyboard::W:
            {
              std::cout << "Please input: wheel" << std::endl;
              std::cin >> ring;
              updateLines(lines);
            }
            case sf::Keyboard::H:
            {
              std::cout << "Please input: hole" << std::endl;
              std::cin >> hole;
              updateLines(lines);
            }
            case sf::Keyboard::C:
            {
              std::cout << "Please input: r g b" << std::endl;
              int r, g, b;
              std::cin >> r >> g >> b;
              fgColor = sf::Color(r, g, b);
            }
            case sf::Keyboard::N:
              fgColor = palette[++paletteIndex];
              hole += 2;
              updateLines(lines);
              break;
            default: break;
          }
          // Default for other events
        default:
          break;
      }
    }

    // LOGIC

    // Rotate each line
    lines[0].rotate(speed * elapsedTime);
    lines[1].rotate(speed * -(ring - wheel) / wheel * elapsedTime);
    lines[2].setRotation(lines[1].getRotation());
    // Move each line
    for(uint i = 1; i < lines.size(); i++) {
      // Move position to the end of the previous line
      lines[i].setPosition(getRotatedVector(lines[i-1], 1));
    }
    if(speed != 0) {
      sf::Vector2f pos = getRotatedVector(lines.back(), 1);
      pixels[int(pos.y)*(dimensions.x) + int(pos.x)].color = fgColor;
    }

    // RENDER

    // Fill the window with background color
    window.clear();

    // Draw each line
    window.draw(pixels);
    if(linesVisible) {
      window.draw(lines[1]);
      sf::Vector2f pos(getRotatedVector(lines[1], 0));
      drawCircleOutline(window, pos.x, pos.y, lines[1].getSize().x, fxColor);
    }

    drawCircleOutline(window, dimensions.x/2, dimensions.y/2, ring * lengthMultiplier, fxColor);

    // Swap buffers to show the image to the screen
    window.display();
  }

  // Program ended without errors
  return 0;
}

sf::Vector2f getRotatedVector(sf::RectangleShape shape, int point) {
  // You can't just use the shape.getPoint() method, because it doesn't
  // take the transformations (rotations) into consideration
  return shape.getTransform().transformPoint(shape.getPoint(point));
}

void updateLines(std::vector<sf::RectangleShape>& lines) {
  wheel *= 2;
  lines[0].setSize(sf::Vector2f((ring-wheel) * lengthMultiplier, lineWidth));
  lines[1].setSize(sf::Vector2f(wheel * lengthMultiplier, lineWidth));
  lines[2].setSize(sf::Vector2f(-hole * lengthMultiplier, lineWidth));
}

void drawCircleOutline(sf::RenderWindow& window, int x, int y, float r, sf::Color color) {
  sf::CircleShape circle(r, 100); 
  circle.setOrigin(r, r);
  circle.setPosition(x, y);
  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(1);
  circle.setOutlineColor(color);
  window.draw(circle);
}
