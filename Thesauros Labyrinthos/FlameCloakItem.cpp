#include "FlameCloakItem.h"

void FlameCloakItem::update()
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		
		rectangle.setPosition(Box2DBody->GetPosition().x * scale, Box2DBody->GetPosition().y * scale); //keep updating the position 
	}
	else {
		active = false; //set false once duration is over so we can destroy the item
	}
}

void FlameCloakItem::createSfml()
{
	//set the size and origin of the sfml rectangle
	rectangle.setSize(sf::Vector2f(6.f, 8.f));
	rectangle.setOrigin(sf::Vector2f(3.f, 4.f));
	rectangle.setPosition(xPosition, yPosition); //set initial position - will be the position set by the player -> so the player's own position
	rectangle.setFillColor(sf::Color::Red);
}

void FlameCloakItem::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_kinematicBody; // kinematic becasue we don't want garvity, etc to affect it but want to be able to apply forces to it 
	BodyDef.position.Set((xPosition + 0.3) / scale , yPosition / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "FlameCloak"; //store what type of item it is (FlameCloak) so that we can use that to check for the collision manager how to react when colliding with item. do this to reduce the number of different enums we have and gheneralise items to ITEMS;
							
	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

											  //box2dShape
	Shape.SetAsBox(2.5f / scale, 4.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

	//create the fixture - the world collision fixture, collides with enemies and walls
	ItemWorldFixture.shape = &Shape;
	ItemWorldFixture.density = 1.f;
	ItemWorldFixture.friction = 0.0f;
	ItemWorldFixture.filter.categoryBits = ITEM; // set the category to be ITEM
	ItemWorldFixture.filter.maskBits = ENEMY | WALL; //set to collide with walls and enemies, don't want it to collide with players
	Box2DBody->CreateFixture(&ItemWorldFixture);

	//don't want the second Fixture for this item becasue it has no interaction with the player due to collision
}

void FlameCloakItem::destroy(b2World & myWorld)
{
	myWorld.DestroyBody(Box2DBody); //destroy the body - destroys the fixtures as well
}
