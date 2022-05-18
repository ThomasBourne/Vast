#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

//a chunk will contain CHUNK_SIZE x CHUNK_SIZE sprites
//a map will have MAP_HEIGHT sprites and MAP_HEIGHT will always go into CHUNK_SIZE
//MapData will be the raw data sent into the Map

namespace map {
	enum Data {
		Air = 0,
		Floor1, //topsoil
		Floor2,
		Ground1, //underneath the top layer
		Ground2,
		Veg1,
		Veg2,
		Veg3,
		StairLeft1, //stairs from bottom left to top right
		StairLeft2,
		StairRight1,
		StairRight2,
		count
	};
}

#define CHUNK_SIZE 16

#define MAP_HEIGHT (CHUNK_SIZE * 1)

typedef std::array<std::vector<map::Data>, MAP_HEIGHT> MapData;

typedef std::array<std::array<map::Data , CHUNK_SIZE>, CHUNK_SIZE> DataChunk;
typedef std::array<std::array<sf::Sprite, CHUNK_SIZE>, CHUNK_SIZE> SpriteChunk;

typedef std::array<std::vector<DataChunk  >, MAP_HEIGHT / CHUNK_SIZE> MapDataChunks  ;
typedef std::array<std::vector<SpriteChunk>, MAP_HEIGHT / CHUNK_SIZE> MapSpriteChunks;

class Map {
private:
	MapSpriteChunks* chunks; //heap
public:
	Map(MapData data) {
		unsigned mapLength = data[0].size();
		unsigned char chunkCountHorizontal = mapLength / CHUNK_SIZE;
		unsigned char chunkCountVertical = MAP_HEIGHT / CHUNK_SIZE;
		if (mapLength % CHUNK_SIZE != 0) {
			chunkCountHorizontal++; //add an extra one on the end for the excess tiles
		}
		MapDataChunks* chunkData = new MapDataChunks();

		//transform data into chunkData

		//then retrive textures and create this->chunks
	}
};

namespace map {
	MapData map0 =
	{
		//                   1           2           3           4           5           6           7           8           9           10          11          12          13          14           15          16
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //1
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //2
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //3
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //4
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //5
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //6
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //7
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //8
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //9
		std::vector<Data> { Floor1,     Floor1,     Floor1,     Floor1,     Floor1,     Floor1,     Floor1,     StairRight1,Air,        Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //10
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        StairRight1,Air,        Air,        Air,        Air,        Air,         Air,        Air         }, //11
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        StairRight1,Air,        Air,        Air,        Air,         Air,        Air         }, //12
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        StairRight2,Air,        Air,        Air,         Air,        Air         }, //13
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        StairRight1,Air,        Air,         Air,        Air         }, //14
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        StairRight2,Air,         Air,        Air         }, //15
		std::vector<Data> { Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Air,        Floor2,      Floor1,     Air         }, //16
	};
}

#endif