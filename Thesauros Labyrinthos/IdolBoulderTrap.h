#pragma once
#include "GameTraps.h"

class IdolBoudlerTrap : public GameTraps {
public:
	IdolBoudlerTrap(int x, int y) {
		trapName = "IdolBoudlerTrap";
		xPosition = x;
		yPosition = y;
		createSfml();
	};

	void update() override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void setTriggered() override { trapTriggered = true; };

	b2CircleShape circleShape;

	
	void createBoulderBody();
	void createBoulderSfml();
	bool boulderCreated = false;
};