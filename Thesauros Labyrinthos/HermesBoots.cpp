#include "HermesBoots.h"
#include "TextureLoader.h"
void HermesBoots::update(PlayerItemStatuses *playerStatus)
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		playerStatus->hermesBootsActive = true; //set true so we can sprint
		//alot of this one is just empty, becasue we're just going to do a check to see if this is the active item, and if so, activate something in player
		//rectangle.setPosition(playerStatus->position.x, playerStatus->position.y + 3.f); //keep updating the position

		//update the texture to be facing the correct way. invert if needed to face left. update position here as well
		if (playerStatus->playerFacingLeftORRight == true) { //facing right
			//the texture faces left, so invert to face right
			textureSubRect.left = 334;
			textureSubRect.width = -334;
			rectangle.setTextureRect(textureSubRect);
			rectangle.setPosition(playerStatus->position.x , playerStatus->position.y + 4.f); //keep updating the position
		}
		else {
			// faces left already so normal texture positions
			textureSubRect.left = 0;
			textureSubRect.width = 334;
			rectangle.setTextureRect(textureSubRect);
			rectangle.setPosition(playerStatus->position.x, playerStatus->position.y + 3.f); //keep updating the position
		}
	}
	else {
		active = false;
		playerStatus->hermesBootsActive = false; //set back false so no longer sprinting
	}
}

void HermesBoots::createSfml()
{
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(2.f, 2.f));
	rectangle.setOrigin(sf::Vector2f(1.f, 1.f));
	rectangle.setTexture(&textureLoader.hermesBootTexture);

	textureSubRect.top = 0;
	textureSubRect.left = 0;
	textureSubRect.width = 334;
	textureSubRect.height = 397;

	rectangle.setTextureRect(textureSubRect);
}

void HermesBoots::createBox2D(b2World & myWorld)
{
	//no body but need this since it inherits from gameItems
}

void HermesBoots::destroy(b2World & myWorld)
{
	//nothing to destroy
}
