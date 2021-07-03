#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <iostream>
#include <Shlwapi.h>

#include "save.hpp"

typedef unsigned int uint;
typedef unsigned char byte;


constexpr int WINDOW_WIDTH{ 1920 };
constexpr int WINDOW_HEIGHT{ 1080 };


sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Vast -Agenda"/*, sf::Style::Fullscreen*/); //something has been set up wrong and sf::Style::Fullscreen slows game down alot


constexpr char TILE_SIZE{ 64 };
const sf::Vector2f TILE(TILE_SIZE, TILE_SIZE);

constexpr uint TILE_SHEET_SIZE{ 192 };
const sf::Vector2i TILE_SHEET(TILE_SHEET_SIZE, TILE_SHEET_SIZE);


std::string exePath() {
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	/*
	strrchr: function function: find the position of the last occurrence of a character c in another string str (that is, find the position of the first occurrence of the character c from the right side of str),
	 And return the address of this location. If the specified character cannot be found, the function returns NULL.
	 Use this address to return the string from the last character c to the end of str.
	*/
	(strrchr(szFilePath, '\\'))[0] = 0; // Delete the file name, only get the path string //
	strcat(szFilePath, "\\_VastSave.save");
	std::string path = szFilePath;
	return path;

}
const std::string defaultSavePath = exePath();

void CloseVast(int code) {
    window.close();
    exit(code);
}

save::SaveState defaultStartSaveState = *save::NewState(0, 10, 10, 5, defaultSavePath.c_str());
save::SaveState* defaultSave = &defaultStartSaveState;
#endif