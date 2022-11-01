#ifndef RENDER_HPP
#define RENDER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

#include "../assets/default_textures.hpp"
#include "../save/savestate.hpp"

#include "../player/PlayerController.hpp"

#include "level.hpp"


//THIS NEEDS TO BE IMPLEMENTED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//this is where scale will be 1:1
#define STANDARD_WIDTH 1920

#define MINIMAP_ZOOM 2.2f

sf::Font firaCode;

class Window {
private:
	bool fullscreen;
	std::string title;
	sf::RenderWindow win;
	sf::View view;
	sf::View staticView; //for use with GUI
	sf::View settingsView; //for the menu setting
	sf::View miniMapView; //for the mini map
	sf::Event evnt;
	sf::Color background;

	sf::Text fps;
	std::ostringstream ss;

	bool vsyncEnabled = false;
	bool showFps = true;
public:
	Window(std::string name = "", unsigned pWidth = 1920, unsigned pHeight = 1080, bool fullscreen = false, bool vsync = false, bool showFPS = false, sf::Color backgroundColour = sf::Color(135, 206, 235)) {
		this->title = name;
		this->fullscreen = fullscreen;
		this->showFps = showFPS;
		this->win.create(sf::VideoMode(pWidth, pHeight), name, (fullscreen ? sf::Style::Fullscreen : sf::Style::Default));
		this->vsyncEnabled = vsync;
		this->win.setVerticalSyncEnabled(this->vsyncEnabled);
		this->view.setSize(sf::Vector2f(pWidth, pHeight));
		this->view.setCenter(pWidth / 2, pHeight / 2);
		this->staticView.setSize(sf::Vector2f(pWidth, pHeight));
		this->staticView.setCenter(pWidth / 2, pHeight / 2);
		this->staticView.setSize(sf::Vector2f(pWidth, pHeight));
		this->staticView.setCenter(pWidth / 2, pHeight / 2);
		this->miniMapView.setSize(sf::Vector2f(this->view.getSize().x * MINIMAP_ZOOM, this->view.getSize().y * MINIMAP_ZOOM)); //SETS VIEW OF MINI MAP
		this->miniMapView.setViewport(sf::FloatRect(0.79f, 0.01f, 0.2f, 0.2f));
		this->background = backgroundColour;
		firaCode.LOAD(Asset::firaCodeData);
	}
	inline sf::RenderWindow& getWin() {
		return this->win;
	}
	inline void toggleVSync() {
		this->vsyncEnabled = !this->vsyncEnabled;
		this->win.setVerticalSyncEnabled(this->vsyncEnabled);
	}
	inline void setFrmeRateLimit(unsigned int limit) {
		this->win.setFramerateLimit(limit);
	}
	inline void Clear() {
		this->win.clear(background);
		this->win.setView(this->view);
	}
	inline void Draw(sf::Sprite& s) {
		//this->win.setView(this->view);
		this->win.draw(s);
	}
	inline void Draw(sf::RectangleShape& r) { this->Draw((sf::Sprite&)r); }
	inline void Draw(sf::Text& t) {
		this->win.draw(t);
	}
	inline void Disp(const unsigned short& fpsCount, TileMap& tilemap, Player& player) {

		if (this->showFps) {
			this->win.setView(this->staticView); //UI elements

			this->ss << fpsCount;

			this->fps.setFont(firaCode);
			this->fps.setString(std::string(this->ss.str()));
			this->fps.setCharacterSize(24);
			this->fps.setFillColor(sf::Color::Green);
			this->fps.setStyle(sf::Text::Bold || sf::Text::Underlined);
			this->win.draw(this->fps);
			this->ss.str(std::string());
		}

		//this->win.setView(this->view); //game elements

		if (player.save.raw["game settings"]["minimap"].get<bool>()) {
			this->miniMapView.setCenter(this->view.getCenter());
			this->win.setView(this->miniMapView);
			sf::RectangleShape outline(sf::Vector2f((this->miniMapView.getSize().x - 50.f), this->miniMapView.getSize().y - 50.f));
			outline.setFillColor(sf::Color::Transparent);
			outline.setOutlineColor(sf::Color::White);
			outline.setOutlineThickness(50.f * (this->miniMapView.getSize().x / (float)this->win.getSize().x));
			outline.setPosition(sf::Vector2f(this->miniMapView.getCenter().x - (outline.getSize().x / 2.f), this->miniMapView.getCenter().y - (outline.getSize().y / 2.f)));
			sf::RectangleShape background(this->miniMapView.getSize());
			background.setPosition(sf::Vector2f(this->miniMapView.getCenter().x - (this->miniMapView.getSize().x / 2.f), this->miniMapView.getCenter().y - (this->miniMapView.getSize().y / 2.f)));
			background.setFillColor(sf::Color(255, 255, 255, 100));
			this->win.draw(background);
			this->win.draw(tilemap);
			this->win.draw(player);
			this->win.draw(outline);
		}

		this->win.setView(this->view);

		this->win.display();
	}
	inline void LightDisp() { //need to figure out how to remove this because it is BAD programming... shame on you Tom
		this->win.setView(this->staticView);
		this->win.display();
	}
	inline bool IsOpen() {
		return this->win.isOpen();
	}
	inline bool PollEvnt() {
		return this->win.pollEvent(this->evnt);
	}
	inline sf::Event& GetEvnt() {
		return this->evnt;
	}
	inline sf::Vector2u GetSize() {
		return this->win.getSize();
	}
	inline sf::Vector2f GetPosition() {
		return sf::Vector2f(this->win.getPosition().x, this->win.getPosition().y);
	}
	inline void Shift(sf::Vector2f amount) {
		this->view.move(amount);
	} inline void Move(sf::Vector2f amount) { this->Shift(amount); }
	inline void SetCenter(sf::Vector2f position) {
		this->view.setCenter(position);
	}
	inline void Rotate(float amount) {
		this->view.rotate(amount);
	}
	inline void Zoom(float amount) {
		this->view.zoom(amount);
	}
	inline void ResetRotation() {
		this->view.setRotation(0);
	}
	inline void ResetZoom(double amount = 1.0) {
		this->view.setSize(sf::Vector2f(this->win.getSize().x * amount, this->win.getSize().y * amount));
	} inline void ResetScale(double amount = 1.0) { this->ResetZoom(amount); }
	inline void ResetViewRatio() {
		//make sure scale is 1:1
		/*//make sure that 1920 = 1
		//1920 * 2 = 2
		float scaleRatio = this->win.getSize().x / STANDARD_WIDTH;
		this->view.setSize(sf::Vector2f(this->win.getSize()) * scaleRatio);
		this->view.setCenter(sf::Vector2f(this->win.getPosition() - sf::Vector2i(this->win.getSize().x / 2.f, this->win.getSize().y / 2.f)));
		*/
		this->ResetZoom();
	}
	inline void DrawUI(sf::Sprite ele) {
		this->win.setView(this->staticView);
		this->win.draw(ele);
	}inline void DrawUI(sf::RectangleShape ele) {
		this->win.setView(this->staticView);
		this->win.draw(ele);
	}
	inline void ToggleFullscreen() {
		this->fullscreen = !this->fullscreen;
		this->win.create(sf::VideoMode(this->win.getSize().x, this->win.getSize().y), this->title, (this->fullscreen ? sf::Style::Fullscreen : sf::Style::Default));
		this->win.setVerticalSyncEnabled(this->vsyncEnabled);
	}
	inline void setIcon(const sf::Image icon) {
		this->win.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	inline sf::View getView() {
		return this->view;
	}
	void UpdateWindowValues(ss::SaveState& save) {
		if (save.raw["game settings"]["vsync"].get<bool>() != this->vsyncEnabled)
			this->toggleVSync();
		else if (save.raw["game settings"]["fullscreen"].get<bool>() != this->fullscreen)
			this->ToggleFullscreen();
		else if (save.raw["game settings"]["show fps"].get<bool>() != this->showFps)
			this->showFps = !showFps;
	}
	inline void Close() {
		this->win.close();
	}
};

typedef Window Win;

using Keyboard = sf::Keyboard;

using WinEvnt = sf::Event;

#endif