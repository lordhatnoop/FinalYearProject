#include "HermesBoots.h"
#include "TextureLoader.h"
void HermesBoots::update(PlayerItemStatuses *playerStatus)
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		playerStatus->hermesBootsActive = true; //set true so we can sprint
		//alot of this one is just empty, becasue we're just going to do a check to see if this is the active item, and if so, activate something in player
	}
	else {
		active = false;
		playerStatus->hermesBootsActive = false; //set back false so no longer sprinting
	}
}

void HermesBoots::createSfml()
{
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(3.f, 3.f));
	rectangle.setOrigin(sf::Vector2f(1.5f, 1.5f));
	rectangle.setTexture(&textureLoader.hermesBootTexture);
}

void HermesBoots::createBox2D(b2World & myWorld)
{
	//no body but need this since it inherits from gameItems
}

void HermesBoots::destroy(b2World & myWorld)
{
	//nothing to destroy
}
