#pragma once
#include "stdafx.h"
//act same as textureloader. load all sounds in one place that can be accessed from any file
class SoundManager {

public:

	SoundManager();
	void loadSounds();

	sf::SoundBuffer medusaBuffer;
	sf::Sound medusaAttack;
	sf::SoundBuffer skeletonBuffer;
	sf::Sound skeletonSound;
	sf::Music mainMenuMusic;

};
extern SoundManager soundManager;
