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

/*
sf::Clock dtClock;
int frameTime;

int main() {
	Window win("Test Window", sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, false);
	sf::CircleShape c(100.f);
	c.setFillColor(sf::Color::Green);
	Player player(sf::Vector2f(50.f, 50.f));
	player.setFillColor(sf::Color::Blue);
	player.move(sf::Vector2f(200.f, 150.f));
	sf::RectangleShape rect(sf::Vector2f(100.f, 50.f));
	rect.setFillColor(sf::Color::Magenta);
	rect.move(sf::Vector2f(600.f, 200.f));
	sf::CircleShape circle(200.f);
	circle.setFillColor(sf::Color::Yellow);
	circle.setOutlineThickness(-15);
	circle.setOutlineColor(sf::Color(255, 99, 200));
	circle.move(sf::Vector2f(1000.f, 1000.f));
	sf::RectangleShape big(sf::Vector2f(6000.f, 3000.f));
	big.setFillColor(sf::Color::Red);
	big.move(sf::Vector2f(900.f, 600.f));
	sf::RectangleShape heart(sf::Vector2f(20, 20));
	heart.setPosition(sf::Vector2f(15, 50));
	heart.setFillColor(sf::Color::Red);

	std::initializer_list<TestRenderObject> objects =
	{
			TRO((void*)&c, TROT::Circle),
			TRO((void*)&rect, TROT::Rectangle),
			TRO((void*)&big, TROT::Rectangle),
			TRO((void*)&circle, TROT::Circle)
	};

	bool freeCamera = false;

	sf::Clock clock;
	float lastTime = 0;

	while (win.IsOpen()) {
		while (win.PollEvnt()) {
			if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::E) {
				if (freeCamera) {
					win.SetCenter(player.getPosition() - sf::Vector2f(player.getSize().x / 2.f, player.getSize().y / 2.f));
					win.ResetRotation();
					win.ResetScale();
				}
				freeCamera = !freeCamera;
			}
			else if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::V) {
				win.toggleVSync();
			}
			else if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::F) {
				win.ToggleFullscreen();
			}
			else if (win.GetEvnt().type == sf::Event::Resized) {
				win.ResetViewRatio();
			}
			else if (win.GetEvnt().type == WinEvnt::Closed) {
				win.Close();
			}
		}

		frameTime = dtClock.restart().asMilliseconds();

		if (freeCamera) {
			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				win.Shift(sf::Vector2f(0, -3 * frameTime));
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down)) {
				win.Shift(sf::Vector2f(0, 3 * frameTime));
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				win.Shift(sf::Vector2f(-3 * frameTime, 0));
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right)) {
				win.Shift(sf::Vector2f(3 * frameTime, 0));
			}
			if (Keyboard::isKeyPressed(Keyboard::Z)) {
				win.Rotate(-0.09f * frameTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::X)) {
				win.Rotate(0.09f * frameTime);
			}
			if (Keyboard::isKeyPressed(Keyboard::Comma)) {
				win.Zoom(1.006f);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Period)) {
				win.Zoom(0.994f);
			}
		}
		else {
			bool grounded = false;
			for (auto& ele : objects) {
				switch (ele.type) {
				case(TROT::Rectangle):
					if (player.getGlobalBounds().intersects(((sf::RectangleShape*)ele.val)->getGlobalBounds())) {
						grounded = true;
					}
					break;
				case(TROT::Circle):
					if (((sf::CircleShape*)(ele.val))->getGlobalBounds().intersects(player.getGlobalBounds())) {
						grounded = true;
					}
					break;
				case(TROT::Sprite):
					if (((sf::Sprite*)(ele.val))->getGlobalBounds().intersects(player.getGlobalBounds())) {
						grounded = true;
					}
					break;
				}
			}
			player.Move(frameTime, grounded);
			win.SetCenter(player.getPosition());
		}
		if (Keyboard::isKeyPressed(Keyboard::LControl) && (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Q))) {
			win.Close();
		}

		float currentTime = clock.restart().asSeconds();
		unsigned short fps = 1 / currentTime;
		lastTime = currentTime;

		win.Clear();
		win.Draw(objects);
		win.Draw(player); //make sure its last of game objects
		win.DrawUI(heart);
		win.Disp(fps);
	}
}
*/
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
	Window win("Test fixed timestep", sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, save.raw["game settings"]["fullscreen"].get<bool>(), save.raw["game settings"]["vsync"].get<bool>(), sf::Color(20, 20, 20));
	sf::Image img{};
	img.LOAD(appImage);
	win.setIcon(img);
	win.setFrmeRateLimit(MAX_RATE); //change this to change the max frame rate
	const float dt = 1.f / MAX_RATE; //change this to change physics rate (should be in sync with framerate)
	float accumulator = 0.f;
	bool drawn = false;
	sf::Clock clock;
	sf::CircleShape c(100.f);
	c.setFillColor(sf::Color::Green);
	Player player(sf::Vector2f(50.f, 50.f), save);
	player.setFillColor(sf::Color::Blue);
	player.move(sf::Vector2f(200.f, 150.f));
	sf::FloatRect nextPos;
	sf::RectangleShape rect(sf::Vector2f(100.f, 50.f));
	rect.setFillColor(sf::Color::Magenta);
	rect.move(sf::Vector2f(600.f, 200.f));
	sf::CircleShape circle(200.f);
	circle.setFillColor(sf::Color::Yellow);
	circle.setOutlineThickness(-15);
	circle.setOutlineColor(sf::Color(255, 99, 200));
	circle.move(sf::Vector2f(1000.f, 1000.f));
	sf::Texture tilemap;
	tilemap.LOAD(tile);
	sf::Sprite larger(tilemap);
	larger.setPosition(sf::Vector2f(-80, -20));
	larger.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(tilemap.getSize().x, tilemap.getSize().y)));
	sf::RectangleShape big(sf::Vector2f(6000.f, 3000.f));
	big.setFillColor(sf::Color::Red);
	big.move(sf::Vector2f(900.f, 600.f));
	sf::RectangleShape heart(sf::Vector2f(20, 20));
	heart.setPosition(sf::Vector2f(15, 50));
	heart.setFillColor(sf::Color::Red);

	std::initializer_list<TestRenderObject> objects =
	{
			TRO((void*)&c, TROT::Circle), //fine to pull mem addr as these are decared in main() and cant rly go out of scope
			TRO((void*)&rect, TROT::Rectangle),
			TRO((void*)&big, TROT::Rectangle),
			TRO((void*)&circle, TROT::Circle),
			TRO((void*)&larger, TROT::Sprite)
	};

	bool freeCamera = false;

	float lastTime = 0;

	bool collided = false;
	bool grounded = false;

	while (win.IsOpen())
	{
		while (win.PollEvnt()) {
			//using if else not switch as keyboard events need multiple checks
			if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::E) {
				if (freeCamera) {
					win.SetCenter(player.getPosition() - sf::Vector2f(player.getSize().x / 2.f, player.getSize().y / 2.f));
					win.ResetRotation();
					win.ResetScale(1920 / win.GetSize().x);
				}
				freeCamera = !freeCamera;
			}
			else if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::V) {
				win.toggleVSync();
			}
			else if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::F) {
				win.ToggleFullscreen();
			}
			else if ((win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::Comma) && freeCamera) {
				win.Rotate(-90.f);
			}
			else if ((win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == Keyboard::Period) && freeCamera) {
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
			if (freeCamera) {
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
				//for (auto& ele : objects) {
					/*switch (ele.type) {
					case(TROT::Rectangle):
						if (player.getGlobalBounds().intersects(((sf::RectangleShape*)ele.val)->getGlobalBounds())) {
							grounded = true;
							player.move(sf::Vector2f(-player.velocity.x, -player.velocity.y));
							player.velocity = sf::Vector2f(0.f, 0.f);
						}
						break;
					case(TROT::Circle):
						if (((sf::CircleShape*)(ele.val))->getGlobalBounds().intersects(player.getGlobalBounds())) {
							grounded = true;
						}
						break;
					case(TROT::Sprite):
						if (((sf::Sprite*)(ele.val))->getGlobalBounds().intersects(player.getGlobalBounds())) {
							grounded = true;
						}
						break;
					}*/
				//}
				player.Move(grounded, collided);

				if(grounded)
					collided = false;

				nextPos = player.getGlobalBounds();
				nextPos.left += player.velocity.x;
				nextPos.top += player.velocity.y;

				for (auto& ele : objects) {
					if (((sf::RectangleShape*)ele.val)->getGlobalBounds().intersects(nextPos)) {
						if ((nextPos.left + nextPos.width > ((sf::RectangleShape*)ele.val)->getGlobalBounds().left) &&
							(nextPos.top + nextPos.height > ((sf::RectangleShape*)ele.val)->getGlobalBounds().top ||
								nextPos.top< ((sf::RectangleShape*)ele.val)->getGlobalBounds().top +
								((sf::RectangleShape*)ele.val)->getGlobalBounds().height)) {
							
							grounded = false;
							player.velocity.x = -(player.velocity.x / 3.f);
						}
						if (nextPos.top < ((sf::RectangleShape*)ele.val)->getGlobalBounds().top +
							((sf::RectangleShape*)ele.val)->getGlobalBounds().height) {
							if (nextPos.top < ((sf::RectangleShape*)ele.val)->getGlobalBounds().top) {
								grounded = true;
								player.setPosition(sf::Vector2f(player.getPosition().x, ((sf::RectangleShape*)ele.val)->getGlobalBounds().top - (player.getGlobalBounds().height + 10.1f)));
							}
							else {
								player.velocity.y = - player.velocity.y;
								grounded = false;
							}
						}
						collided = true;
						break;
					}
					else {
						collided = false;
						grounded = false;
					}
				}

				win.SetCenter(player.getPosition());
			}
			if (Keyboard::isKeyPressed(Keyboard::LControl) && (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Q))) {
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
			win.Draw(objects);
			win.Draw(player); //make sure its last of game objects
			sf::RectangleShape boundBox;
			boundBox.setSize(player.getSize());
			boundBox.setPosition(nextPos.left, nextPos.top);
			boundBox.setFillColor(sf::Color::Transparent);
			boundBox.setOutlineColor(sf::Color::White);
			boundBox.setOutlineThickness(3.f);
			win.Draw(boundBox);
			win.DrawUI(heart);
			win.Disp(fps);

			drawn = true;
		}
	}
}