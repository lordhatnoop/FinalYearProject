#include "ExitCell.h"

ExitCell::ExitCell(int x, int y)
{
	cellSize = sf::Vector2f(10.f, 10.f); // set the size of the object to be 10 by 10
	cellPosition = sf::Vector2f(x, y);//pos = passed positions

	cellType = "Exit";
	createSFML(); // create the sfml for the exitcell
}

void ExitCell::createSFML()
{
	rectangle.setSize(cellSize);
	rectangle.setFillColor(sf::Color::Red); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(cellPosition); // set the postion of the rectangle to be the position passed

}

void ExitCell::update()
{
	//playerReached(); // call the player reached function which just checks the player's position against the
	//exit position and if they're the same sets the bool to true
}

void ExitCell::playerReached(PlayerCharacter player)
{
	if (player.xPosition == posX && player.yPosition == posY) {
		ExitReached = true;
	}
}
