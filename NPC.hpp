#ifndef NPC_HPP
#define NPC_HPP
#include "layers.hpp"

class NPC : public Entity {
public:
	uint health;
	NPC() {
		this->layer = Layers::NPC;
	}
};
#endif
