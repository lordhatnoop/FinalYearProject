#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter(int x, int y)
{
	xPosition = x;
	yPosition = y;
	createSFML();
	torch = new TorchLight(xPosition,yPosition);
}

void PlayerCharacter::createSFML()
{
	rectangle.setSize(sf::Vector2f(10.f, 10.f));
	rectangle.setFillColor(sf::Color::Green); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(xPosition, yPosition); // set the postion of the rectangle to be the position passed
	//rectangle.setTextureRect(25,25)
	//rectangle.setTexture(&texture, false);
}

void PlayerCharacter::createCollisionBox()
{
}

void PlayerCharacter::update()
{
	
	//move player left by reducing the xposition and setting the posiiton of the rectangle again.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		xPosition -= 5;
		rectangle.setPosition(xPosition, yPosition);
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		xPosition += 5;
		rectangle.setPosition(xPosition, yPosition);
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		yPosition += 5;
		rectangle.setPosition(xPosition, yPosition);
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		yPosition -= 5;
		rectangle.setPosition(xPosition, yPosition);
	};
	
	torchCountdown();
	//call the torch update so that it updates along with the player
	torch->update(xPosition,yPosition,maxTorchFuel,currentTorchFuel);
}

void PlayerCharacter::torchCountdown()
{
	if (currentTorchFuel > 0) {
		currentTorchFuel -= 0.05; // just minus 0.5 perupdate for a test
	}
}

