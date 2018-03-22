#include "IdolBoulderTrap.h"

void IdolBoudlerTrap::update()
{
	if(trapTriggered == true){
		rectangle.setPosition(3000, 3000); //temp mvoe off screen to make it look like idol has been picked up
	}
	if (trapTriggered == true && TrapDelayTimer.getElapsedTime().asSeconds() > 2) { //once triggered.wait for a delay, delete idol and create boulder
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)){
		if (boulderCreated == false) { // if triggered but not created boulder - do that
			createBoulderBody();
			createBoulderSfml();
		}
		else if (boulderCreated == true) { //else update the boulder
			xPosition = Box2DBody->GetPosition().x * scale;
			yPosition = Box2DBody->GetPosition().y * scale;
			rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
		}
	}
	else {

	}
}

void IdolBoudlerTrap::createSfml()
{	//setup the rectangle size, pos, texture
	rectangle.setPosition(xPosition -5.f, yPosition -1.5f);
	rectangle.setSize(sf::Vector2f(2.f, 3.f));
	rectangle.setOrigin(sf::Vector2f(1.f, 1.5f));
	rectangle.setTexture(&textureLoader.GoldenIdol);
}

void IdolBoudlerTrap::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_staticBody;
	BodyDef.position.Set((xPosition-5.f) / scale, (yPosition - 1.5f) / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "Idol"; 

	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	//box2dShape
	Shape.SetAsBox(1.f / scale, 1.5f / scale);// create the box2d shape - the box- and set it's size.

	//create the fixture - the world collision fixture, collides with enemies and walls
	fixture1.shape = &Shape;
	fixture1.density = 1.f;
	fixture1.friction = 0.0f;
	fixture1.filter.categoryBits = TRAPS; // set the category to be Traps
	fixture1.isSensor = true; // set to sensor
	fixture1.userData = this;
	fixture1.filter.maskBits = PLAYER; //set to collide with player 
	Box2DBody->CreateFixture(&fixture1);
}

void IdolBoudlerTrap::createBoulderBody()
{
	b2World *world = Box2DBody->GetWorld();
	if (Box2DBody != nullptr) {

		world->DestroyBody(Box2DBody);
		Box2DBody = nullptr;
	}
	BodyDef.type = b2_dynamicBody; //change to dynamic body
	BodyDef.position.Set((xPosition - 50.f) / scale, (yPosition - 6.f) / scale); //change the position of the body to be to the left 
	BodyDef.fixedRotation = false; //allow rotation now for the boulder

	Box2DBody = world->CreateBody(&BodyDef);// recreate the body with the new properties
	

	//shape - circle
	circleShape.m_radius = 5.f / scale;

	//setup the arrow fixture
	fixture2.shape = &circleShape;
	
	fixture2.density = 1.f;
	fixture2.friction = 0.0f;
	fixture2.filter.categoryBits = TRAPS; // set the category to be trap
	fixture2.isSensor = false; 
	fixture2.userData = "Boulder"; //set this so we can check it
	fixture2.filter.maskBits = PLAYER | WALL; //set to collide with player and walls
	Box2DBody->CreateFixture(&fixture2);

	Box2DBody->ApplyForceToCenter(b2Vec2(20, 0),true); //apply force to make it roll
	boulderCreated = true; // set true so this doesn't keep repeating

}

void IdolBoudlerTrap::createBoulderSfml()
{
	//change the rectangle size, pos, texture
	rectangle.setPosition(xPosition - 30.f, yPosition -6.f);
	rectangle.setSize(sf::Vector2f(10.f, 10.f));
	rectangle.setOrigin(sf::Vector2f(5.f, 5.f));
	rectangle.setFillColor(sf::Color(255, 255, 255, 255)); //set to use the texture colour instead of the black colour from before

	//get the Boulder texture
	rectangle.setTexture(&textureLoader.Boulder);

	textureSubRect.width = 140;
	textureSubRect.height = 142;

	rectangle.setTextureRect(textureSubRect);
}
