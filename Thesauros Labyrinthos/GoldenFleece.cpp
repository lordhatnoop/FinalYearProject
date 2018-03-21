#include "GoldenFleece.h"

void GoldenFleece::update(PlayerItemStatuses * playerStatus)
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		if (playerStatus->goldenFleeceAlreadyHealed == false) { // if not healed yet
			playerStatus->GoldenFleeceActive = true; //set true so we can heal
			playerStatus->goldenFleeceAlreadyHealed = true; //set true so we don't heal twice
			//alot of this one is just empty, becasue we're just going to do a check to see if this is the active item, and if so, activate something in player

			}
		rectangle.setPosition(playerStatus->position.x + 1.f, playerStatus->position.y + 4.f); //keep updating the position so that the fleece follows along just under the player
	}
	else {
		playerStatus->GoldenFleeceActive = false;
		playerStatus->goldenFleeceAlreadyHealed = false;
		active = false;
	
	}
}

void GoldenFleece::createSfml()
{
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(2.f, 2.f));
	rectangle.setOrigin(sf::Vector2f(1.f, 1.f));
	rectangle.setTexture(&textureLoader.goldenFleece);

	

	
}

void GoldenFleece::createBox2D(b2World & myWorld)
{
	//no collision - no body
}

void GoldenFleece::destroy(b2World & myWorld)
{
	//no bodies to destroy
}
