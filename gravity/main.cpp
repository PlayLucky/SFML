#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

sf::Vector2i dimensions(800, 600);

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<float> randomFloat(0, 1);


// PARTICLE

class Particle {
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Vector2f acceleration;

  public:
    Particle(const sf::Vector2f &p);
    Particle(const sf::Vector2f &p, sf::Vector2f &v);
    void update(const float deltaTime);
    void applyForce(const sf::Vector2f &force);
    void draw(sf::RenderWindow &window) const;
};

Particle::Particle(const sf::Vector2f &p) : position(p), velocity(0,0) {}
Particle::Particle(const sf::Vector2f &p, sf::Vector2f &v) : position(p), velocity(v) {}

void Particle::update(const float deltaTime) {
  velocity += acceleration * deltaTime;
  position += velocity * deltaTime;
  acceleration *= 0.f;

  if(position.x < 0) {
    velocity.x *= -1.f;
    position.x = 0;
  }
  if(position.x > dimensions.x) {
    velocity.x *= -1.f;
    position.x = dimensions.x;
  }
  if(position.y < 0) {
    velocity.y *= -1.f;
    position.y = 0;
  }
  if(position.y > dimensions.y) {
    velocity.y *= -1.f;
    position.y = dimensions.y;
  }
}

void Particle::applyForce(const sf::Vector2f &force) {
  acceleration += force;
}

void Particle::draw(sf::RenderWindow &window) const {
  sf::CircleShape c(5);
  c.setOrigin(5, 5);
  c.setPosition(position);
  c.setFillColor(sf::Color::White);
  window.draw(c);
}

// MAIN

int main() {
  std::vector<Particle> particles;
  for(int i = 0; i < 10; i++) {
    particles.push_back(Particle(sf::Vector2f(randomFloat(rng)*dimensions.x, randomFloat(rng)*dimensions.y)));
  }

  sf::RenderWindow window(sf::VideoMode(dimensions.x, dimensions.y), "Physics");
  window.setFramerateLimit(30);

  sf::Clock clock;
  float deltaTime(clock.getElapsedTime().asSeconds());

  while(window.isOpen()) {
    deltaTime = clock.restart().asSeconds();

    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        default: break;
      }
    }

    for(Particle &p : particles) {
      p.applyForce(sf::Vector2f(100, 900));
      p.update(deltaTime);
    }

    window.clear(sf::Color::Black);

    for(Particle &p : particles) {
      p.draw(window);
    }

    window.display();
  }
  return 0;
}
