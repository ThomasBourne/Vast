#include "3dSprite.hpp"

#include <iostream>

Sprite3d::Sprite3d(const char* jsonSource) {

	this->data = json::parse(jsonSource); //contents of file now stored in json

}