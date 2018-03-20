#pragma once
#include "GameItems.h"
#include "TextureLoader.h"
class HermesHelm : public GameItems {
public:
	HermesHelm() {

		coolDownTimer = 60; // cooldown time
		duration = 10; 
		durationTimer = new sf::Clock; //start the timer
		itemDamage = 0; // the damage the item does 
		itemName = "HermesHelm";
	}

	//override the inherited methods
	void update(PlayerItemStatuses *playerStatus) override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;
};