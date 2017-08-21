#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    int height = 720;
    int width = height*16/9;
    
    float pixelPerM = 720;
    float gravityX = 0, gravityY = 0;
    
    float mouseForce = 0.01;
    
    sf::Vector2i mouse;
    
    sf::Clock clock;
    float elapsedTime;
    
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    
    sf::CircleShape ball(100.f);
    ball.setFillColor(sf::Color::Green);
    ball.setOrigin(ball.getRadius(), ball.getRadius());
    
    float ballX = width/2 - ball.getRadius()/2;
    float ballY = height/2 - ball.getRadius()/2;
    float ballDx = 0;
    float ballDy = 9.81;
    
    clock.restart();
    while (window.isOpen()) {
        elapsedTime = clock.restart().asSeconds();
        
        // INPUT HANDLING
        
        mouse = sf::Mouse::getPosition(window);
        
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close(); break;
                case sf::Event::MouseButtonPressed:
                    ballX = mouse.x;
                    ballY = mouse.y;
                    ballDx = 0;
                    ballDy = 0;
                    break;
                case sf::Event::MouseWheelScrolled:
                    mouseForce += 0.01 * event.mouseWheelScroll.delta;
                    std::cout << mouseForce << " " << event.mouseWheelScroll.delta << std::endl;
                    break;
                default: break;
            }
        }
        
        // LOGIC
        
        // mouse
        ballDx += (mouse.x - ball.getPosition().x) * mouseForce * elapsedTime;
        ballDy += (mouse.y - ball.getPosition().y) * mouseForce * elapsedTime;
        
        // gravity
        ballDy += gravityY * elapsedTime;
        ballDx += gravityX * elapsedTime;
        
        // move position based on speed
        ballX += ballDx * pixelPerM * elapsedTime;
        ballY += ballDy * pixelPerM * elapsedTime;
        
        // set sprite position to variable position
        ball.setPosition(ballX, ballY);
        
        // RENDERING
        window.clear();
        window.draw(ball);
        window.display();
    }

    return 0;
}