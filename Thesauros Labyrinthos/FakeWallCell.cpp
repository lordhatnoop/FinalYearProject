#include "FakeWallCell.h"
#include "TextureLoader.h"
FakeWallCell::FakeWallCell(int x, int y, b2World & world)
{
	cellSize = sf::Vector2f(10.f, 10.f); // set the size of the object to be 10 by 10
	cellPosition = sf::Vector2f(x, y);//pos = passed positions

	createSFML(); // call staright to the createSFML function as part of the contructor so that it is done on creation of the cell.
	createBox2D(world); // call after the rectangle is created so that collision box can use the sfml sizes.

	cellType = "Fake wall cell";
}

void FakeWallCell::createSFML()
{
	rectangle.setSize(cellSize);
	
	rectangle.setPosition(cellPosition); // set the postion of the rectangle to be the position passed
	//set the Texture using textureLoader extern
	rectangle.setTexture(&textureLoader.fakeWallTexture, false);
}

void FakeWallCell::update()
{
}

void FakeWallCell::createBox2D(b2World & world)
{
	//using dynamic bodies that reacted to gravity caused severe lag
	//BodyDef.type = b2_dynamicBody; //we want this type of wall to be affected by gravity and colliison so use dynamic instead of the other walls static
	BodyDef.type = b2_kinematicBody; //allow for forces but no gravity or collision to prevent lag
	BodyDef.position.Set((cellPosition.x + 5.f)/ scale , (cellPosition.y +5.f) / scale); //add 0.169 to get the body into the correct position. would usually set the sfml rect origin to be size to sync the sfml and box2d, but in this case, i don't want the sfml to move to the bodyin the update and instead want the body to be created where the sfml rect is
	BodyDef.angle = 0;
	
	BodyDef.userData = this; //set userdata for collision checks
	BodyDef.fixedRotation = true; //set fixed rotation so that it doesn't rotate when moving
	cellBody = world.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	//box2dShape
	//create custom shape for the wall using vertices points. corners cut off top 
	b2Vec2 verticices[8];
	verticices[7].Set(-5.f / scale, 4.5f / scale);
	verticices[6].Set(-5.f / scale, 4.f / scale);
	verticices[5].Set(-5.f / scale, -4.f / scale);
	verticices[4].Set(-4.7f / scale, -4.5f / scale);
	verticices[3].Set(4.7f / scale, -4.5f / scale);
	verticices[2].Set(5.f / scale, -4.f / scale);
	verticices[1].Set(5.f / scale, 4.f / scale);
	verticices[0].Set(5.f / scale, 4.5f / scale);

	//cellBox2DPolygonShape.SetAsBox(5.f / scale, 4.5f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
	cellBox2DPolygonShape.Set(verticices, 8); //set shape to use the vertices we created

	//create the fixture
	objectFixtureDef.shape = &cellBox2DPolygonShape;
	objectFixtureDef.density = 1.f;
	objectFixtureDef.friction = 0.0f;
	objectFixtureDef.filter.categoryBits = WALL; // set category to be wall

	objectFixtureDef.filter.maskBits = PLAYER | ENEMY | PLAYERPROJECTILE | TREASURE | ITEM; //set walls to collide with player's, projectiles anbd enemieis, won't collide with each other
	cellBody->CreateFixture(&objectFixtureDef);
}
