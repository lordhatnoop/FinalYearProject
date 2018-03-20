#include "HermesHelm.h"

//allow for flight.
//actual mythology says the boots allowed for flight not hermes' helm, but for the game the boots being sprint and the helm being flying seems more thematic
void HermesHelm::update(PlayerItemStatuses * playerStatus)
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		playerStatus->hermesHelmActive = true; //set true so we can fly
		//alot of this one is just empty, becasue we're just going to do a check to see if this is the active item, and if so, activate something in player
		rectangle.setPosition(playerStatus->position); //keep updating the position
	}
	else {
		active = false;
		playerStatus->hermesHelmActive = false; //set back false so no longer sprinting
	}
}

void HermesHelm::createSfml()
{
	rectangle.setPosition(xPosition, yPosition - 3.f);
	rectangle.setSize(sf::Vector2f(2.f, 2.f));
	rectangle.setOrigin(sf::Vector2f(1.f, 1.f));
	rectangle.setTexture(&textureLoader.hermesHelmTexture);
}

void HermesHelm::createBox2D(b2World & myWorld)
{
	//no body
}

void HermesHelm::destroy(b2World & myWorld)
{
	//no body to destroy
}
