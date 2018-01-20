#pragma once
#include "stdafx.h"
class LevelManager {

public:
	LevelManager();
	void LoadMenu();
	void LoadNextLevel();
	void DeleteCurrentLevel();
	void update(); // will update all evrything in the currentlevel, will be called in main. cpp while loop
private:
	sf::RenderWindow* window; // pointer to the sf window we will pass

};