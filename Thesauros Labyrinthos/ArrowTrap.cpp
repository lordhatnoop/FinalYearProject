#include "ArrowTrap.h"

void ArrowTrap::update()
{
	if (trapTriggered == true) { //once triggered. delete pressureplate and create arrow
		if (arrowShot == false) { // if triggered but not shot arrow - do that
			createArrowBody();
			createArrowSfml();
		}
		else if (arrowShot == true) { //else update the arrow
			xPosition = Box2DBody->GetPosition().x * scale;
			yPosition = Box2DBody->GetPosition().y * scale;
			rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
		}
	}
	else  {

	}
}

void ArrowTrap::createSfml()
{
	//setup the rectangle size, pos, texture
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(1.f, 1.f));
	rectangle.setOrigin(sf::Vector2f(0.5f, 0.5f));
	rectangle.setFillColor(sf::Color::Black);
}

void ArrowTrap::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_staticBody;
	BodyDef.position.Set(xPosition / scale, (yPosition) / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "ArrowTrap"; //store what type of item it is (MedusaHead) so that we can use that to check for the collision manager how to react when colliding with item. do this to reduce the number of different enums we have and gheneralise items to ITEMS;

	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

											  //box2dShape
	Shape.SetAsBox(0.5f / scale, 0.5f / scale);// create the box2d shape - the box- and set it's size.

											  //create the fixture - the world collision fixture, collides with enemies and walls
	fixture1.shape = &Shape;
	fixture1.density = 1.f;
	fixture1.friction = 0.0f;
	fixture1.filter.categoryBits = TRAPS; // set the category to be ITEM
	fixture1.isSensor = true; // set to sensor
	fixture1.userData = this;
	fixture1.filter.maskBits = PLAYER; //set to collide with player 
	Box2DBody->CreateFixture(&fixture1);
}

void ArrowTrap::createArrowBody()
{
	b2World *world = Box2DBody->GetWorld();
	if (Box2DBody != nullptr) {

		world->DestroyBody(Box2DBody);
		Box2DBody = nullptr;
	}
	BodyDef.type = b2_dynamicBody; //change to dynamic body
	BodyDef.position.Set((xPosition - 30.f) / scale, yPosition  / scale); //change the position of the body to be to the left 

	Box2DBody = world->CreateBody(&BodyDef);// recreate the body with the new properties
	Box2DBody->SetGravityScale(0);// turn off gravity

	Shape.SetAsBox(1.f / scale, 0.5f / scale); //change shape to be more arrow shape

											   //setup the arrow fixture
	fixture2.shape = &Shape;
	fixture2.density = 1.f;
	fixture2.friction = 0.0f;
	fixture2.filter.categoryBits = TRAPS; // set the category to be trap
	fixture2.isSensor = true; // set to sensor
	fixture2.userData = "Arrow"; //set this so we can check it
	fixture2.filter.maskBits = PLAYER; //set to collide with player 
	Box2DBody->CreateFixture(&fixture2);

	Box2DBody->SetLinearVelocity(b2Vec2(2, 0)); //set some veloctiy to the arrow
	arrowShot = true; // set true so this doesn't keep repeating
}

void ArrowTrap::createArrowSfml()
{
	//change the rectangle size, pos, texture
	rectangle.setPosition(xPosition - 30.f, yPosition);
	rectangle.setSize(sf::Vector2f(2.f, 1.f));
	rectangle.setOrigin(sf::Vector2f(1.f, 0.5f));
	rectangle.setFillColor(sf::Color(255,255,255,255)); //set to use the texture colour instead of the black colour from before

	//get the arrow texture
	rectangle.setTexture(&textureLoader.playerTexture);
	//set texture sheet position
	textureSubRect.left = 998;
	textureSubRect.width = 32;
	textureSubRect.top = 342;
	textureSubRect.height = 5;

	rectangle.setTextureRect(textureSubRect); //set to use the selected portion of the sprite sheet
}

void ArrowTrap::setTriggered()
{
	trapTriggered = true;
}
