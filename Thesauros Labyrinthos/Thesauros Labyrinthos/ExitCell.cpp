#include "ExitCell.h"

ExitCell::ExitCell(int x, int y)
{
	posX = x;
	posY = y;
	createSFML(posX, posY);
}

void ExitCell::createSFML(int x, int y)
{
	rectangle.setSize(sf::Vector2f(10.f, 10.f));
	rectangle.setFillColor(sf::Color::Red); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(x, y); // set the postion of the rectangle to be the position passed
}

void ExitCell::update()
{
	
}

void ExitCell::playerReached(PlayerCharacter player)
{
	if (player.xPosition == posX && player.yPosition == posY) {
		ExitReached = true;
	}
}
