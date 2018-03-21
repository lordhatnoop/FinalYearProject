#include "MedusaHead.h"

void MedusaHead::update(PlayerItemStatuses * playerStatus)
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		if (playerStatus->playerFacingLeftORRight == true) { //facing right
			Box2DBody->SetTransform(b2Vec2((playerStatus->position.x + 20.f) / scale, (playerStatus->position.y -1.5f) /scale), 0.f); // set body position to be slightly to the right of the player
			rectangle.setPosition(playerStatus->position.x + 2.5f, playerStatus->position.y); //keep updating the position so that the Head follows yhe player just to the side
			

			//animation
			textureSubRect.width = 63; //right so normal width
			if (durationTimer->getElapsedTime().asSeconds() > 0 && durationTimer->getElapsedTime().asSeconds() <= 0.33) {
				textureSubRect.left = 2;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 0.33 && durationTimer->getElapsedTime().asSeconds() <= 0.66) {
				textureSubRect.left = 108;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 0.66 && durationTimer->getElapsedTime().asSeconds() <= 1.0) {
				textureSubRect.left = 220;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 1.0 && durationTimer->getElapsedTime().asSeconds() <= 1.33) {
				textureSubRect.left = 326;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 1.33 && durationTimer->getElapsedTime().asSeconds() <= 1.66) {
				textureSubRect.left = 438;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 1.66 && durationTimer->getElapsedTime().asSeconds() <= 2.0) {
				textureSubRect.left = 560;
				rectangle.setTextureRect(textureSubRect);
			}
		
		}
		else if (playerStatus->playerFacingLeftORRight == false) { //left
			Box2DBody->SetTransform(b2Vec2((playerStatus->position.x - 20.f) / scale, (playerStatus->position.y -1.5f) / scale), 0.f); // set body position to be slightly to the left of the player
			rectangle.setPosition(playerStatus->position.x - 2.15f, playerStatus->position.y); //keep updating the position so that the Head follows yhe player just to the side


			 //animation
			textureSubRect.width = -63; //left, so invert width and add it (non inverted width) to the left so that the texture is inverted
			if (durationTimer->getElapsedTime().asSeconds() > 0 && durationTimer->getElapsedTime().asSeconds() <= 0.33) {
				textureSubRect.left = 65;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 0.33 && durationTimer->getElapsedTime().asSeconds() <= 0.66) {
				textureSubRect.left = 171;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 0.66 && durationTimer->getElapsedTime().asSeconds() <= 1.0) {
				textureSubRect.left = 285;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 1.0 && durationTimer->getElapsedTime().asSeconds() <= 1.33) {
				textureSubRect.left = 389;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 1.33 && durationTimer->getElapsedTime().asSeconds() <= 1.66) {
				textureSubRect.left = 501;
				rectangle.setTextureRect(textureSubRect);
			}
			else if (durationTimer->getElapsedTime().asSeconds() > 1.66 && durationTimer->getElapsedTime().asSeconds() <= 2.0) {
				textureSubRect.left = 623;
				rectangle.setTextureRect(textureSubRect);
			}
		}
	}
	else {
		
		active = false; //set not active to despawn

	}
}

void MedusaHead::createSfml()
{
	//setup the rectangle size, pos, texture
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(4.f, 2.f));
	rectangle.setOrigin(sf::Vector2f(2.f, 1.f));
	rectangle.setTexture(&textureLoader.medusaHead);

	//choose portion of texture
	textureSubRect.top = 0;
	textureSubRect.left = 2;
	textureSubRect.width = 63;
	textureSubRect.height = 20;

	rectangle.setTextureRect(textureSubRect);
}

void MedusaHead::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_staticBody;
	BodyDef.position.Set((xPosition) / scale, yPosition / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "MedusaHead"; //store what type of item it is (MedusaHead) so that we can use that to check for the collision manager how to react when colliding with item. do this to reduce the number of different enums we have and gheneralise items to ITEMS;

	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

											  //box2dShape
	Shape.SetAsBox(20.f / scale, 5.f / scale);// create the box2d shape - the box- and set it's size.

	//create the fixture - the world collision fixture, collides with enemies and walls
	ItemWorldFixture.shape = &Shape;
	ItemWorldFixture.density = 1.f;
	ItemWorldFixture.friction = 0.0f;
	ItemWorldFixture.filter.categoryBits = ITEM; // set the category to be ITEM
	ItemWorldFixture.isSensor = true; // set to sensor
	ItemWorldFixture.filter.maskBits = ENEMY; //set to collide with  enemies, don't want it to collide with players
	Box2DBody->CreateFixture(&ItemWorldFixture);
}

void MedusaHead::destroy(b2World & myWorld)
{
	myWorld.DestroyBody(Box2DBody); //destroy the body - destroys the fixtures as well
}
