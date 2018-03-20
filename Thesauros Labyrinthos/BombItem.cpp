#include "BombItem.h"
#include "TextureLoader.h"
void BombItem::update(PlayerItemStatuses *playerStatus)
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		//change the animation as time goes along
		if (durationTimer->getElapsedTime().asSeconds() >= 0.5 && durationTimer->getElapsedTime().asSeconds() <= 1) {
			textureSubRect.left = 82;
			rectangle.setTextureRect(textureSubRect);
		}
		else if (durationTimer->getElapsedTime().asSeconds() >= 1 && durationTimer->getElapsedTime().asSeconds() <= 1.5) {
			textureSubRect.left = 114;
			rectangle.setTextureRect(textureSubRect);
		}
		else if (durationTimer->getElapsedTime().asSeconds() >= 1.5 && durationTimer->getElapsedTime().asSeconds() <= 2) {
			textureSubRect.left = 146;
			rectangle.setTextureRect(textureSubRect);
		}
		else if (durationTimer->getElapsedTime().asSeconds() > 2.5) { //after a delay do an explosion
			if (exploded == false) { // if not already done the explosion
				
				
				// change shape size to be bigger for the explosion
				Shape.m_radius = 5.f / scale;
				//destroy the fixture 
				Box2DBody->DestroyFixture(Box2DBody->GetFixtureList());
				//recreate it with the larger size
				ItemWorldFixture.shape = &Shape;
				ItemWorldFixture.density = 1.f;
				ItemWorldFixture.friction = 0.0f;
				//ItemWorldFixture.isSensor = true;
				ItemWorldFixture.filter.categoryBits = ITEM; // set the category to be ITEM
				ItemWorldFixture.filter.maskBits = ENEMY | WALL; //set to collide with walls and enemies, don't want it to collide with players
				Box2DBody->CreateFixture(&ItemWorldFixture);
					

				//update size to match explosion
				rectangle.setSize(sf::Vector2f(20.f, 20.f));
				rectangle.setOrigin(sf::Vector2f(10.f, 10.f));
				//choose the explosion section of the sprite sheet
				textureSubRect.left = 31;
				textureSubRect.width = 16;
				textureSubRect.height = 16;
				textureSubRect.top = 0;

				rectangle.setTextureRect(textureSubRect);
				
				
				exploded = true; //set true so can't be done again
			}
		}
	}
	else {
		active = false; //set false once duration is over so we can destroy the item
	}
}

void BombItem::createSfml()
{
	rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
	rectangle.setSize(sf::Vector2f(2.f, 2.f));
	rectangle.setOrigin(sf::Vector2f(1.f, 1.f));
	//rectangle.setFillColor(sf::Color::White);
	rectangle.setTexture(&textureLoader.bombTexture);

	//choose starting portion of the sprite sheet
	textureSubRect.left = 63;
	textureSubRect.width = 16;
	textureSubRect.top = 0;
	textureSubRect.height = 16;
	rectangle.setTextureRect(textureSubRect);
}

void BombItem::createBox2D(b2World & myWorld)
{
	//create the body 
	//bodyDef
	BodyDef.type = b2_dynamicBody;
	BodyDef.position.Set((xPosition) / scale, yPosition / scale + 20 /scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "BombItem"; //store what type of item it is (FlameCloak) so that we can use that to check for the collision manager how to react when colliding with item. do this to reduce the number of different enums we have and gheneralise items to ITEMS;
	BodyDef.gravityScale = 0;

	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above
	
	//box2dShape
	//Shape.SetAsBox(10.f / scale, 10.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
	Shape.SetAsBox(2.f / scale, 2.f / scale); //small size to start with

	// create the fixture - the world collision fixture, collides with enemies and walls
	ItemWorldFixture.shape = &Shape;
	ItemWorldFixture.density = 1.f;
	ItemWorldFixture.friction = 0.0f;
	//ItemWorldFixture.isSensor = true;
	ItemWorldFixture.filter.categoryBits = ITEM; // set the category to be ITEM
	ItemWorldFixture.filter.maskBits = ENEMY | WALL; //set to collide with walls and enemies, don't want it to collide with players
	Box2DBody->CreateFixture(&ItemWorldFixture);
										 
}

void BombItem::destroy(b2World & myWorld)
{
	myWorld.DestroyBody(Box2DBody); //destroy the body - destroys the fixtures as well
	exploded = false; //reset for next bomb
}
