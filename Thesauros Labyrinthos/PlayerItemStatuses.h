#pragma once
#include "stdafx.h"
class PlayerItemStatuses {
public:
	PlayerItemStatuses() {
	};
	bool aegisShieldAquired = false;
	bool hermesBootsActive = false;
	bool hermesHelmActive = false;
	sf::Vector2f position; //hold the player position for the items
};