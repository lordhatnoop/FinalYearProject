#pragma once
#include "stdafx.h"

class GameCharacters {
public:
	int xPosition;
	int yPosition;
	int health;
	int damage;
	
	virtual void createSFML() = 0;
	virtual void createCollisionBox() = 0;
	virtual void update() = 0;
	sf::RectangleShape rectangle;
};