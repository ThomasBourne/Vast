#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>

#include "../entity/3dSprite.hpp"
#include "../str_transformation.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////
///                                      window.hpp                                           ///
///                                      ----------                                           ///
///                                                                                           ///
///                                                                                           ///
///                                                                                           ///
/// window.hpp contains mostly a blanket over SFML (super fast multimedia library) but it's   ///
/// mainly here to allow an easy update of a view (which in itself is a blanket over openGL   ///
///                                                                                           ///
/////////////////////////////////////////////////////////////////////////////////////////////////

namespace nut {
	class Window {
	private:
		sf::RenderWindow win;
		sf::View view;
		sf::Event evnt;
	public:
		Window(std::string name = ""_std, unsigned pWidth = 1920, unsigned pHeight = 1080) {
			this->win.create(sf::VideoMode(pWidth, pHeight), name);
			this->view.setSize(sf::Vector2f(pWidth, pHeight));
			this->win.setView(this->view);
		}
		inline void Clear() {
			this->win.clear();
		}
		inline void Draw(sf::RectangleShape& s) {
			this->win.draw(s);
		}
		inline void Draw(sf::Sprite& s) {
			this->win.draw(s);
		}
		void Draw(Sprite3d& sprite);

		inline void Disp() {
			this->win.display();
		}
		inline bool IsOpen() {
			return this->win.isOpen();
		}
		inline bool PollEvnt() {
			return this->win.pollEvent(this->evnt);
		}
		sf::Event& GetEvnt() {
			return this->evnt;
		}
		inline void Shift(sf::Vector2f amount) {
			this->view.move(amount);
			this->win.setView(this->view);
		}
		inline void Move(sf::Vector2f amount) { this->Shift(amount); }

		void Close() {
			this->win.close();
		}
	};

	typedef Window Win;

	using WinEvnt = sf::Event;

}

using Keyboard = sf::Keyboard;
namespace nut { namespace WindowStyle { using namespace sf::Style; } }

#endif