#ifndef SMOKE_HPP
#define SMOKE_HPP
#include "layers.hpp"
constexpr double GRAVITY = 0.5;
namespace smoke { //use a namespace as static class is slower i think and lol too late now
	sf::Texture texture;
	sf::Sprite sprite;

	Layers layer = Layers::Player;

	double jumpVel{ 0 };
	char facing{ 1 }; //1 is right 0 is left
	constexpr int SpriteWidth{ 128 };
	constexpr int SpriteHeight{ 64 };

	constexpr int SheetWidth{ 512 };
	constexpr int SheetHeight{ 448 };

	int currentTopLeft{ 0 };
	sf::IntRect rectSourceSprite(SpriteWidth, currentTopLeft, SpriteWidth, SpriteHeight);


	uint currentDourn{ 100 };
	uint health{ 30 };


	bool isGrounded() {
		return true;
	}


    void MovePlayer() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Joystick::isButtonPressed(0, sf::Joystick::AxisCount)) {
            smoke::sprite.move(1.f, 0.f);
            if (smoke::facing == 0) { //!= 1
                smoke::sprite.setTextureRect(sf::IntRect(smoke::currentTopLeft, smoke::currentTopLeft, smoke::SpriteWidth, smoke::SpriteHeight));
                smoke::facing = 1;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            smoke::sprite.move(-1.f, 0.f);
            if (smoke::facing == 1) { //== 1
                smoke::sprite.setTextureRect(sf::IntRect(smoke::SpriteWidth, smoke::currentTopLeft, -smoke::SpriteWidth, smoke::SpriteHeight));
                smoke::facing = 0;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::isButtonPressed(0, sf::Joystick::X)) {
            //jump machanics
            //add if grounded
            smoke::jumpVel = -3;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            //slide machanics
        }
        if (smoke::jumpVel <= GRAVITY) {
            smoke::jumpVel += .1f;
        }
        smoke::sprite.move(0.f, smoke::jumpVel);

        if (smoke::facing == 1) {
            smoke::sprite.setTextureRect(sf::IntRect(smoke::currentTopLeft, smoke::currentTopLeft, smoke::SpriteWidth, smoke::SpriteHeight));
        }
        else {
            smoke::sprite.setTextureRect(sf::IntRect(smoke::SpriteWidth, smoke::currentTopLeft, -smoke::SpriteWidth, smoke::SpriteHeight));
        }

        if (smoke::sprite.getGlobalBounds().top + smoke::sprite.getGlobalBounds().height > WINDOW_HEIGHT) {
            //CHANGE TO GAMEOVER SCREEN
            CloseVast(0);
        }
    }

    void MoveCamera() {

    }
}
#endif