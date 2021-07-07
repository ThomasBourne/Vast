#ifndef LEVELS_HPP
#define LEVELS_HPP

#include "smoke.hpp"
#include "floor.hpp"
#include "window.hpp"
#include "_assets/Levels/maps.hpp"

using smoke::MoveCamera;
using smoke::MovePlayer;

class Level {
public:
    std::vector<std::vector<int>> map;

    Level(std::vector<std::vector<int>> map) {
        this->map = map;
    }

    void PrintMap() {
        for (int y = 0; y < this->map.size(); y++) {
            for (int x = 0; x < this->map[0].size(); x++) {
                std::cout << map[y][x];
            }
            std::cout << std::endl;
        }
    }

    void Load() {
        std::cout << "lol" << std::endl;
    }

    void Unload() {
        std::cout << "Works" << std::endl;
    }
};

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
            bool collided = false;
            for (std::vector<Chunk>::iterator it = chunks.begin(); it != chunks.end(); ++it) {
                it->render();
                for (auto& floorRow : it->chunk) {
                    for (Floor& floor : floorRow) {
                        if (smoke::sprite.getGlobalBounds().intersects(floor.collider.getGlobalBounds())) {
                            collided = !collided;
                        }
                    }
                }
            }
            window.draw(smoke::sprite); //draw smoke to the current buffer
            window.display(); //switch buffers
            MovePlayer((!collided)); //calculate players new position
            MoveCamera(); //move all entites reletive to the player
        }
    }
}


Level lev0(maps::l0);
Level lev1(maps::l1);
Level lev2(maps::l2);

std::vector<Level> levels{ lev0, lev1, lev2 };
#endif