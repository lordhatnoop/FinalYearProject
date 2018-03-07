#include "RopeItem.h"

void RopeItem::update()
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		//Box2DBody->SetLinearVelocity(b2Vec2(0, 1.1)); //just keep moving the rope up so it hits the ceiling . have to use linear velocity and not force, since force doesn't affect kinematic bodies
		rectangle.setPosition(Box2DBody->GetPosition().x * scale, Box2DBody->GetPosition().y * scale);
	}
	else {
		active = false; //set false once duration is over so we can destroy the item
	}
}

void RopeItem::createSfml()
{
	rectangle.setPosition(xPosition , yPosition ); //set initial position
	rectangle.setOrigin(sf::Vector2f(1.f, 20.f)); // set the origin
	rectangle.setSize(sf::Vector2f(2.f, 40.f)); //set the size 
	rectangle.setFillColor(sf::Color::Magenta);//colour 
}

void RopeItem::createBox2D(b2World &myWorld)
{
	//just add a little to the positions so that we don't spawn directly ontop of the player (positions are set to be players')
	//xPosition = xPosition + 5.f;
	yPosition = yPosition - 16.f;
	//bodyDef
	BodyDef.type = b2_kinematicBody; // kinematic becasue we don't want garvity, etc to affect it but want to be able to apply forces to it 
	BodyDef.position.Set(xPosition / scale , yPosition / scale  ); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "Rope"; //store what type of item it is (rope) so that we can use that to check for the collision manager how to react when colliding with item. do this to reduce the number of different enums we have and gheneralise items to ITEMS;
	//	BodyDef.userData = "playerCharacter"; // set the Userdata so that we can check what is colliding
	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

												//box2dShape
	Shape.SetAsBox(1.f / scale, 20.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

	//create the fixture - the world collision fixture, collides with enemies and walls
	ItemWorldFixture.shape = &Shape;
	ItemWorldFixture.density = 1.f;
	ItemWorldFixture.friction = 0.0f;
	ItemWorldFixture.filter.categoryBits = ITEM; // set the category to be ITEM
	ItemWorldFixture.filter.maskBits = ENEMY | WALL; //set to collide with walls and enemies
	Box2DBody->CreateFixture(&ItemWorldFixture);

	//create the second sensor fixture for detecting player but still allowing them to walk through
	
	ItemPlayerSensorFixture.shape = &Shape;
	ItemPlayerSensorFixture.isSensor = true; //set to be sensor, stuff can walk through it
	ItemPlayerSensorFixture.filter.categoryBits = ITEM; // set the category to be ITEM
	ItemPlayerSensorFixture.filter.maskBits = PLAYER; //set to detect collision with PLAYER
	Box2DBody->CreateFixture(&ItemPlayerSensorFixture);
	
}

void RopeItem::destroy(b2World &myWorld)
{
	myWorld.DestroyBody(Box2DBody); //destroy the body - destroys the fixtures as well
}
