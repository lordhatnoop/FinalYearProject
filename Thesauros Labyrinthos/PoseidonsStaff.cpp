#include "PoseidonsStaff.h"
#include "TextureLoader.h"
void PoseidonsStaff::update(PlayerItemStatuses * playerStatus)
{
	if (durationTimer->getElapsedTime().asSeconds() < duration) { //so long as item hasn't used it's duration
		if (velocitySet == false) {
			if (playerStatus->playerFacingLeftORRight == true) { // facing right
				Box2DBody->SetLinearVelocity(b2Vec2(3, 0));
			}
			else {
				Box2DBody->SetLinearVelocity(b2Vec2(-3, 0));
			}
		}
		if (animationTimer.getElapsedTime().asSeconds() > 0.3) { //time the animation
			textureSubRect.height = 71;
			textureSubRect.top = 0;
			if (playerStatus->playerFacingLeftORRight == true) { // moving right animation
				textureSubRect.width = 60;
				if (animationCounter == 0) {
					textureSubRect.left = 0;
					
				}
				else if (animationCounter == 1) {
					textureSubRect.left = 66;
					
				}
				else if (animationCounter == 2) {
					textureSubRect.left = 130;
					animationCounter = 0; //reset counter
				}
			}
			else { // inverse for going left
				textureSubRect.width = -60;
				if (animationCounter == 0) {
					textureSubRect.left = 60;

				}
				else if (animationCounter == 1) {
					textureSubRect.left = 126;

				}
				else if (animationCounter == 2) {
					textureSubRect.left = 190;
					animationCounter = 0; //reset counter
				}
			}

			rectangle.setTextureRect(textureSubRect); //use the new frame
			animationTimer.restart(); //restart timer
			animationCounter++; //update counter
		}
		rectangle.setPosition(Box2DBody->GetPosition().x * scale, Box2DBody->GetPosition().y * scale);
	}
	else {
		active = false; //set false once duration is over so we can destroy the item
	}
}

void PoseidonsStaff::createSfml()
{
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(10.f, 10.f));
	rectangle.setOrigin(sf::Vector2f(5.f, 5.f));
	//rectangle.setFillColor(sf::Color::Blue);
	rectangle.setTexture(&textureLoader.tidalWave);


}

void PoseidonsStaff::createBox2D(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_dynamicBody;
	BodyDef.position.Set((xPosition) / scale, yPosition / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements. adjust the xposition slightly so that it centers on the playerand surronds them like a ring
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = "PoseidonsStaff"; //store what type of item it is (FlameCloak) so that we can use that to check for the collision manager how to react when colliding with item. do this to reduce the number of different enums we have and gheneralise items to ITEMS;

	Box2DBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

											  //box2dShape
	Shape.SetAsBox(5.f / scale, 5.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

											  //create the fixture - the world collision fixture, collides with enemies and walls
	ItemWorldFixture.shape = &Shape;
	ItemWorldFixture.density = 1.f;
	ItemWorldFixture.friction = 0.0f;
	ItemWorldFixture.userData = this;
	ItemWorldFixture.filter.categoryBits = ITEM; // set the category to be ITEM
	ItemWorldFixture.filter.maskBits = ENEMY | WALL; //set to collide with walls and enemies, don't want it to collide with players
	Box2DBody->CreateFixture(&ItemWorldFixture);

}

void PoseidonsStaff::destroy(b2World & myWorld)
{
	myWorld.DestroyBody(Box2DBody);
}
