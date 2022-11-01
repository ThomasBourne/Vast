#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <vector>

#include "player/PlayerController.hpp"
#include "render/render.hpp"

#define CLICKED_COLOUR sf::Color(255, 255, 255, 100)
#define NOT_CLICKED_COLOUR sf::Color(255, 255, 255)

#define RESET "reset"

namespace menu {

	extern std::string keys[] = {
        "A",    
        "B",        
        "C",        
        "D",        
        "E",        
        "F",        
        "G",        
        "H",        
        "I",        
        "J",        
        "K",        
        "L",        
        "M",        
        "N",        
        "O",        
        "P",        
        "Q",        
        "R",        
        "S",        
        "T",        
        "U",        
        "V",        
        "W",        
        "X",        
        "Y",        
        "Z",        
        "Num0",     
        "Num1",     
        "Num2",     
        "Num3",     
        "Num4",     
        "Num5",     
        "Num6",     
        "Num7",     
        "Num8",     
        "Num9",     
        "Escape",   
        "LControl", 
        "LShift",   
        "LAlt",     
        "LSystem",  
        "RControl", 
        "RShift",   
        "RAlt",     
        "RSystem",  
        "Menu",     
        "LBracket", 
        "RBracket", 
        "Semicolon",
        "Comma",    
        "Period",   
        "Quote",    
        "Slash",    
        "Backslash",
        "Tilde",    
        "Equal",    
        "Hyphen",   
        "Space",    
        "Enter",    
        "Backspace",
        "Tab",      
        "PageUp",   
        "PageDown", 
        "End",      
        "Home",     
        "Insert",   
        "Delete",   
        "Add",      
        "Subtract", 
        "Multiply", 
        "Divide",   
        "Left",     
        "Right",    
        "Up",       
        "Down",     
        "Numpad0",  
        "Numpad1",  
        "Numpad2",  
        "Numpad3",  
        "Numpad4",  
        "Numpad5",  
        "Numpad6",  
        "Numpad7",  
        "Numpad8",  
        "Numpad9",  
        "F1",       
        "F2",       
        "F3",       
        "F4",       
        "F5",       
        "F6",       
        "F7",       
        "F8",       
        "F9",       
        "F10",      
        "F11",      
        "F12",      
        "F13",      
        "F14",      
        "F15",
	};

    std::vector<std::string> split(std::string str, std::string token) {
        std::vector<std::string> result;
        while (str.size()) {
            int index = str.find(token);
            if (index != std::string::npos) {
                result.push_back(str.substr(0, index));
                str = str.substr(index + token.size());
                if (str.size() == 0)
                    result.push_back(str);
            }
            else {
                result.push_back(str);
                str = "";
            }
        }
        return result;
    }

	void GetText(json&, std::string&);
	std::vector<std::string> GetText(json& j) { //optimise this mess plzzz
		std::string catchStr = "";
        GetText(j, catchStr);
        std::vector<std::string> settings(split(catchStr, "\n"));
		return settings;
	}

	void Menu(Player& p, Window& win) {
        win.getWin().setMouseCursorVisible(true);
        sf::Texture captureTexture;
        captureTexture.create(win.GetSize().x, win.GetSize().y);
        captureTexture.update(win.getWin());
        sf::Sprite capture;
        capture.setTexture(captureTexture);
        capture.setPosition(win.getView().getCenter() - sf::Vector2f(win.getView().getSize().x/2, win.getView().getSize().y/2));
        sf::RectangleShape transparency((sf::Vector2f)win.GetSize());
        transparency.setFillColor(sf::Color(50, 50, 50, 200));
        transparency.setPosition(win.getView().getCenter() - sf::Vector2f(win.getView().getSize().x / 2, win.getView().getSize().y / 2));
        MENU_START:
		win.Clear();
        std::vector<sf::Text> menu;
        sf::Text* lastClicked = nullptr;
        std::vector<std::string> values = GetText(p.save.raw["game settings"]);
        const int fontSize = 30;
        for (auto& ele : values)
            menu.push_back(sf::Text(ele, firaCode));
        float widest = menu[0].getGlobalBounds().width;
        for (auto& ele : menu) {
            if (widest < ele.getGlobalBounds().width)
                widest = ele.getGlobalBounds().width;
        }
        int depth = 0;

        sf::Text reset(RESET, firaCode);
        reset.setCharacterSize(fontSize);
        reset.setStyle(sf::Text::Bold);
        reset.setFillColor(NOT_CLICKED_COLOUR);

		if (menu[0].getString() == "")
			goto LOOP_END;
        for (auto& ele : menu) {
            ele.setCharacterSize(fontSize);
            ele.setStyle(sf::Text::Bold);
            ele.setFillColor(NOT_CLICKED_COLOUR);
            ele.setPosition(sf::Vector2f(win.getView().getCenter().x - widest / 2.f, (win.getView().getCenter().y - (win.getView().getSize().y * 0.4f)) + (depth * fontSize)));
            depth++;
        }
        reset.setPosition(sf::Vector2f(win.getView().getCenter().x - reset.getGlobalBounds().width, menu[menu.size() - 1].getGlobalBounds().height + menu[menu.size() - 1].getPosition().y + 50));

		while (win.IsOpen()) {
			while (win.PollEvnt()) {
				if (win.GetEvnt().type == sf::Event::Resized)
					win.ResetViewRatio();
				else if (win.GetEvnt().type == WinEvnt::Closed)
					win.Close();
                else if (win.GetEvnt().type == sf::Event::MouseButtonReleased) {
                    if (win.GetEvnt().mouseButton.button == sf::Mouse::Left) {
                        for (auto& ele : menu) {
                            if (ele.getGlobalBounds().intersects(sf::FloatRect(win.getWin().mapPixelToCoords(sf::Mouse::getPosition(win.getWin()), win.getView()), sf::Vector2f(1.f, 1.f)))) {
                                if (lastClicked) {
                                    if (ele.getFillColor() == CLICKED_COLOUR) {
                                        ele.setFillColor(NOT_CLICKED_COLOUR);
                                        lastClicked = nullptr;
                                    }
                                    else {
                                        lastClicked->setFillColor(NOT_CLICKED_COLOUR);
                                        ele.setFillColor(CLICKED_COLOUR);
                                        lastClicked = &ele;
                                    }
                                }
                                else {
                                    ele.setFillColor(CLICKED_COLOUR);
                                    lastClicked = &ele;
                                }
                            }
                        }
                        sf::Text*& currentClicked = lastClicked;
                        if (currentClicked) {
                            if (currentClicked->getString().toAnsiString().find("True") != std::string::npos) {
                                currentClicked->setString(currentClicked->getString().toAnsiString().replace(currentClicked->getString().toAnsiString().find("True"), 5, "False"));
                                p.save.raw["game settings"][currentClicked->getString().toAnsiString().substr(0, currentClicked->getString().toAnsiString().find(" :"))] = false;
                            }
                            else if (currentClicked->getString().toAnsiString().find("False") != std::string::npos) {
                                currentClicked->setString(currentClicked->getString().toAnsiString().replace(currentClicked->getString().toAnsiString().find("False"), 5, "True"));
                                p.save.raw["game settings"][currentClicked->getString().toAnsiString().substr(0, currentClicked->getString().toAnsiString().find(" :"))] = true;
                            }
                        }
                        if (reset.getGlobalBounds().intersects(sf::FloatRect(win.getWin().mapPixelToCoords(sf::Mouse::getPosition(win.getWin()), win.getView()), sf::Vector2f(1.f, 1.f)))) {
                            const char* defaultSave = {
#include "save.json"
                            };
                            json newJ = json::parse(std::string(defaultSave));
                            p.save.raw["game settings"] = newJ["game settings"];
                            goto MENU_START;
                        }
                    }
                }
                else if (win.GetEvnt().type == WinEvnt::KeyReleased && lastClicked != nullptr) {
                    if (lastClicked->getString().toAnsiString().find("True") == std::string::npos && lastClicked->getString().toAnsiString().find("False") == std::string::npos) {
                        p.save.raw["game settings"]["key binds"][lastClicked->getString().toAnsiString().substr(0, lastClicked->getString().toAnsiString().find(" :"))] = win.GetEvnt().key.code;
                        lastClicked->setString(lastClicked->getString().toAnsiString().substr(0, lastClicked->getString().toAnsiString().find(" :") + 3) + keys[win.GetEvnt().key.code]);

                        lastClicked->setFillColor(NOT_CLICKED_COLOUR);
                        lastClicked = nullptr;
                    }
                }
                else if (win.GetEvnt().type == WinEvnt::KeyReleased && win.GetEvnt().key.code == p.save.raw["game settings"]["key binds"]["menu toggle"].get<int>()) {
                    goto LOOP_END;
                }
			}
			win.Clear();
            win.getWin().draw(capture);
            win.Draw(transparency);
            for(auto& ele : menu)
			    win.Draw(ele);
            win.Draw(reset);
			win.LightDisp();
		}

		LOOP_END:
		p.save.Save();
        p.SetKeybinds(); 
        win.UpdateWindowValues(p.save);
        win.getWin().setMouseCursorVisible(false);
		return;
	}

	void GetText(json& j, std::string& currentString) {
		for (auto it = j.begin(); it != j.end(); ++it) {
			if (it->is_object())
				GetText(*it, currentString); //probe deeper
			else {
				if(it.value().type() == detail::value_t::string)
					currentString += std::string(it.key()) + " : " + it.value().get<std::string>() + "\n";
				else if (it.value().type() == detail::value_t::number_integer || it.value().type() == detail::value_t::number_unsigned)
                    if(it.value().get<int>() < *(&keys + 1) - keys)
					    currentString += std::string(it.key()) + " : " + keys[it.value().get<int>()] + "\n";
                    else
					    currentString += std::string(it.key()) + " : " + std::to_string(it.value().get<int>()) + "\n";
				else if (it.value().type() == detail::value_t::boolean)
					currentString += std::string(it.key()) + " : " + (it.value().get<bool>() ? "True" : "False") + "\n";
				else {
					std::cout << "ERROR INVALID TOKEN IN MENU\n" << std::endl;
					return;
				}
			}
		}
	}

}

#endif