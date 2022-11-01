#ifndef HOMEPAGE_HPP
#define HOMEPAGE_HPP

#include "render/render.hpp"
#include "packet_WEB/packet.hpp"
#include "save/json.hpp"
#include <thread>
#include <SFML/Graphics.hpp>
#include <string>
#include <array>

std::string GetDir();

namespace homepage {
	enum class HomePageAction {
		Exit = 0,
		Play = 1,
        Unused,
		count //more options to be added later

	};
	HomePageAction Run(Window& win, ss::SaveState& save, Player& player) {
        sf::Texture background;
        //CHANGE TO BE ABLE TO LOAD FROM MEM
        background.loadFromFile(GetDir() + "\\landingimage.png");
        sf::Sprite picture(background);
        picture.setPosition(sf::Vector2f(0, 0));
        picture.setScale(sf::Vector2f(win.getView().getSize().x / picture.getGlobalBounds().width, win.getView().getSize().y / picture.getGlobalBounds().height));

        //define all the buttons
        std::array<std::string, 5> text{ { "Play", "reset", "Settings", "Store", "Exit"} };
        std::array<sf::Text, 5> buttons;
        for (int i = 0; i < text.size(); i++) {
            sf::Text currentText(text[i], firaCode);
            currentText.setPosition(sf::Vector2f((70 * picture.getScale().x) - currentText.getGlobalBounds().width / 2, ((30 * picture.getScale().y) * i) + (60 * picture.getScale().y)));
            buttons[i] = currentText;
        }

        while (win.IsOpen())
        {
            sf::Event evnt;
            while (win.getWin().pollEvent(evnt))
            {
                
                if (evnt.type == sf::Event::Closed)
                    win.Close();
                else if (evnt.type == sf::Event::Resized)
                        win.ResetViewRatio();
                else if (evnt.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < buttons.size(); i++) {
                        if (buttons[i].getGlobalBounds().intersects(sf::FloatRect(win.getWin().mapPixelToCoords(sf::Mouse::getPosition(win.getWin()), win.getView()), sf::Vector2f(1.f, 1.f)))) {
                            switch (i) {
                            case 0:
                                return HomePageAction::Play;
                            case 1: {
                                const char*  defaultSave = {
#include "save.json"
                                };
                                json newSave = json::parse(std::string(defaultSave));
                                save.Update(newSave);
                                player.SetSave(save);
                                player.save.Write();
                                return HomePageAction::Play;
                            }
                            case 2: {
                                menu::Menu(player, win);
                                win.getWin().setMouseCursorVisible(true); //auto setes to invisible, so override
                                break;
                            }
                            case 3: {
                                std::thread webBrowser(packet::CreateWebWindow);
                                webBrowser.join();
                            }
                            case 4:
                                return HomePageAction::Exit;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
            sf::RectangleShape shape(sf::Vector2f(10, 10));
            shape.setPosition(sf::Vector2f(100, 100));
            win.getWin().clear(sf::Color(0,0,0));
            win.Draw(picture);
            for (int i = 0; i < text.size(); i++) {
                win.Draw(buttons[i]);
            }
            win.getWin().display();
        }
		return HomePageAction::Unused;
	}
}

#endif