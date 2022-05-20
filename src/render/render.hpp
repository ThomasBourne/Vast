#ifndef RENDER_HPP
#define RENDER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

#include "../assets/default_textures.hpp"
#include "../save/savestate.hpp"

#include "chunk.hpp"

//#include "../assets/FiraCode.hpp"

//this is where scale will be 1:1
#define STANDARD_WIDTH 1920

sf::Font firaCode;

enum class RenderObjectType {
	Rectangle = 0,
	Circle,
	Sprite,
	count
};
typedef RenderObjectType TROT;
struct TestRenderObject { //node
	RenderObjectType type;
	void* val;
	TestRenderObject(void* val, RenderObjectType type) { this->val = val; this->type = type; }
};
typedef TestRenderObject TRO;

class Window {
private:
	bool fullscreen;
	std::string title;
	sf::RenderWindow win;
	sf::View view;
	sf::View staticView; //for use with GUI
	sf::View settingsView; //for the menu setting
	sf::Event evnt;
	sf::Color background;

	sf::Text fps;
	std::ostringstream ss;

	bool vsyncEnabled = false;
public:
	Window(std::string name = "", unsigned pWidth = 1920, unsigned pHeight = 1080, bool fullscreen = false, bool vsync = false, sf::Color backgroundColour = sf::Color(135, 206, 235)) {
		this->title = name;
		this->fullscreen = fullscreen;
		this->win.create(sf::VideoMode(pWidth, pHeight), name, (fullscreen ? sf::Style::Fullscreen : sf::Style::Default));
		this->vsyncEnabled = vsync;
		this->win.setVerticalSyncEnabled(this->vsyncEnabled);
		this->view.setSize(sf::Vector2f(pWidth, pHeight));
		this->view.setCenter(pWidth / 2, pHeight / 2);
		this->staticView.setSize(sf::Vector2f(pWidth, pHeight));
		this->staticView.setCenter(pWidth / 2, pHeight / 2);
		this->staticView.setSize(sf::Vector2f(pWidth, pHeight));
		this->staticView.setCenter(pWidth / 2, pHeight / 2);
		this->background = backgroundColour;
		if (!firaCode.LOAD(firaCodeData))
			exit(1);
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
	inline void Draw(sf::RectangleShape& r) {
		this->win.draw(r);
	}
	inline void Draw(sf::CircleShape& c) {
		this->win.draw(c);
	}
	inline void Draw(sf::Sprite& s) {
		this->win.draw(s);
	}
	inline void Draw(std::initializer_list<TestRenderObject>& renderObjects) { //multiple types for debugging
		for (auto& ele : renderObjects) {
			switch (ele.type)
			{
			case(RenderObjectType::Rectangle):
				this->Draw(*((sf::RectangleShape*)ele.val));
				break;
			case(RenderObjectType::Circle):
				this->Draw(*((sf::CircleShape*)ele.val));
				break;
			case(RenderObjectType::Sprite):
				this->Draw(*((sf::Sprite*)ele.val));
				break;
			default:
				break;
			}
		}
	}
	inline void Draw(std::initializer_list<sf::Sprite> renderObjects) {
		for (auto& ele : renderObjects)
			this->win.draw(ele);
	}
	inline void Draw(sf::Text& t) {
		this->win.draw(t);
	}
	inline void Disp(const unsigned short& fpsCount) {

		this->win.setView(this->staticView); //UI elements

		this->ss << fpsCount;

		this->fps.setFont(firaCode);
		this->fps.setString(std::string(this->ss.str()));
		this->fps.setCharacterSize(24);
		this->fps.setFillColor(sf::Color::Green);
		this->fps.setStyle(sf::Text::Bold || sf::Text::Underlined);
		this->win.draw(this->fps);
		this->ss.str(std::string());

		this->win.setView(this->view); //game elements

		this->win.display();
	}
	inline void LightDisp() {
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
	} inline void ResetScale(double amount = 1.0) { this->ResetZoom(); }
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
	void DrawMenu(const sf::View freezeFrame) { //HOW TO GET FREEZE FRAME???
		this->win.clear();
		this->win.setView(freezeFrame);
		this->win.setView(this->settingsView);
		sf::Text settings;
		sf::RectangleShape centreTransparent(this->win.getView().getSize()*0.95f);
		centreTransparent.setPosition(this->win.getView().getSize()*0.025f);
		centreTransparent.setFillColor(sf::Color(255, 255, 255, 50));
		this->win.draw(centreTransparent);
		this->win.display();
	}
	void UpdateWindowValues(ss::SaveState& save) {
		if (save.raw["game settings"]["vsync"].get<bool>() != this->vsyncEnabled)
			this->toggleVSync();
		else if (save.raw["game settings"]["fullscreen"].get<bool>() != this->fullscreen)
			this->ToggleFullscreen();
	}
	inline void Close() {
		this->win.close();
	}
};

typedef Window Win;

using Keyboard = sf::Keyboard;

using WinEvnt = sf::Event;

#endif