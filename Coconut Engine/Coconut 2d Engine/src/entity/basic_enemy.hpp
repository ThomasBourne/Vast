#ifndef BASIC_ENEMY_HPP
#define BASIC_ENEMY_HPP

//////////////////////////////////////////////////////////////////////////////
///                              Basic Entity                              ///
///                              ------------                              ///
///                                                                        ///
/// Basic Enemy is a class inheriting from [BasicEntity] and it can be     ///
/// used in either of 2 ways, either by inheritance and using the          ///
/// Basic Enemy constructor upon creation OR by having all entites that    ///
/// require BasicEntity::Levels::basicEnemy to contain a BasicEnemy object ///
/// and have a Levels refrance linked to the one in the BasicEnemy object  ///
///                                                                        ///
//////////////////////////////////////////////////////////////////////////////


#include "basic_entity.hpp"

COCONUT_BEGIN

class BasicEnemy : public BasicEntity {
public:
	BasicEnemy(sf::Sprite& collider, unsigned long long health);
	BasicEnemy() = default;
};

BasicEnemy::BasicEnemy(sf::Sprite& collider, unsigned long long health) {
	this->level = BasicEntity::Levels::basicEnemy;
	this->collider = collider;
	this->health = health;
}

COCONUT_END

#endif