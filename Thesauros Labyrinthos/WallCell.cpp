#include "WallCell.h"
#include "TextureLoader.h"


WallCell::WallCell(int x, int y, b2World &world) {
	
	cellSize = sf::Vector2f(10.f, 10.f); // set the size of the object to be 10 by 10
	cellPosition = sf::Vector2f(x, y);//pos = passed positions

	createSFML(); // call staright to the createSFML function as part of the contructor so that it is done on creation of the cell.
	createBox2D(world); // call after the rectangle is created so that collision box can use the sfml sizes.
	cellType = "wall cell"; //set the type of cell in case we need to check this
	
}

void WallCell::createSFML()
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

void WallCell::update()
{
}

//function that will add the box2d elements 
void WallCell::createBox2D(b2World &world)
{
	BodyDef.type = b2_staticBody; //set the wall to have a staticbody becasue we don't want them to be affected by gravity or be moved byt the player
	BodyDef.position.Set(cellPosition.x / scale + 0.169, cellPosition.y / scale + 0.169);
	BodyDef.angle = 0;
	BodyDef.userData = this; //set userdata for collision checks
	cellBody = world.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

												//box2dShape
	b2Vec2 verticices[8];
	verticices[7].Set(-5.f / scale, 4.5f / scale);
	verticices[6].Set(-5.f / scale, 4.f / scale);
	verticices[5].Set(-5.f / scale, -4.f / scale);
	verticices[4].Set(-4.7f / scale, -4.5f / scale);
	verticices[3].Set(4.7f / scale, -4.5f / scale);
	verticices[2].Set(5.f / scale, -4.f / scale);
	verticices[1].Set(5.f / scale, 4.f / scale);
	verticices[0].Set(5.f / scale, 4.5f / scale);

	/*
	verticices[7].Set(-4.7f / scale, 4.5f / scale);
	verticices[6].Set(-5.f / scale, 4.f / scale);
	verticices[5].Set(-5.f / scale, -4.5f / scale);
	verticices[4].Set(-4.7f / scale, -4.f / scale);
	verticices[3].Set(4.7f / scale, -4.5f / scale);
	verticices[2].Set(5.0f / scale, -4.f / scale);
	verticices[1].Set(5.0f / scale, 4.f / scale);
	verticices[0].Set(4.7f / scale, 4.5f / scale);
	*/
	//cellBox2DPolygonShape.SetAsBox(5.f / scale, 4.5f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
	cellBox2DPolygonShape.Set(verticices, 8);
	
	//create the fixture
	objectFixtureDef.shape = &cellBox2DPolygonShape;
	objectFixtureDef.density = 1.f;
	objectFixtureDef.friction = 0.0f;
	objectFixtureDef.filter.categoryBits = WALL; // set category to be wall
	objectFixtureDef.filter.maskBits = PLAYER | ENEMY | PLAYERPROJECTILE | ITEM | TREASURE; //set walls to collide with player's, projectiles anbd enemieis, won't collide with each other
	cellBody->CreateFixture(&objectFixtureDef);
}


