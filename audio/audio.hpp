#ifndef AUDIO_HPP
#define AUDIO_HPP
/*
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>


#define MAX_SOUNDS 8

//this isnt best practice but shows effects are global and while static does mean that it clears it up
#define GLOBAL static

template <typename T>
void remove(std::vector<T>& vec, size_t pos)
{
	std::vector<T>::iterator it = vec.begin();
	std::advance(it, pos);
	vec.erase(it);
}

class Audio {
private:
	GLOBAL std::vector<sf::Sound> sounds;
	GLOBAL unsigned char soundCount;

public:
	void Test() {
		if (sounds[1].getStatus() == sf::SoundSource::Stopped) {
			remove(sounds, 1);
		}
	}

	class Sound {
	private:
		sf::Sound sound;
		const bool loop;
	public:
		Sound(const char* filePath, bool loopable) : loop(loopable) {
			sf::SoundBuffer tmpBuf;
			if (!tmpBuf.loadFromFile(filePath)) {
				std::cerr << "File: " << filePath << '\t' << "failed to load...." << std::endl;
				exit(2);
			}
		}
	};
};

#undef GLOBAL
*/
#endif