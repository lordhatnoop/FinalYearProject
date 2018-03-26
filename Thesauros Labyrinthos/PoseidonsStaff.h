#pragma once
#include "GameItems.h"

class PoseidonsStaff : public GameItems {
public:

	//constructor
	PoseidonsStaff() {

		coolDownTimer = 40; // cooldown time
		duration = 5; // short duration to prevent from being too strong 
		durationTimer = new sf::Clock;
		itemDamage = 1; // the damage the item does - not too much as this item is meant more for mix of damage and protection
		itemName = "PoseidonsStaff";
		animationTimer.restart();
	}
	//override the inherited methods
	void update(PlayerItemStatuses *playerStatus) override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;

	int animationCounter = 0; //counter for updating current frame of animation
	bool velocitySet = false;
	sf::Clock animationTimer;

};