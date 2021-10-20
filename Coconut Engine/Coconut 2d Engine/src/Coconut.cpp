#ifndef COCONUT_2D_ENGINE
#define COCONUT_2D_ENGINE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                                                      ///
///                                                           COCONUT 2D ENGINE                                                          ///
///                                                           =================                                                          ///
///                                                                                                                                      ///
/// Welcome to The Coconut 2D Engine: The engine is a game engine with game engine abstraction, but is still mostly a blanket over SFML  ///
/// rendering and uses SFML to do any I/O handled either by things like Win32 api or OpenGL. It is however when using the engine, made   ///
/// that the sf:: namespace is not needed BUT the SFML librarys and dlls are needed (but will be statically linked automatically)        ///
///                                                                                                                                      ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                     Whats currently broken                                                            ///
///
/// Scene

#define SFML_STATIC
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/OpenGL.hpp>
#include <GLFW/glfw3.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
//#include <glad/glad.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define COCONUT_BEGIN namespace nut {
#define COCONUT_END }
#define USE_COCONUT_NAMESPACE using namespace nut;

  /////////////////////
 #define NUT_DEBUG ///
/////////////////////

#define OLD_WINDOW

#include "str_transformation.hpp"

namespace nut {
	namespace ss {
		extern "C" {
#include "save/save.h"
		}
		//function pointers to the SS_ functions, depreciated to the namespace
		SaveState* (*New)(const char*) = &SS_new;
		//SaveState* (*Blank)() = &SS_blank; //depreciated
		void (*Read)(SaveState*) = &SS_read;
		void (*Write)(SaveState*) = &SS_write;
		void (*Delete)(SaveState*) = &SS_delete;
	}
}
#include "entity/3dSprite.hpp"
#include "entity/basic_entity.hpp"
//#include "entity/basic_enemy.hpp"
#include "entity/smoke/player_controller.hpp"
//#include "scene/scene.hpp"
#include "window/window.hpp"
#include "misc/misc.hpp"
#if defined COCONUT_AUDIO
#include "audio/audo.hpp" //guess who dosent exist yet
#endif

#undef COCONUT_BEGIN
#undef COCONUT_END

#ifdef NO_NUT_NAMESPACE
USE_COCONUT_NAMESPACE
#undef USE_COCONUT_NAMESPACE
//#undef NO_NUT_NAMESPACE //this would break if called by multiple files
#endif

void EndNut(int code) {
	exit(code);
}


std::unique_ptr<sf::ContextSettings> settings = std::make_unique<sf::ContextSettings>((24, 8, 4, 4, 5));

sf::RectangleShape r(sf::Vector2f(100.f, 100.f));

int main() {
	r.setFillColor(sf::Color::Green);
	//construction

	//default save path: defaultSavePath
	char* path = new char[_MAX_PATH];
	GetModuleFileNameA(NULL, path, _MAX_PATH);
	std::unique_ptr<std::string> defaultSavePath = std::make_unique<std::string>(path);
	delete[] path;
	*(defaultSavePath) = defaultSavePath->substr(0, defaultSavePath->find_last_of('\\'));
	std::unique_ptr<std::string> folderPath = std::make_unique<std::string>(*defaultSavePath);
	*(defaultSavePath) += "\\vast.save";

	//default save
	auto defaultSave = nut::ss::New(defaultSavePath->c_str());

	//main window

	nut::Window win("Test Window"_std, 1920, 1080);

	//start

	sf::Clock c;

	std::unique_ptr<Sprite3d> cube = std::make_unique<Sprite3d>(cube_gltf);
	Sprite3d gltfCube(cube_gltf);

	win.Draw(gltfCube);

	sf::RectangleShape r(sf::Vector2f(100, 100));
	while (win.IsOpen()) {
		if (win.PollEvnt()) {
			switch (win.GetEvnt().type)
			{
			case(nut::WinEvnt::Closed):
				win.Close();
				break;
			default:
				break;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			win.Shift(sf::Vector2f(0, 3));
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			win.Shift(sf::Vector2f(0, -3));
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			win.Shift(sf::Vector2f(3, 0));
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			win.Shift(sf::Vector2f(-3, 0));
		}
		win.Clear();
		win.Draw(r);
		win.Disp();
		if (c.getElapsedTime().asMilliseconds() >= 2000)
			win.Close();
	}

	*(defaultSave->gameElementsStart + nut::ss::SS_GameContents_Heath) += 2;

	//destruction
	nut::ss::Write(defaultSave);
	nut::ss::Delete(defaultSave);
	std::string endB;
	std::cin >> endB;
	EndNut(0);
}

#endif