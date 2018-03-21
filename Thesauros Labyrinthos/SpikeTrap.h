#pragma once
#include "GameTraps.h"

class SpikeTrap : public GameTraps {
public:
	SpikeTrap(int x, int y) {
		trapName = "SpikeTrap";
		xPosition = x;
		yPosition = y;
		createSfml();
	};

	void update() override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	
};