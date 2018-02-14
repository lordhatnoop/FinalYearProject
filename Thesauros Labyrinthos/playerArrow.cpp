#include "playerArrow.h"
#include "TextureLoader.h"
playerArrow::playerArrow(int x, int y, bool playerDirection)
{
	xPosition = x;
	yPosition = y;
	direction = playerDirection;
	createSFML(); // create the sfml on arrow creation
}

void playerArrow::createSFML()
{
	if (direction == false) {// if the direction is left
		arrowRect.setPosition(sf::Vector2f(xPosition - 1, yPosition)); // set the arrow position to be slightly to the left of the player when created

		//set the texture to start from the right and use negative width and hie=ght to get texture flipped to face left
		arrowTextureRect.left = 1030; //998
		arrowTextureRect.width = -32; 
		arrowTextureRect.top = 347;
		arrowTextureRect.height = -5;
	}
	else if (direction == true) {//else if directiopn is right
		arrowRect.setPosition(sf::Vector2f(xPosition + 1, yPosition)); // set the arrow position to be slightly to the Right of the player when created

		//set texture sheet position
		arrowTextureRect.left = 998; 
		arrowTextureRect.width = 32;
		arrowTextureRect.top = 342;
		arrowTextureRect.height = 5;
	}

	arrowRect.setSize(sf::Vector2f(2.f, 1.f)); //set size, small cause arrow
	arrowRect.setOrigin(sf::Vector2f(1.f, 0.5f));
	arrowRect.setTexture(&textureLoader.playerTexture); //set texture to be the player texture from the loader. has arrows on it
	arrowRect.setTextureRect(arrowTextureRect); //tell it to use the setup textureRect sprite sheet positions
}

