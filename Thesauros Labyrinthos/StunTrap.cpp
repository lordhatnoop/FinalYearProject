#include "StunTrap.h"

void StunTrap::update()
{
	if (trapTriggered == true) {
		if (trapCreated == false) { //only do this once
			b2World *world = Box2DBody->GetWorld();
			world->DestroyBody(Box2DBody); //destroy the body
			Box2DBody = nullptr; //set null
			//change rectangle position,etc
			rectangle.setPosition(sf::Vector2f(xPosition - 4.f, yPosition - 4.f));
			rectangle.setSize(sf::Vector2f(4.f, 2.f));
			rectangle.setOrigin(sf::Vector2f(2.f, 1.f));
			rectangle.setFillColor(sf::Color(255, 255, 255, 255)); // changes the colour from black to use the texture colour
			//take the texture from the medusaHeadItem - medusa will stun player
			rectangle.setTexture(&textureLoader.medusaHead);

			//choose portion of texture
			textureSubRect.top = 0;
			textureSubRect.left = 2;
			textureSubRect.width = 63;
			textureSubRect.height = 20;

			rectangle.setTextureRect(textureSubRect);

			durationTimer.restart();
			trapCreated = true; //set true so we don't create trap again
		}
		else {//if it is created keep updating the animation
			if (durationTimer.getElapsedTime().asSeconds() > 0 && durationTimer.getElapsedTime().asSeconds() <= 0.33) {
				textureSubRect.left = 2;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer.getElapsedTime().asSeconds() > 0.33 && durationTimer.getElapsedTime().asSeconds() <= 0.66) {
				textureSubRect.left = 108;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer.getElapsedTime().asSeconds() > 0.66 && durationTimer.getElapsedTime().asSeconds() <= 1.0) {
				textureSubRect.left = 220;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer.getElapsedTime().asSeconds() > 1.0 && durationTimer.getElapsedTime().asSeconds() <= 1.33) {
				textureSubRect.left = 326;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer.getElapsedTime().asSeconds() > 1.33 && durationTimer.getElapsedTime().asSeconds() <= 1.66) {
				textureSubRect.left = 438;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer.getElapsedTime().asSeconds() > 1.66 && durationTimer.getElapsedTime().asSeconds() <= 2.0) {
				textureSubRect.left = 560;
				rectangle.setTextureRect(textureSubRect);

				deactivated = true; //set trap ended.
			}
		}
	}
	else { // if trap not triggered yet
		//keep the sfml on top of the trap
		xPosition = Box2DBody->GetPosition().x * scale;
		yPosition = Box2DBody->GetPosition().y * scale;
		rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
	}
}

void StunTrap::createSfml()
{
	//setup the rectangle size, pos, texture
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(1.f, 1.f));
	rectangle.setOrigin(sf::Vector2f(0.5f, 0.5f));
	rectangle.setFillColor(sf::Color::Black);
}

void StunTrap::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_staticBody;
	BodyDef.position.Set(xPosition / scale, (yPosition) / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "StunTrap"; //store what type of item it is (MedusaHead) so that we can use that to check for the collision manager how to react when colliding with item. do this to reduce the number of different enums we have and gheneralise items to ITEMS;

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

void StunTrap::setTriggered()
{
	trapTriggered = true;
}
