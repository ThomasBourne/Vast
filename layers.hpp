#ifndef LAYERS_HPP
#define LAYERS_HPP
#include <SFML/Graphics.hpp>
#include "window.hpp"

enum class Layers : byte {
	Empty = 0,
	Player = 1,
	Ground = 2,
	Shrubs = 3,
	NPC = 4,
	Enemy = 5,
	cloud = 6,


	count //keep at end
};

struct Entity {
	//base class for any entity minus the player
	sf::RectangleShape collider;
	sf::Vector2f collisionShape;
	Layers layer;
};
#endif