#pragma once
#include "GameItems.h"

class BombItem : public GameItems {
public:
	//constructor
	BombItem() {

		coolDownTimer = 10; // cooldown time
		duration = 3; // short duration to prevent from being too strong 
		durationTimer = new sf::Clock; //start the timer
		itemDamage = 10; // the damage the item does 
		itemName = "BombItem";
	}
	//override the inherited methods
	void update() override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;

	bool exploded = false;
};