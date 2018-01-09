#pragma once
#include "GameCharacters.h"

class PlayerCharacter : public GameCharacters {

public:
	PlayerCharacter(int x, int y);
	void createSFML();
	void createCollisionBox();
	void update();


};