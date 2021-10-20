#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include <iostream>

#include "../basic_entity.hpp"

namespace nut {
	namespace smoke {
		BasicEntity smoke;
		void BindEntity(BasicEntity smokeInit);
		void MoveSM(v2f amount);
		void MoveSM(sf::Event& evnt);
	}
	namespace sm = smoke; //depreciate for simplicity
}

#endif