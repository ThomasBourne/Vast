#ifndef FLOOR_HPP
#define FLOOR_HPP
#include <SFML/Graphics.hpp>
#include <vector>

#include "layers.hpp"
#include "window.hpp"

//need to add what to gen texture
class Floor : public Entity {
public:
	Floor(float width, float height, int xOffset = 0, int yOffset = 0) {
		this->layer = Layers::Ground;
		this->collisionShape = sf::Vector2f(width, height);
		this->collider = sf::RectangleShape(this->collisionShape);
		this->collider.move(sf::Vector2f(xOffset, yOffset));
		this->collider.setFillColor(sf::Color::Green);
	}
	Floor(sf::Vector2f size, int xOffset = 0, int yOffset = 0) {
		this->layer = Layers::Ground;
		this->collisionShape = size;
		this->collider = sf::RectangleShape(this->collisionShape);
		this->collider.move(sf::Vector2f(xOffset, yOffset));
		this->collider.setFillColor(sf::Color::Green);
	}
	Floor() { //not quite sure why but this is needed
		this->layer = Layers::Ground;
		this->collisionShape = TILE;
		this->collider = sf::RectangleShape(this->collisionShape);
		this->collider.move(sf::Vector2f(0, 0));
		this->collider.setFillColor(sf::Color::Green);
	}
};

//example floor entity
//Floor floor(TILE_SIZE, TILE_SIZE, 0, 800);
//window.draw(floor.collider);

constexpr int CHUNK_SIZE{ 5 };

class Chunk { //1 chunk is 5 Floor Entitys
public:
	sf::Vector2f pos; //position of top left point of Chunk
	Floor chunk[CHUNK_SIZE][CHUNK_SIZE]; //array of Floor's
	bool rendered;

	Chunk(sf::Vector2f pos, std::vector<Chunk>& chunksCont) {
		this->pos = pos;
		this->rendered = false;
		for (byte i = 0; i < CHUNK_SIZE; i++) { ///for all uninitialised chunks
			for (byte j = 0; j < CHUNK_SIZE; j++) {
				this->chunk[i][j] = Floor(TILE_SIZE, TILE_SIZE, this->pos.x + (TILE_SIZE * i), this->pos.y + (TILE_SIZE * j));
			}
		}
		chunksCont.push_back(*(this));
	}

	void render() {
		if ((this->pos.x > 0 && this->pos.y > 0) && (this->pos.x + (TILE_SIZE * CHUNK_SIZE) < WINDOW_WIDTH && this->pos.y + (TILE_SIZE * CHUNK_SIZE) < WINDOW_HEIGHT)) {
			if (!rendered)
				rendered = !rendered;
			for (byte i = 0; i < CHUNK_SIZE; i++)
			{
				for (byte j = 0; j < CHUNK_SIZE; j++)
				{
					window.draw(this->chunk[i][j].collider);
				}
			}
		}
		else if (rendered)
			rendered = !rendered;
	}

	// operator += (sf::Vector2f offset) {

	//}
};

std::vector<Chunk> chunks;
#endif