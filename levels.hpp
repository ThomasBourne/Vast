#ifndef LEVELS_HPP
#define LEVELS_HPP


#include "smoke.hpp"
#include "floor.hpp"
#include "window.hpp"

using smoke::MoveCamera;
using smoke::MovePlayer;

namespace level {
    void TestLevel(sf::Event& evnt, sf::Clock& animClock) {


        //Just a test chunk
        Chunk testChunk(sf::Vector2f{ TILE_SIZE * 10, TILE_SIZE * 3 }, chunks);



        //main loop that the game runs in
        while (window.isOpen()) {
            while (window.pollEvent(evnt)) { //checks and handle the handle
                if (evnt.type == sf::Event::Closed)
                    CloseVast(0);
            }

            //smoke animation loop THAT IS VERY BROKEN
            if (animClock.getElapsedTime().asMilliseconds() > 200) {
                if (smoke::rectSourceSprite.left == smoke::SheetWidth - smoke::SpriteWidth) {
                    smoke::rectSourceSprite.left = 0;
                    smoke::currentTopLeft = 0;
                }
                else {
                    smoke::rectSourceSprite.left += smoke::SpriteWidth;
                    smoke::currentTopLeft += smoke::SpriteWidth;
                }
                smoke::sprite.setTextureRect(smoke::rectSourceSprite);
                animClock.restart();
            }


            window.clear(); //clear current buffer
            //to render all floor entities
            for (std::vector<Chunk>::iterator it = chunks.begin(); it != chunks.end(); ++it) {
                it->render();
            }
            window.draw(smoke::sprite); //draw smoke to the current buffer
            window.display(); //switch buffers
            MovePlayer(); //calculate players new position
            MoveCamera(); //move all entites reletive to the player
        }
    }
}

#endif