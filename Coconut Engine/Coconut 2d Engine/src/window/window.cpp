#include "window.hpp"

#include <iostream>

#include "../entity/json.hpp"

void nut::Window::Draw(Sprite3d& sprite) {
	sprite.GetJson();
	//std::cout << sprite.GetJson().get<std::string>() << std::endl;
}