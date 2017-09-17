#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

sf::Vector2i dimensions(800, 600);

int fps = 5;
int stepsPerFrame = 1;
int maximumValue = dimensions.y;
int values = 15;
int gapSize = 1;

// Setup random numbers
std::random_device rd;
std::mt19937_64 generator(rd());
std::uniform_int_distribution<unsigned int> valueRange(0, maximumValue);



// SORTER

class Sorter {
  protected:
    int barWidth;
    int step = 0;
    bool finished = false;
    std::vector<int> items;
    std::vector<sf::Color> colors;

    void swap(const int i1, const int i2);
    void calculateBarWidth();
    void clearColors();
    int getHeigthFromValue(const int value) const;

    Sorter(const std::vector<int> &i);
    Sorter();

  public:
    void generate(std::random_device &rd, std::uniform_int_distribution<unsigned int> &valueRange);
    void draw(sf::RenderWindow &window);
    bool isFinished() const;

    virtual void next() = 0;
};

Sorter::Sorter(const std::vector<int> &i) : items(i), colors(i.size()) {
  calculateBarWidth();
  clearColors();
}

Sorter::Sorter() : items() { }

void Sorter::generate(std::random_device &rd, std::uniform_int_distribution<unsigned int> &valueRange) {
  items.clear();
  for(int i = 0; i < values; i++) {
    items.push_back(valueRange(rd));
  }
  calculateBarWidth();
  clearColors();
}

void Sorter::swap(const int i1, const int i2) {
  int temp = items[i1];
  items[i1] = items[i2];
  items[i2] = temp;

  colors[i1] = sf::Color::Yellow;
  colors[i2] = sf::Color::Yellow;
}

void Sorter::draw(sf::RenderWindow &window) {
  sf::RectangleShape rect;
  for(unsigned long i = 0; i < values; i++) {
    int barHeight = getHeigthFromValue(items[i]);
    rect.setPosition(i * barWidth + i * gapSize, dimensions.y - barHeight);
    rect.setSize(sf::Vector2f(barWidth, barHeight));
    rect.setFillColor(colors[i]);
    window.draw(rect);
  }
}


void Sorter::calculateBarWidth() {
  barWidth = dimensions.x / values - gapSize;
}

void Sorter::clearColors() {
  colors.clear();
  for(int i = 0; i < values; i++) colors.push_back(sf::Color::White);
}

int Sorter::getHeigthFromValue(const int value) const {
  return value * dimensions.y / maximumValue;
}

bool Sorter::isFinished() const {
  return finished;
}



// BUBBLESORTER

class BubbleSorter : public Sorter {
  int index = 1;
  int passes = 0;
  public:
  BubbleSorter(const std::vector<int> &i);
  BubbleSorter();
  void next();
};

BubbleSorter::BubbleSorter(const std::vector<int> &i) : Sorter(i) {}
BubbleSorter::BubbleSorter() : Sorter() {}

void BubbleSorter::next() {
  if(finished) return;
  clearColors();

  index += 1;
  if(index >= values - passes) {
    index = 1;
    passes++;
    if(passes >= values) finished = true;
  }
  if(index >= values - passes) passes++;

  if(items[index-1] > items[index]) {
    swap(index-1, index);
  } else colors[index] = sf::Color::Red;
  step++;

  for(int i = values - passes; i < values; i++) {
    colors[i] = sf::Color::Green;
  }
}




// MAIN

int main() {

  BubbleSorter sorter;
  sorter.generate(rd, valueRange);

  sf::RenderWindow window(sf::VideoMode(dimensions.x, dimensions.y), "SortingAlgorithms");
  window.setFramerateLimit(fps);

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::KeyPressed:
          switch(event.key.code) {
            case sf::Keyboard::Space:
              sorter.next();
              break;
            default: break;
          }
        default: break;
      }
    }

    if(!sorter.isFinished()) {
      window.clear(sf::Color::Black);
      for(int i = 0; i < stepsPerFrame; i++)
        sorter.next();
      sorter.draw(window);
      window.display();
    }
  }
  return 0;
}
