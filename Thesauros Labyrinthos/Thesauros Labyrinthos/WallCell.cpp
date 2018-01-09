#include "WallCell.h"



WallCell::WallCell(int x, int y, sf::Texture &texture) {
	createSFML(x, y, texture); // call staright to the createSFML function as part of the contructor so that it is done on creation of the cell.
	createCollisionBox(); // call after the rectangle is created so that collision box can use the sfml sizes.
}

void WallCell::createSFML(int x , int y, sf::Texture &texture)
{
	rectangle.setSize(sf::Vector2f(20.f,20.f)); 
	//rectangle.setFillColor(sf::Color::Green); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(x, y); // set the postion of the rectangle to be the position passed
	//rectangle.setTextureRect(25,25)
	rectangle.setTexture(&texture, false);
}

void WallCell::update()
{
}

void WallCell::createCollisionBox()
{
}
