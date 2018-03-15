#include "Griffin.h"
#include "TextureLoader.h"
#include "soundManager.h"
#include <iostream>

Griffin::Griffin(int x, int y)
{
	xPosition = x;
	yPosition = y;

	visionRange = 50.f;
	health = 3;

	createSFML();
	timer.restart(); //start the timer
}

void Griffin::createSFML()
{
	rectangle.setPosition(xPosition, yPosition);
	rectangle.setSize(sf::Vector2f(6.f, 8.f));
	rectangle.setOrigin(sf::Vector2f(3.f, 4.f));

	rectangle.setTexture(&textureLoader.griffinTexture); //set the texture to be the griffin texture
	//set the inital portion of the sprite sheet to use 
	textureSubRect.left = 371;
	textureSubRect.width = 107;
	textureSubRect.top = 208;
	textureSubRect.height = 76;
	rectangle.setTextureRect(textureSubRect);
}

void Griffin::createCollisionBox(b2World & myWorld)
{
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(xPosition / scale, yPosition / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = this;

	dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	//box2dShape
	//need to setup the vertices for the body manually becasue we want to cut the edges of the square to prevent an error with box2d where moving across multiple flat bodies can cause you to become stuck
	b2Vec2 verticices[8];
	verticices[7].Set(-2.7f / scale, 4.5f / scale);
	verticices[6].Set(-3.f / scale, 3.5f / scale);
	verticices[5].Set(-3.f / scale, -3.5f / scale);
	verticices[4].Set(-2.7f / scale, -4.5f / scale);
	verticices[3].Set(2.7f / scale, -4.5f / scale);
	verticices[2].Set(3.f / scale, -3.5f / scale);
	verticices[1].Set(3.f / scale, 3.5f / scale);
	verticices[0].Set(2.7f / scale, 4.5f / scale);

	//Shape.SetAsBox(3.f / scale, 4.5f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
	Shape.Set(verticices, 8); //set shape to use the verticices we defined 

	//create the fixture
	FixtureDef.shape = &Shape;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.filter.categoryBits = ENEMY; //category
	FixtureDef.filter.maskBits = PLAYER | WALL | PLAYERPROJECTILE | ITEM; //won't collide with other enemies, but will with player's, projectiles and walls.
	dynamicBody->CreateFixture(&FixtureDef);
}

void Griffin::update(PlayerCharacter * player)
{
	if (chargeStart == false) { //stop looking for player when we charge. - just want it to keep charging until the end of the charge
		LookForPlayer(player); //look for the player
	}

	if (playerLeft == true || playerRight == true) {
		Charge(); //if the player is seen on either side, charge at them
	}
	else {
		Wander(); //will only do wander behaviour if we aren't charging
	}

	//keep updating the position to match the box2d
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;

	rectangle.setPosition(xPosition, yPosition);

	//deal with the animation
	if (timer.getElapsedTime().asSeconds() > 0.2f) {
		//check direction by checking velocity positiove = right, negative = left
		if (dynamicBody->GetLinearVelocity().x > 0) { //heading right
			if (animationCounter == 0) {
				textureSubRect.left = 371;
				textureSubRect.width = 107;
				textureSubRect.top = 208;
				textureSubRect.height = 76;

				animationCounter++;
			}
			else if (animationCounter == 1) {
				textureSubRect.left = 556;
				textureSubRect.width = 107;
				textureSubRect.top = 218;
				textureSubRect.height = 76;

				animationCounter--;
			}
		}
		else if (dynamicBody->GetLinearVelocity().x < 0){ //heading left
			if (animationCounter == 0) {
				textureSubRect.left = 478;
				textureSubRect.width = -107; //left so flip animations
				textureSubRect.top = 208;
				textureSubRect.height = 76;

				animationCounter++;
			}
			else if (animationCounter == 1) {
				textureSubRect.left = 663;
				textureSubRect.width = -107;//left so flip animations
				textureSubRect.top = 218;
				textureSubRect.height = 76;

				animationCounter--;
			}
		}

		rectangle.setTextureRect(textureSubRect); // use the newly set texture frame
		timer.restart();// restart the timer so we can use it for the check again
	}
	
	checkDead(); //check if dead
}


void Griffin::LookForPlayer(PlayerCharacter * player)
{
	//look for player within Griffin sightrange
	if (player->xPosition > xPosition && player->xPosition < xPosition + visionRange || player->xPosition < xPosition && player->xPosition > xPosition - visionRange) {
		if (player->yPosition > yPosition && player->yPosition < yPosition + 5.f || player->yPosition < yPosition && player->yPosition > yPosition - 5.f) {

			std::cout << "PlayerSeen" << endl;


			//check which side
			// if difference is minus, to the right
			if (xPosition - player->xPosition < 0) {
				playerLeft = false;
				playerRight = true;
			}
			else if (xPosition - player->xPosition > 0) { //else to the left
				playerRight = false;
				playerLeft = true;
			}
		}
	}
	else {
		//if player not seen, make sure the player seen left and right are false
		playerLeft = false;
		playerRight = false;
	}
}

void Griffin::Charge()
{
	if (chargeStart == false) {
		ChargeStartPosition = sf::Vector2f(xPosition, yPosition); //set the start point of the charge for reference
		chargeStart = true; //change to true so we don't repeat this
	}
	if (chargeStart == true) {
		//charge  direction 
		if (playerLeft == true) {
			dynamicBody->SetLinearVelocity(b2Vec2(-3, 0));

			if (xPosition <= ChargeStartPosition.x - chargeDistance) { // once travelled the charge distance
				chargeStart = false; //set this false, stops the charge , will start looking for player again
			}
		}
		else if (playerRight == true) {
			dynamicBody->SetLinearVelocity(b2Vec2(3, 0));

			if (xPosition >= ChargeStartPosition.x + chargeDistance) {// once travelled the charge distance
				chargeStart = false; //set this false, stops the charge , will start looking for player again
			}
		}
		//check if charge target distance
	}
}

void Griffin::Wander()
{
	if (wanderStart == false) { // if not gen'd distance
		wanderDistance = rand() % 80 + 1; //generate a randomn distance to wander
		wanderStartPosition = sf::Vector2f(xPosition, yPosition); //set the inital position so we can compare against it 
		leftORRight = rand() % 2 + 1; //random 1 or 2; 1 will be left 2 will be right
		wanderStart = true;
	}
	if (wanderStart == true) {
		if (leftORRight == 1) { //1 so want to go left
			dynamicBody->SetLinearVelocity(b2Vec2(-1, 0));

			if (xPosition <= wanderStartPosition.x - wanderDistance) {//if wandered far enough - 
				wanderStart = false; //set back to false so that we can start the wander again
			}
		}
		else if (leftORRight == 2) { //right
			dynamicBody->SetLinearVelocity(b2Vec2(1, 0));

			if (xPosition >= wanderStartPosition.x + wanderDistance) {//if wandered far enough - 
				wanderStart = false; //set back to false so that we can start the wander again
			}
		}

		
	}
}

void Griffin::checkDead()
{
	if (health <= 0) { // if ran out of health
		dead = true;
	}
}