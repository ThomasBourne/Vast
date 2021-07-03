#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "window.hpp"
#include "layers.hpp"
#include "smoke.hpp"
#include "floor.hpp"
#include "NPC.hpp"
#include "save.hpp"
#include "levels.hpp"




void inline SplashScreen(sf::Clock& time, sf::Event& evnt) { //function to display triangle at the start
    //THE SPLASH SCREEN TRIANGLE
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray triangle(sf::Triangles, 3);
    // define the position of the triangle's points
    triangle[0].position = sf::Vector2f(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - 100.f);
    triangle[1].position = sf::Vector2f((WINDOW_WIDTH / 2) - 100.f, (WINDOW_HEIGHT / 2) + 100.f);
    triangle[2].position = sf::Vector2f((WINDOW_WIDTH / 2) + 100.f, (WINDOW_HEIGHT / 2) + 100.f);
    // define the color of the triangle's points
    triangle[0].color = sf::Color::Red;
    triangle[1].color = sf::Color::Blue;
    triangle[2].color = sf::Color::Green;
    bool played = false;

    sf::SoundBuffer buffer;
    buffer.loadFromFile("C:\\dev\\vast\\src\\_assets\\SplashScreen\\SplashScreen.wav");
    sf::Sound sound;
    while (time.getElapsedTime().asMilliseconds() < 2500 && window.isOpen()) {
        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                CloseVast(0);
        }
        if (time.getElapsedTime().asSeconds() > 1.f && !played) {
                sound.setBuffer(buffer);
                sound.play();
                played = !played;
        }
        //window.clear(); //as Triangle gets bigger no need to refresh (saves 1 func call)
        window.draw(triangle);
        window.display();
        triangle[0].position += sf::Vector2f(.0f, -.1f);
        triangle[1].position += sf::Vector2f(-.1f, .1f);
        triangle[2].position += sf::Vector2f(.1f, .1f);
    }
    time.restart();
}

int main()
{
    //creates the default file if not already
    if (!save::FileExists(defaultSavePath.c_str())) {
        save::WriteSave(defaultSave);
    }

    //able to manually update the save file without having to delete the file
    #ifdef UpdateSave
        save::WriteSave(&defaultSave);
    #endif
    
    sf::Clock clock; //clock for things in Vast
    sf::Clock animClock; // clock for sprite animations
    sf::Event evnt; //window event handle

    //loads and sets texture on smoke
    smoke::texture.loadFromFile("C:\\dev\\vast\\src\\_assets\\player\\smoke_sheet.png");
    smoke::sprite = sf::Sprite(smoke::texture, smoke::rectSourceSprite);

    
    //run splash screen (deactivate for now)
    //SplashScreen(clock, evnt);

    level::TestLevel(evnt, animClock);

    return 0;
}