#ifndef MAP_HPP
#define MAP_HPP

//////////////////////////////////////////////////////////////////////
///                            map.hpp                             ///
///                            -------                             ///
///                                                                ///
/// map.hpp is just a layer of abstraction on-top of std::vector   ///
/// See below for the abstraction                                  ///
///                                                                ///
//////////////////////////////////////////////////////////////////////

#ifndef _ARRAY_
#include <array>
#endif

#define MAP_HEIGHT 100
#define MAP_WIDTH 400

COCONUT_BEGIN

typedef std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT> Map;
typedef std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT> *MapPtr;

COCONUT_END


#endif