#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

// Window dimensions
sf::Vector2i dimensions(800, 600);

// Define colors others apart from palette
sf::Color fgColor;
sf::Color bgColor = sf::Color::White;
sf::Color fxColor = sf::Color(200, 200, 200);

// Width of the lines
float lineWidth = 2;
float paintWidth = 2;

// This file contains ring, wheel, hole and other config data
std::ifstream configFile;

// Change speed with mouse wheel
float degreesPerFrame = 1;
float degreesPerFrameStep = 0.1;

float lengthMultiplier;
std::vector<sf::RectangleShape> lines(3);

// Values for spirograph
float ring;
float wheel;
float hole;
float step;

// Keep ring's and wheel's last rotation to calculate full rotations
float lastRingRotation;
float lastWheelRotation;

// Angle at which the spirograph starts
float rotation;

// Palette to store all colors in
std::vector<sf::Color> palette;
int paletteIndex;

// Function declarations
sf::Vector2f getRotatedVector(sf::RectangleShape shape, int point);
int slowestLineIndex(std::vector<float> degreesPerSecond);
void updateLines(std::vector<sf::RectangleShape>& lines);
void drawCircleOutline(sf::RenderWindow& window, int x, int y, float r, sf::Color color);
void nextConfig();
void nextHole();

int main() {
  // Take config path from std::cin
  std::string configFilePath;
  std::cin >> configFilePath;
  configFile.open(configFilePath);
  nextConfig();

  lengthMultiplier = (dimensions.y/2 - 1)/ring;
  // Pixelarray for drawn pixels
  sf::VertexArray paint(sf::TrianglesStrip);

  // Create window
  sf::RenderWindow window(sf::VideoMode(dimensions.x, dimensions.y), "Spirograph");
  window.setFramerateLimit(30);

  // Set rotation and color of all spirograph lines
  for(uint i = 0; i < lines.size(); i++) {
    lines[i] = sf::RectangleShape();
    lines[i].rotate(rotation);
    lines[i].setFillColor(fxColor);
  }
  // Update line lengths from global variables ring, wheel and hole
  updateLines(lines);


  // Move first line to screen center
  lines[0].setPosition(sf::Vector2f(dimensions.x/2, dimensions.y/2));

  bool linesVisible = true;

  // MAINLOOP
  while(window.isOpen()) {
    // Check for events
    sf::Event event;
    while(window.pollEvent(event)) {
      // Check if event is one of the following types
      switch(event.type) {
        // Close window
        case sf::Event::Closed:
          window.close();
          break;
          // Scrollwheel to change degreesPerFrame
        case sf::Event::MouseWheelScrolled:
          {
            // Change degreesPerFrame by amount scrolled
            degreesPerFrame += event.mouseWheelScroll.delta * degreesPerFrameStep;
            // Check minimum and maximum
            if(degreesPerFrame < 0) degreesPerFrame = 0;

            // Hide lines if degreesPerFrame is 0
            if(degreesPerFrame == 0) linesVisible = false;
            else linesVisible = true;
            break;
          }
        case sf::Event::KeyPressed:
          switch(event.key.code) {
            case sf::Keyboard::Space:
              linesVisible = !linesVisible;
              break;
            case sf::Keyboard::N:
              nextHole();
              break;
            case sf::Keyboard::Return:
              nextConfig();
              break;
            default: break;
          }
        default:
          break;
      }
    }

    // LOGIC

    if(linesVisible != 0) {
      // Rotate each line
      lines[0].rotate(degreesPerFrame);
      lines[1].rotate(degreesPerFrame * -(ring - wheel) / wheel);
      lines[2].setRotation(lines[1].getRotation());
      // Move each line
      for(uint i = 1; i < lines.size(); i++) {
        // Move position to the end of the previous line
        lines[i].setPosition(getRotatedVector(lines[i-1], 1));
      }
      sf::Vector2f pos = getRotatedVector(lines.back(), 1);
      paint.append(sf::Vertex(sf::Vector2f(pos.x-paintWidth, pos.y), fgColor));
      paint.append(sf::Vertex(sf::Vector2f(pos.x+paintWidth, pos.y), fgColor));
      paint.append(sf::Vertex(sf::Vector2f(pos.x, pos.y-paintWidth), fgColor));
      paint.append(sf::Vertex(sf::Vector2f(pos.x, pos.y+paintWidth), fgColor));
    }

    float ringRotation = lines[0].getRotation();
    float wheelRotation = lines[1].getRotation();

    if(lastRingRotation < rotation && lastWheelRotation < rotation) {
      if(ringRotation >= rotation && wheelRotation >= rotation)
        std::cout << "SUCCESS" << std::endl;
    }
    //std::cout << lastRingRotation << " " << lastWheelRotation << " " << rotation << std::endl;
    //std::cout << ringRotation << " " << wheelRotation << "\n" << std::endl;

    lastRingRotation = ringRotation;
    lastWheelRotation = wheelRotation;
    // RENDER

    // Fill the window with background color
    window.clear(bgColor);

    // Draw each line
    window.draw(paint);
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
  float hole_temp = hole * 2.3 + 10;
  lines[0].setSize(sf::Vector2f((ring-wheel) * lengthMultiplier, lineWidth));
  lines[1].setSize(sf::Vector2f(wheel * lengthMultiplier, lineWidth));
  lines[2].setSize(sf::Vector2f(-hole_temp * lengthMultiplier, lineWidth));
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

void nextConfig() {
  palette.clear();
  std::string word;
  while(configFile >> word && word[0] != '=') {
    if(word[0] == '+') {
      float r, g, b, a;
      configFile >> r >> g >> b >> a;
      std::cout << r << " " << g << " " << b << " " << a << std::endl;
      palette.push_back(sf::Color(r, g, b, a));
    }
  }
  configFile >> ring >> wheel >> hole >> rotation >> step;
  std::cout << ring << " " << wheel << " " << " " << hole << " " << rotation << " " << step << std::endl;
  paletteIndex = 0;
  fgColor = palette[paletteIndex];
}

void nextHole() {
  fgColor = palette[++paletteIndex % palette.size()];
  hole += step;
  updateLines(lines);
}
