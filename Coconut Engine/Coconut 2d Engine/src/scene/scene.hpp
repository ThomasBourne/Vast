#ifndef SCENE_HPP
#define SCENE_HPP


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                               HOW THE SCENE IS BUILT                                                  ///
///                                               ----------------------                                                  ///
///                                                                                                                       ///
/// Map is defined as [using Map = std::array<std:array<int>>;]							                                  ///
/// To acces a cell in the Map access by : [map[y][x]]                                                                    ///
/// A Scene contains a ChunkMap which contains an [std::vector<Chunk>]                                                    ///
/// ChunkData is an [std::array<std::array<unsigned, ChunkSize>, ChunkSize>] where chunksize is a macro currrently [5]    ///
/// Chunk is a class containing a ChunkData                                                                               ///
/// In the Scene constructor, a Map is passed in and split into its relevent amount of chunks in its chunkList            ///
///                                                                                                                       ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <array> //for chunk
#include <vector>

template<typename T, size_t N>
using Array = std::array<T, N>;

#include "map/map.hpp"

COCONUT_BEGIN

#define ChunkSize 5

class Chunk {
public:
	Array<Array<unsigned char, ChunkSize>, ChunkSize> chunk;
	Chunk(Array<Array<unsigned char, ChunkSize>, ChunkSize> data) {
		this->chunk = data;
	}
	~Chunk() { }
};

typedef Array<Array<Chunk, MAP_WIDTH/ChunkSize>, MAP_HEIGHT/ChunkSize> ChunkMap;

class Scene {
private:
	Map typesMap;
	ChunkMap types;
public:
	Scene(/*Map*/int types) {
		//this->typesMap = types;

	}

};

COCONUT_END

#endif
/*
this:
[
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	1, 2, 3, 4, 5, 6, 7, 8, 9
]

becomes:
[
	------------ ------------
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	------------ ------------
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	| 1 2 3 4 5 | 6 7 8 9   |
	------------ ------------
*/