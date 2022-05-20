#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include <SFML/Graphics.hpp>

#include <string>

#include "../save/savestate.hpp"
#include "physics.hpp"

#define KEY (Keyboard::Key)
#define INT (int)

class Player : public sf::RectangleShape {
//private:
public:
	sf::Vector2f velocity;
	const sf::Vector2f acceleration = sf::Vector2f(0.05f, 10.f);;
	const float maxSpeed = 8.f;
	const float maxFallSpeed = 5.f;
	ss::SaveState save;

	unsigned int health = 100;
	unsigned int maxHealth = 100;

	enum class Keybinds { //dont forget to set in SetKeybinds()
		Up = 0,
		Crouch = 1,
		Left = 2,
		Right = 3,
		CameraUp,
		CameraDown,
		CameraLeft,
		CameraRight,
		ZoomIn,
		ZoomOut,
		Menu,
		count
	};

	int* keybinds; //array for movement keybinds that pairs with enum Keybinds above
public:
	Player(sf::Vector2f size, std::string savePath) : sf::RectangleShape(size) {
		InitKeybinds();
		velocity = sf::Vector2f(0.f, 0.f);
		save = ss::SaveState(savePath);
		SetKeybinds();
	}
	Player(sf::Vector2f size, ss::SaveState& save) : sf::RectangleShape(size) {
		InitKeybinds();
		velocity = sf::Vector2f(0.f, 0.f);
		this->save = save;
		SetKeybinds();
	}

	void SetSave(std::string savePath) {
		this->save = ss::SaveState(savePath);
	}
	void SetSave(ss::SaveState save) {
		this->save = save;
	}

	void Move(const bool& grounded, const bool& collided) {
		if (!collided) {
			if (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Right])) {
				if (velocity.x < maxSpeed) {
					velocity.x += acceleration.x;
				}
			}
			else if (Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Left])) {
				if (velocity.x > -maxSpeed) {
					velocity.x -= acceleration.x;
				}
			}
			else if (velocity.x < 0.2f && velocity.x > -0.2f) {
				velocity.x = 0;
			}
			else {
				if (velocity.x > 0.f) {
					velocity.x -= acceleration.x;
				}
				else {
					velocity.x += acceleration.x;
				}
			}
		}

		if (this->getPosition().y >= 500 || grounded) {
			velocity.y = 0.f;
			if(this->getPosition().y >= 500)
				this->setPosition(this->getPosition().x, 500);
			if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(KEY this->keybinds[INT Keybinds::Up])) { //figure out multiple keys per bind e.g space and w
				velocity.y -= acceleration.y;
			}

		}
		else {
			if (velocity.y <= maxFallSpeed)
				velocity.y += gravity;
			else
				velocity.y = maxFallSpeed;
		}
		this->move(velocity); //move() is a sf shape method... Move() is a Player method
	}
private:
	inline void InitKeybinds() {
		this->keybinds = new int[(int)Keybinds::count];
	}
public:
	inline void SetKeybinds() { //this REALLY sucks, can this be made automatic somehow... the whole reason its in an array is to make it quicker to access as its gonna be slow calling these EVERY frame
		*(this->keybinds + (int)Keybinds::Up) = this->save.raw["game settings"]["key binds"]["up"].get<int>();
		*(this->keybinds + (int)Keybinds::Crouch) = this->save.raw["game settings"]["key binds"]["crouch"].get<int>();
		*(this->keybinds + (int)Keybinds::Left) = this->save.raw["game settings"]["key binds"]["left"].get<int>();
		*(this->keybinds + (int)Keybinds::Right) = this->save.raw["game settings"]["key binds"]["right"].get<int>();

		*(this->keybinds + (int)Keybinds::CameraUp) = this->save.raw["game settings"]["key binds"]["camera up"].get<int>();
		*(this->keybinds + (int)Keybinds::CameraDown) = this->save.raw["game settings"]["key binds"]["camera down"].get<int>();
		*(this->keybinds + (int)Keybinds::CameraLeft) = this->save.raw["game settings"]["key binds"]["camera left"].get<int>();
		*(this->keybinds + (int)Keybinds::CameraRight) = this->save.raw["game settings"]["key binds"]["camera right"].get<int>();

		*(this->keybinds + (int)Keybinds::ZoomIn) = this->save.raw["game settings"]["key binds"]["zoom in"].get<int>();
		*(this->keybinds + (int)Keybinds::ZoomOut) = this->save.raw["game settings"]["key binds"]["zoom out"].get<int>();

		*(this->keybinds + (int)Keybinds::Menu) = this->save.raw["game settings"]["key binds"]["menu toggle"].get<int>();
	}
public:
	~Player() {
		delete[] this->keybinds;
	}
};

#define KEY_BIND(player, Key) KEY player.keybinds[INT Player::Keybinds::Key]

#endif