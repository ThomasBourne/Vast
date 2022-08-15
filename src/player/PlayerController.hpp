#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <array>
#include <iostream> //delete me at some point please :)

#include "physics.hpp"

#include "../render/level.hpp"

using Keyboard = sf::Keyboard;

#define KEY (Keyboard::Key)
#define INT (int)

class Player : public sf::RectangleShape {
//private:
public:
	sf::Vector2f velocity;
	const sf::Vector2f acceleration = sf::Vector2f(0.05f, 10.f);;
	const sf::Vector2f sprintAcceleration = sf::Vector2f(acceleration.x * 2, acceleration.y);
	const float maxSpeed = 8.f;
	const float maxSprintSpeed = 15.f;
	const float maxFallSpeed = 5.f;
	ss::SaveState save;

	unsigned int health = 100;
	unsigned int maxHealth = 100;

	static enum class Keybinds { //dont forget to set in SetKeybinds()
		Up = 0,
		Sprint = 1,
		Left = 2,
		Right = 3,
		CameraUp,
		CameraDown,
		CameraLeft,
		CameraRight,
		ZoomIn,
		ZoomOut,
		RotateLeft,
		RotateRight,
		Menu,
		FreeCamera,
		Minimap,
		count
	};

	int* keybinds; //array for movement keybinds that pairs with enum Keybinds above
public:
	Player(sf::Vector2f size, ss::SaveState& save) : sf::RectangleShape(size) {
		InitKeybinds();
		velocity = sf::Vector2f(0.f, 0.f);
		this->setPosition(sf::Vector2f(-300, 0));
		this->save = save;
		this->SetKeybinds();
	}

	void SetSave(std::string savePath) {
		this->save = ss::SaveState(savePath);
	}
	void SetSave(ss::SaveState save) {
		this->save = save;
	}

	void Move(const bool& grounded, const bool& collided, const std::array<std::vector<sf::Vector2f>, 4>& colliders) {
		if (!collided) {
			/*const auto& accelerationType = (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Sprint]) ? sprintAcceleration : acceleration);
			const auto& maxSpeedType = (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Sprint]) ? maxSprintSpeed : maxSpeed);
			if (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Right])) {
				if (velocity.x < maxSpeedType) {
					velocity.x += accelerationType.x;
				}
			}
			else if (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Left])) {
				if (velocity.x > -maxSpeedType) {
					velocity.x -= accelerationType.x;
				}
			}
			else if (velocity.x < 0.2f && velocity.x > -0.2f) {
				velocity.x = 0;
			}
			else {
				if (velocity.x > 0.f) {
					velocity.x -= accelerationType.x;
				}
				else {
					velocity.x += accelerationType.x;
				}
			}
			*/
			if (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Right])) {
				this->move(sf::Vector2f(.5f, .0f));
			}
			if (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Left])) {
				this->move(sf::Vector2f(-.5f, .0f));
			}
		}

		//generate colliders
		std::array<cldr::ColliderShape, 4> colliderShapes;
		for (int i = 0; i < 4; i++) {
			std::cout << '\r' << colliders[0][0].x << " : " << colliders[0][0].y << "           ";
			colliderShapes[i] = colliders[i];
		}

		if (colliderShapes[2].Collided(this->getGlobalBounds())) {
			exit(0);
		}

		//if (this->getPosition().y >= 1000 || grounded) {
			//velocity.y = 0.f;
			//if(this->getPosition().y >= 500)
			//	this->setPosition(this->getPosition().x, 1000);
			if (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Up])) { //figure out multiple keys per bind e.g space and w
				//velocity.y -= acceleration.y;
				this->move(sf::Vector2f(0.f, -.5f));
			}
			if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
				this->move(sf::Vector2f(0.f, .5f));
			}

		//}
		/*else {
			if (velocity.y <= maxFallSpeed)
				velocity.y += gravity;
			else
				velocity.y = maxFallSpeed;
		}*/
		this->move(velocity); //move() is a sf shape method... Move() is a Player method
	}
private:
	inline void InitKeybinds() {
		this->keybinds = new int[(int)Keybinds::count];
	}
public:
	inline void SetKeybinds() { //this REALLY sucks, can this be made automatic somehow... the whole reason its in an array is to make it quicker to access as its gonna be slow calling these EVERY frame
		*(this->keybinds + (int)Keybinds::Up) = this->save.raw["game settings"]["key binds"]["up"].get<int>();
		*(this->keybinds + (int)Keybinds::Sprint) = this->save.raw["game settings"]["key binds"]["sprint"].get<int>();
		*(this->keybinds + (int)Keybinds::Left) = this->save.raw["game settings"]["key binds"]["left"].get<int>();
		*(this->keybinds + (int)Keybinds::Right) = this->save.raw["game settings"]["key binds"]["right"].get<int>();

		*(this->keybinds + (int)Keybinds::CameraUp) = this->save.raw["game settings"]["key binds"]["camera up"].get<int>();
		*(this->keybinds + (int)Keybinds::CameraDown) = this->save.raw["game settings"]["key binds"]["camera down"].get<int>();
		*(this->keybinds + (int)Keybinds::CameraLeft) = this->save.raw["game settings"]["key binds"]["camera left"].get<int>();
		*(this->keybinds + (int)Keybinds::CameraRight) = this->save.raw["game settings"]["key binds"]["camera right"].get<int>();

		*(this->keybinds + (int)Keybinds::ZoomIn) = this->save.raw["game settings"]["key binds"]["zoom in"].get<int>();
		*(this->keybinds + (int)Keybinds::ZoomOut) = this->save.raw["game settings"]["key binds"]["zoom out"].get<int>();
		*(this->keybinds + (int)Keybinds::RotateLeft) = this->save.raw["game settings"]["key binds"]["rotate left"].get<int>();
		*(this->keybinds + (int)Keybinds::RotateRight) = this->save.raw["game settings"]["key binds"]["rotate right"].get<int>();

		*(this->keybinds + (int)Keybinds::Menu) = this->save.raw["game settings"]["key binds"]["menu toggle"].get<int>();

		*(this->keybinds + (int)Keybinds::FreeCamera) = this->save.raw["game settings"]["key binds"]["toggle free camera"].get<int>();
		*(this->keybinds + (int)Keybinds::Minimap) = this->save.raw["game settings"]["key binds"]["toggle minimap"].get<int>();
	}
public:
	~Player() {
		delete[] this->keybinds;
	}
};

#define KEY_BIND(player, Key) KEY (##player).keybinds[INT Player::Keybinds::Key]

#endif