#pragma once
#include "GameItems.h"
class RopeItem :public GameItems {

public:
	//constructor
	RopeItem() {
		
		coolDownTimer = 20;
		duration = 10;
		durationTimer = new sf::Clock;
	}
	//override the inherited methods
	void update() override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;

};