#include "NonBodyWallCell.h"

#include "TextureLoader.h"

//THIS class is literally the same as wallcell without the box2d body. didn't want to create so many, so create these where the player won't be able to reach the walls.

NonBodyWallCell::NonBodyWallCell(int x, int y) {

	cellSize = sf::Vector2f(10.f, 10.f); // set the size of the object to be 10 by 10
	cellPosition = sf::Vector2f(x, y);//pos = passed positions

	createSFML(); // call staright to the createSFML function as part of the contructor so that it is done on creation of the cell.
	


}

void NonBodyWallCell::createSFML()
{
	rectangle.setSize(cellSize);
	//rectangle.setFillColor(sf::Color::Yellow); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(cellPosition); // set the postion of the rectangle to be the position passed


										 //set the Texture using textureLoader extern
	rectangle.setTexture(&textureLoader.wallTexture, false);


	//get a random int to be used to choose which texture to use
	//works , but eh. try a counter that cycles through textures to use
	/*int randomInt = rand() % 10;
	if (randomInt == 0 || randomInt == 2 || randomInt == 4 || randomInt == 6 || randomInt == 8 || randomInt == 10) {
	//set the Texture using textureLoader extern
	rectangle.setTexture(&textureLoader.wallTexture, false);
	}
	else {
	//set the Texture using textureLoader extern
	rectangle.setTexture(&textureLoader.wallTexture2, false);
	}*/

	/*if (textureCounter == 0)
	{
	//set the Texture using textureLoader extern
	rectangle.setTexture(&textureLoader.wallTexture, false);
	textureCounter++;
	}
	else if (textureCounter == 1) {
	//set the Texture using textureLoader extern
	rectangle.setTexture(&textureLoader.wallTexture2, false);
	textureCounter = 0;
	}*/


}

NonBodyWallCell::~NonBodyWallCell()
{
}
