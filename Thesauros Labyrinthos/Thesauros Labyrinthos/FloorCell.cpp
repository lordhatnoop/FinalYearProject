#include "FloorCell.h"
#include "TextureLoader.h"

FloorCell::FloorCell(int x, int y)
{
	createSFML(x, y); // call staright to the createSFML function as part of the contructor so that it is done on creation of the cell.
	
	
}

void FloorCell::createSFML(int x, int y)
{
	rectangle.setSize(sf::Vector2f(10.f, 10.f));
	//rectangle.setFillColor(sf::Color::Green); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(x, y); // set the postion of the rectangle to be the position passed
							
	//set the texture, using the extern textureLoader to get the floorTexture
	rectangle.setTexture(&textureLoader.floorTexture, false);
}

void FloorCell::update()
{
}
