#pragma once
#include "GameItems.h"

class HermesBoots : public GameItems {
public:
	HermesBoots() {

		coolDownTimer = 30; // cooldown time
		duration = 10; // short duration to prevent from being too strong 
		durationTimer = new sf::Clock; //start the timer
		itemDamage = 0; // the damage the item does 
		itemName = "HermesBoots";
	}

	//override the inherited methods
	void update(PlayerItemStatuses *playerStatus) override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;
};