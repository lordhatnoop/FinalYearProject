#pragma once
#include "GameItems.h"

class FlameCloakItem : public GameItems {
public:

	//constructor
	FlameCloakItem() {

		coolDownTimer = 20; // cooldown time
		duration = 5; // short duration to prevent from being too strong 
		durationTimer = new sf::Clock;
		itemDamage = 1; // the damage the item does
	}
	//override the inherited methods
	void update() override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;

	int animationCounter = 0; //counter for updating current frame of animation



};