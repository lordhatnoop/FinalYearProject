#include "FlameCloakItem.h"
#include "TextureLoader.h"
void FlameCloakItem::update()
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		
		//update the f;lame cloak position by checking for the same button presses as the player and moving the same distance - do this becasue i can't pass the player to get their posiiton without overwriting the defualt update - which would  stop me from using the GAmeitems* update call to update any kind of item.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			Box2DBody->SetLinearVelocity(b2Vec2(-5, 0));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			Box2DBody->SetLinearVelocity(b2Vec2(5, 0));
		}
		else {
			Box2DBody->SetLinearVelocity(b2Vec2(0, 0)); //stop velocity if no button presses
		}
		
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			//yPosition -= 5;
			//used breakpoitns to see the difference between the two so that i could do the math
			Box2DBody->ApplyLinearImpulse(b2Vec2(0, -0.55), b2Vec2(0, 0), true); // apply an impulse to propel flamecloak upward with the player jump - it's a larger impulse than the player's becasue of the larger body size. maths worked out it needed to be roughly 2.2 x the size of the players
			 temp = Box2DBody->GetLinearVelocity();
		}
		//animation stuff
		//update the top and left based on which frame we are in to update the sprite sheet psotion and animate. 
		//for this one, the sprite sheet has consistent distances (made the edited version myself) and so i can just use that distance * the frame we're on to get the position
		textureSubRect.left = 0 + (100 *animationCounter); //proper
		//textureSubRect.left = 0 + (100 * animationCounter);
		textureSubRect.top = 220;
		animationCounter++; //update the animation frame counter for next time
		if (animationCounter > 6) { // 7 frames, but starts from 0 so if above 6 start back from 0
			animationCounter = 0;
		}
		rectangle.setTextureRect(textureSubRect);
		
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
	rectangle.setTexture(&textureLoader.flameCloakTexture); // set the flame cloak texture
	//setup the texture Subrect for taking a fram from the sprite sheet
	textureSubRect.left = 0;
	textureSubRect.width = 70;
	textureSubRect.top = 220;
	textureSubRect.height = 80;
	rectangle.setTextureRect(textureSubRect); //use the subrect
	//rectangle.setFillColor(sf::Color::Red);
}

void FlameCloakItem::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_dynamicBody; 
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
