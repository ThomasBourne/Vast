#define SFML_STATIC //not really needed but if used with g++/gcc this is definatly needed
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


void StartMenu(sf::Event&);

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


    //run splash screen
    SplashScreen(clock, evnt);

    //main menu
    StartMenu(evnt);
    save::PrintSave(&currentSave);

    if (currentSave.currentLevel > levels.size())
        currentSave.currentLevel = 0;

    //load the current level on from the current save file
    levels[currentSave.currentLevel].Load(); //both load and unload dont currently do anything
    levels[currentSave.currentLevel].PrintMap();
    levels[currentSave.currentLevel].Unload();
    currentSave.currentLevel++;
    save::SaveState tmpSave = currentSave;
    save::WriteSave(&tmpSave);

    level::TestLevel(evnt, animClock);

    return 0;
}

namespace menu {
    bool ClickedButton(sf::RectangleShape&);
}
bool menu::ClickedButton(sf::RectangleShape& button) {
    auto mousePos = sf::Mouse::getPosition(window); // Mouse position relative to the window
    auto translatedPos = window.mapPixelToCoords(mousePos); // Mouse position translated into world coordinates

    return (button.getGlobalBounds().contains(translatedPos)); // Rectangle-contains-point check
}

void StartMenu(sf::Event& evnt) {
    bool start = false;

    //change current savestate button
    sf::RectangleShape changeSaveButton(sf::Vector2f(200, 50));
    changeSaveButton.setFillColor(sf::Color::Blue);
    changeSaveButton.setPosition(300, 200);

    //exit button
    sf::RectangleShape exitButton(sf::Vector2f(200, 50));
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setPosition(changeSaveButton.getPosition() + sf::Vector2f(0, 200));

    while (window.isOpen() && !start) {
        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                CloseVast(0);
            if (evnt.type == sf::Event::MouseButtonPressed) {
                if (menu::ClickedButton(changeSaveButton)) {
                    std::string newPath = GetSaveFile();

                    if (newPath.substr(newPath.find_last_of(".") + 1) == "save") {
                        save::SaveState* newState = save::ReadSaveFromPath(newPath.c_str());

                        cashedSave = *(newState);
                        save::ChangeDefaultSave(&cashedSave, currentSave);
                    }
                }
                if (menu::ClickedButton(exitButton)) {
                    start = !start;
                }
            }
        }
        window.clear();
        window.draw(changeSaveButton);
        window.draw(exitButton);
        window.display();
    }
}