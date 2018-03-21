#pragma once
#include "stdafx.h"
class PlayerItemStatuses {
public:
	PlayerItemStatuses() {
		
	};
	bool aegisShieldAquired = false;
	bool hermesBootsActive = false;
	bool hermesHelmActive = false;
	bool playerFacingLeftORRight = false; //left = false, right = true. hold this so we can invert some of the textures
	bool GoldenFleeceActive = false;
	bool goldenFleeceAlreadyHealed = false;
	sf::Vector2f position; //hold the player position for the items
};