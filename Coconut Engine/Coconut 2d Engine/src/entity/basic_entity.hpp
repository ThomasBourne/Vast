#ifndef BASIC_ENTITY_HPP
#define BASIC_ENTITY_HPP

////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                             Basic Entity                                         ///
///                                             ------------                                         ///
///                                                                                                  ///
/// Basic Entity is a is the lowest abstraction level for any entities and is not to be constructed  ///
/// directly, rather to be inhereted by other classes and constructed with that class                ///
/// The enum class Levels is different levels that an entity can be, to be set when constricted with ///
/// the class its being set as                                                                       ///
///                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <SFML/Graphics.hpp>
#include <vector>

#include "3dSprite.hpp"

namespace nut {
	using v2f = sf::Vector2f;
	struct BasicEntity {
	public:
		BasicEntity() { }
		static enum class Levels : char {
			invalidEntity = -1,
			unsetEntity = 0,
			basicEnemy = 1,
			touchableGround = 2,
			basicLiquid = 3,


			//more jucy abstraction
			hiddenEntity = unsetEntity,
			untouchableEntity = hiddenEntity,

			//count keep at end
			count
		};
	protected:
		sf::Sprite collider;
		Sprite3d sprite;

		v2f pos;

		unsigned long health;
		Levels level;
	};

}

#endif