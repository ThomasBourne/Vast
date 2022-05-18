#ifndef SAVESTATEHPP
#define SAVESTATEHPP

#include "json.hpp"
#include <string>
#include <vector>

#define CURRENT_MAJOR 0
#define CURRENT_MINOR 0

////////////////////////////////////////////////////////////////////////////////////////////
// FILE PEEP                                                                              //
// {                                                                                      //
//		"major" : <major version : int>                                                   //
//		"minor" : <minor version : int>                                                   //
//		"game values" : {                                                                 //
//			"health" : <current amount of health : int>,                                  //
//			"max health" : <max achivable health : int>                                   //
//		},                                                                                //
//		"game settings": {                                                                //
//			"vsync" : <vsync toggle : bool>,                                              //
//			"fullscreen" : <fullscreen toggle : bool>                                     //
//			"key rebinds" : {                                                             //
//				"<key rebinded>": <key rebinded to : int (from sf::Keyboard enum)>        //
//			    ...                                                                       //
//			}                                                                             //
//		}                                                                                 //
//}                                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////

namespace ss {
	struct SaveState {
		std::string file;

		json raw;
		std::string err = "";
		unsigned char major;
		unsigned char minor;

		SaveState(std::string);
		SaveState() = default;
		void Write();
		inline void Save() { Write(); }
		void Update(json&);

		static SaveState Read(std::string);
		static void MatchCurrentSave(SaveState&);
	};
}

#endif