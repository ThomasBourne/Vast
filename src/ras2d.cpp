#define SFML_STATIC
//#define USE_LOCAL_TEXTURES

#include <iostream>

#include <SFML/Graphics.hpp>
#include <vector>

#include <fstream>
#include <iomanip>

#include <string>
#include <Windows.h>

#include "render/render.hpp"
#include "packet_WEB/packet.hpp"
#include "player/PlayerController.hpp"
#include "save/savestate.hpp"
#include "assets/default_textures.hpp"
#include "menu.hpp"

sf::Clock dtClock;
sf::Clock otherClock;
int frameTime;

#define MAX_RATE 200

std::string GetDir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return (std::string(buffer)).substr(0, (std::string(buffer)).find_last_of("\\/"));
}
bool FileExists(const char* path) {
	return static_cast<bool>(std::ifstream(path));
}

int main() {
	if (!FileExists(std::string(GetDir() + "\\settings.save").c_str())) {
		const char* defaultSave = {
#include "save.json"
		};
		std::ofstream save;
		save.open(GetDir() + "\\settings.save");
		save << defaultSave;
		save.close();
	}
	ss::SaveState save(GetDir() + "\\settings.save");
	Window win("Test fixed timestep", sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, save.raw["game settings"]["fullscreen"].get<bool>(), save.raw["game settings"]["vsync"].get<bool>(), save.raw["game settings"]["show fps"].get<bool>(), sf::Color(20, 20, 20));
	win.getWin().setMouseCursorVisible(false);
	sf::Image img{};
	img.LOAD(Asset::appImage);
	win.setIcon(img);
	win.setFrmeRateLimit(MAX_RATE); //change this to change the max frame rate
	const float dt = 1.f / MAX_RATE; //change this to change physics rate (should be in sync with framerate)
	float accumulator = 0.f;
	bool drawn = false;
	sf::Clock clock;
	Player player(sf::Vector2f(50.f, 50.f), save);
	player.setFillColor(sf::Color::Blue);
	player.move(sf::Vector2f(200.f, 150.f));
	sf::FloatRect nextPos;
	sf::RectangleShape heart(sf::Vector2f(20, 20));
	heart.setPosition(sf::Vector2f(15, 50));
	heart.setFillColor(sf::Color::Red);
	TileMap map;
	map.load(Asset::tileMapData, sf::Vector2u(32, 32), level, 16, 8);


	bool freeCamera = false;

	float lastTime = 0;

	bool collided = false;
	bool grounded = false;

	while (win.IsOpen())
	{
		while (win.PollEvnt()) {
			//using if else not switch as keyboard events need multiple checks
			if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == KEY_BIND(player, FreeCamera)) {
				if (freeCamera) {
					win.SetCenter(player.getPosition() - sf::Vector2f(player.getSize().x / 2.f, player.getSize().y / 2.f));
					win.ResetRotation();
					win.ResetScale();
				}
				freeCamera = !freeCamera;
			}
			else if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == KEY_BIND(player, Minimap)) {
				json& toggleMinimap = player.save.raw["game settings"]["minimap"];

				toggleMinimap = !(toggleMinimap.get<bool>());
				player.save.Save();
			}
			else if ((win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == KEY_BIND(player, RotateLeft)) && freeCamera) {
				win.Rotate(-90.f);
			}
			else if ((win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == KEY_BIND(player, RotateRight)) && freeCamera) {
				win.Rotate(90.f);
			}
			else if (win.GetEvnt().type == sf::Event::Resized) {
				win.ResetViewRatio();
			}
			else if (win.GetEvnt().type == WinEvnt::Closed) {
				win.Close();
			}
			else if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == KEY_BIND(player, Menu)) {
				win.DrawMenu(win.getView());
				menu::Menu(player, win);
			}
		}

		frameTime = dtClock.restart().asMilliseconds();

		accumulator += clock.getElapsedTime().asSeconds();
		clock.restart();

		while (accumulator >= dt)
		{
			// Physics and gameplay updates.
			if (freeCamera) { //window shift does not speed up/slow down as player zooms in or out
				if (Keyboard::isKeyPressed(KEY_BIND(player, CameraUp))) {
					win.Shift(sf::Vector2f(0, -3));
				}
				else if (Keyboard::isKeyPressed(KEY_BIND(player, CameraDown))) {
					win.Shift(sf::Vector2f(0, 3));
				}
				if (Keyboard::isKeyPressed(KEY_BIND(player, CameraLeft))) {
					win.Shift(sf::Vector2f(-3, 0));
				}
				else if (Keyboard::isKeyPressed(KEY_BIND(player, CameraRight))) {
					win.Shift(sf::Vector2f(3, 0));
				}
				if (Keyboard::isKeyPressed(KEY_BIND(player, ZoomIn))) {
					win.Zoom(1.006f);
				}
				else if (Keyboard::isKeyPressed(KEY_BIND(player, ZoomOut))) {
					win.Zoom(0.994f);
				}
			}
			else {
				auto colliders = TileMap::GenetateColliderPoints(map.GetCloseColliders(player), map); //atm only calls, this needs to be caught and passed into Player::Move() to be useable
				player.Move(grounded, collided, colliders);

				if(grounded)
					collided = false;

				nextPos = player.getGlobalBounds();
				nextPos.left += player.velocity.x;
				nextPos.top += player.velocity.y;

				win.SetCenter(player.getPosition() + sf::Vector2f(player.getSize().x / 2.f, player.getSize().y / 2.f));
			}
			if (Keyboard::isKeyPressed(Keyboard::LControl) && (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Q))) { //these can be hard binded
				win.Close();
			}

			accumulator -= dt;
			drawn = false;
		}

		if (drawn)
		{
			sf::sleep(sf::microseconds(1));
		}
		else
		{
			float currentTime = otherClock.restart().asSeconds();
			unsigned short fps = 1 / currentTime;
			lastTime = currentTime;

			// Draw everything.
			win.Clear();
			win.getWin().draw(map);
			win.Draw(player); //make sure its last of game objects
			/*sf::RectangleShape boundBox;
			boundBox.setSize(player.getSize());
			boundBox.setPosition(nextPos.left, nextPos.top);
			boundBox.setFillColor(sf::Color::Transparent);
			boundBox.setOutlineColor(sf::Color::White);
			boundBox.setOutlineThickness(3.f);
			win.Draw(boundBox);*/ //just for debugging
			win.DrawUI(heart);
			win.Disp(fps, map, player);

			drawn = true;
		}
	}
}