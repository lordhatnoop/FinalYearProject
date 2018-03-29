#pragma once
#include "stdafx.h"
//act same as textureloader. load all sounds in one place that can be accessed from any file
class SoundManager {

public:

	SoundManager();
	void loadSounds();
	void updateSoundVolume();
	//enemies
	sf::SoundBuffer medusaBuffer;
	sf::Sound medusaAttack;
	sf::SoundBuffer skeletonBuffer;
	sf::Sound skeletonSound;
	sf::SoundBuffer ghostBuffer;
	sf::Sound ghostsound;
	//menu
	sf::Music mainMenuMusic;
	sf::SoundBuffer menuClickBuffer;
	sf::Sound menuClick;

	//chest sounds
	sf::SoundBuffer chestOpenBuffer;
	sf::Sound chestOpen;


	float soundVolume = 100.f;
	bool muted = false;
};
extern SoundManager soundManager;
