#pragma once
#include "GameTraps.h"

class StunTrap : public GameTraps {
public:
	StunTrap(int x, int y) {
		trapName = "StunTrap";
		xPosition = x;
		yPosition = y;
		createSfml();
	};

	void update() override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;

	bool trapCreated = false;
	sf::Clock durationTimer;
	void setTriggered() override;
};