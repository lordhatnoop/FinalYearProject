#pragma once
#include "GameItems.h"
#include "TextureLoader.h"

class MedusaHead : public GameItems {
public:
	MedusaHead() {

		coolDownTimer = 1; // cooldown time
		duration = 2;
		durationTimer = new sf::Clock; //start the timer
		itemDamage = 2; // the damage the item does 
		itemName = "MedusaHead";

	};

	//override the inherited methods
	void update(PlayerItemStatuses *playerStatus) override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;
	int animationCounter = 0;
};