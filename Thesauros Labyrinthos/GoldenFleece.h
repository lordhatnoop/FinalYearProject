#pragma once
#include "GameItems.h"
#include "TextureLoader.h"

class GoldenFleece : public GameItems  {
public:
	GoldenFleece() {
		coolDownTimer = 120; // cooldown time
		duration = 5;
		durationTimer = new sf::Clock; //start the timer
		itemDamage = 0; // the damage the item does 
		itemName = "GoldenFleece";
	};

	//override the inherited methods
	void update(PlayerItemStatuses *playerStatus) override;
	void createSfml() override;
	void createBox2D(b2World &myWorld) override;
	void destroy(b2World & myWorld) override;
};