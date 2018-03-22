#include "SpikeTrap.h"

void SpikeTrap::update()
{
}

void SpikeTrap::createSfml()
{
	//setup the rectangle size, pos, texture
	rectangle.setPosition(xPosition, yPosition - 1.4f);
	rectangle.setSize(sf::Vector2f(5.f, 6.f));
	rectangle.setOrigin(sf::Vector2f(2.5f, 3.f));
	rectangle.setTexture(&textureLoader.spikeTexture);
}

void SpikeTrap::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_staticBody;
	BodyDef.position.Set(xPosition / scale, (yPosition - 2.f) / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "SpikeTrap";

	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

											  //box2dShape
	Shape.SetAsBox(3.f / scale, 1.5f / scale);// create the box2d shape - the box- and set it's size.

	//create the fixture - the world collision fixture, collides with enemies and walls
	fixture1.shape = &Shape;
	fixture1.density = 1.f;
	fixture1.friction = 0.0f;
	fixture1.filter.categoryBits = TRAPS; // set the category to be ITEM
	fixture1.isSensor = true; // set to sensor
	fixture1.filter.maskBits = PLAYER; //set to collide with player 
	Box2DBody->CreateFixture(&fixture1);
}
