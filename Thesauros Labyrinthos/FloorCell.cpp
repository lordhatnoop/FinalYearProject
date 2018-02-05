#include "FloorCell.h"
#include "TextureLoader.h"

FloorCell::FloorCell(int x, int y)
{
	cellSize = sf::Vector2f(10.f, 10.f); // set the size of the object to be 10 by 10
	cellPosition = sf::Vector2f(x, y);//pos = passed positions

	createSFML(); // call staright to the createSFML function as part of the contructor so that it is done on creation of the cell.
}

void FloorCell::createSFML()
{
	rectangle.setSize(cellSize); // set the size of the rectangle to be the stored value
	//rectangle.setFillColor(sf::Color::Green); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(cellPosition); // set the postion of the rectangle to be the position passed

	//set the texture, using the extern textureLoader to get the floorTexture
	rectangle.setTexture(&textureLoader.floorTexture, false);
}

void FloorCell::update()
{
}
