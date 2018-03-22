#pragma once
#include "GameTraps.h"

class ArrowTrap : public GameTraps {
public:
	ArrowTrap(int x, int y) {
		trapName = "ArrowTrap";
		xPosition = x;
		yPosition = y;
		createSfml();
	};

	void update() override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	bool arrowShot = false;

	void createArrowBody();
	void createArrowSfml();
};