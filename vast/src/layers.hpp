#ifndef LAYERS_HPP
#define LAYERS_HPP
#include <SFML/Graphics.hpp>
#include "window.hpp"

enum class Layers : byte {
	Player = 0,
	Ground = 1,
	NPC = 2,
	Enemy = 3,
	cloud = 4,


	count //keep at end
};

struct Entity {
	//base class for any entity minus the player
	sf::RectangleShape collider;
	sf::Vector2f collisionShape;
	Layers layer;
};
#endif