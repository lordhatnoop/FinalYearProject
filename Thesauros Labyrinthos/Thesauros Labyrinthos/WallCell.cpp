#include "WallCell.h"
#include "TextureLoader.h"


WallCell::WallCell(int x, int y) {
	
	createSFML(x, y ); // call staright to the createSFML function as part of the contructor so that it is done on creation of the cell.
	createCollisionBox(); // call after the rectangle is created so that collision box can use the sfml sizes.
	
}

void WallCell::createSFML(int x , int y)
{
	rectangle.setSize(sf::Vector2f(10.f,10.f)); 
	//rectangle.setFillColor(sf::Color::Yellow); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(x, y); // set the postion of the rectangle to be the position passed
	

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

void WallCell::update()
{
}

void WallCell::createCollisionBox()
{
}
