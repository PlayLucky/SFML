#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

sf::Vector2f getRotatedVector(sf::RectangleShape shape, int point);
int slowestLineIndex(std::vector<float> degreesPerSecond);

int main(int argc, char **argv)
{
    // SETUP
    
    // setup window
    const int width = 800;
    const int height = 600;
    
    // width of the lines
    float lineWidth = 2;
    
    // length of the lines
    std::vector<float> lengthInPixels = {
        50, 20
    };
    
    // rotation speed of the lines
    std::vector<float> degreesPerSecond = {
        51, 100
    };
    
    // zoom in
    float lengthMultiplier = 3;
    
    // check if the vectors have the same size (each line has a corresponding speed)
    if(lengthInPixels.size() != degreesPerSecond.size()) {
        std::cerr << "'lengthInPixels' and 'degreesPerSecond'\
        vectors must have the same size" << std::endl;
        return -1;
    }
    
    int slowest = slowestLineIndex(degreesPerSecond);
    int slowestLastRotation = 0;
    int revolutions = 0;
    
    // change speed with mouse wheel
    float speedMultiplier = 1;
    float maxSpeedMultiplier = 10;
    
    // pixelarray for drawn pixels
    sf::VertexArray paint(sf::LineStrip);
    
    //  create window
    sf::RenderWindow window(sf::VideoMode(width, height), "Spirograph");
    
    // create lines with specified length in the 'lines' array
    std::vector<sf::RectangleShape> lines;
    for(float length: lengthInPixels) {
        sf::RectangleShape line(sf::Vector2f(length * lengthMultiplier, lineWidth));
        line.setFillColor(sf::Color::Red);
        lines.push_back(line);
    }
    
    // move first line to screen center
    lines[0].setPosition(sf::Vector2f(width/2, height/2));
    
    bool linesVisible = true;
    
    // create Clock to measure time between frames
    // and move accordingly
    sf::Clock clock;
    float elapsedTime;
    
    // MAINLOOP
    while(window.isOpen()) {
        // get time since last check
        elapsedTime = clock.restart().asSeconds();
        
        // check for events
        sf::Event event;
        while(window.pollEvent(event)) {
            // check if event is one of the following types
            switch(event.type) {
                // close window
                case sf::Event::Closed:
                    window.close();
                    break;
                // scrollwheel to change speed
                case sf::Event::MouseWheelScrolled:
                {
                    // change speed by amount scrolled
                    speedMultiplier += event.mouseWheelScroll.delta;
                    // check minimum and maximum
                    if(speedMultiplier > maxSpeedMultiplier) speedMultiplier = maxSpeedMultiplier;
                    if(speedMultiplier < 0) speedMultiplier = 0;
                    
                    //hide lines if speed is 0
                    if(speedMultiplier == 0) linesVisible = false;
                    else linesVisible = true;
                    break;
                }
                // default for other events
                default:
                    break;
            }
        }
        
        // LOGIC
        
        // rotate and move each line
        for(uint i = 0; i < lines.size(); i++) {
            // rotate by amount specified in 'degreesPerSecond'
            // vector at the line's index
            lines[i].rotate(degreesPerSecond[i]*elapsedTime*speedMultiplier);
            
            //move position to the end of the previous line
            if(i != 0) {
                lines[i].setPosition(getRotatedVector(lines[i-1], 1));
            }
        }
        if(speedMultiplier != 0) paint.append(getRotatedVector(lines.back(), 1));
        
        if(lines[slowest].getRotation() > 0)
            if(lines[slowest].getRotation() < slowestLastRotation) {
                revolutions++;
            }
        else
            if(lines[slowest].getRotation() > slowestLastRotation) {
                revolutions++;
            }
        std::cout << "slowest=" << slowest << " Revolutions=" << revolutions << " slowestLastRotation=" << slowestLastRotation << " slowestRotation=" << lines[slowest].getRotation() << std::endl;
        slowestLastRotation = lines[slowest].getRotation();
        
        // RENDER
        
        // fill the window with background color
        window.clear(sf::Color::Black);
        
        // draw each line
        window.draw(paint);
        if(linesVisible)
            for(sf::RectangleShape line: lines) window.draw(line);
        
        // swap buffers to show the image to the screen
        window.display();
    }
    
    // Program ended without errors
    return 0;
}

sf::Vector2f getRotatedVector(sf::RectangleShape shape, int point) {
    // you cant just use the shape.getPoint() method, because it doesn't
    // take the transformations (rotation) into consideration
    return shape.getTransform().transformPoint(shape.getPoint(point));
}

// find out slowest line to use in calculations for rotations
int slowestLineIndex(std::vector<float> degreesPerSecond) {
    int slowest = 0;
    for(uint i = 0; i < degreesPerSecond.size(); i++) { // TODO NEGATIVE NUMBERS
        if(degreesPerSecond[i] < slowest) slowest = i;
    }
    return slowest;
}
