#pragma once
#include "stdafx.h"
#include "GameCharacters.h"
#include "TextureLoader.h"
#include <iostream>

class Ghost : public GameCharacters {
public:
	Ghost(int x, int y);
	void createSFML();
	void createCollisionBox(b2World &myWorld);
	void Animate();
	void decidePath();
	void update();

	sf::Clock animationTimer;
	int animationCounter = 0;
	sf::Vector2f pathGoal;
	bool dead = false;
};