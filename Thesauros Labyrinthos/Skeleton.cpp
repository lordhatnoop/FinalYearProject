#pragma once
#include "Skeleton.h"
#include "TextureLoader.h"

#include <iostream>
Skeleton::~Skeleton()
{
}

Skeleton::Skeleton(int x, int y)
{
	//set the positions toi be those passed
	xPosition = x;
	yPosition = y;

	//setup the skeletons Stats
	visionRange = 20.f;
	health = 5;
	
	//call createSfml to create the sprite
	createSFML();

	
}

void Skeleton::createSFML()
{
	//create the base sfml for the skeleton
	rectangle.setSize(sf::Vector2f(6.f, 2.f));
	rectangle.setOrigin(sf::Vector2f(3.f, 1.f)); //starts off on the floor as pile of bones 
	rectangle.setFillColor(sf::Color::Yellow); // sets the colour of the rectangle to be Yellow // placeholder until texture
	rectangle.setPosition(xPosition, yPosition +3); // set the postion of the rectangle to be the position passed

	//setup the start position of the texture Subrect (rectangle that is the part of the sprite sheet we are using
	textureSubRect.left = 415; //left side of the rectangle
	textureSubRect.top = 557; // top of the rectangle for the sprite
	textureSubRect.height = 19; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
	textureSubRect.width = 43; // width
	
	rectangle.setTextureRect(textureSubRect); // set to use the texture SubRectangle
	rectangle.setTexture(&textureLoader.skeletonTexture);

}

void Skeleton::createCollisionBox(b2World & myWorld)
{

	//bodyDef
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(xPosition / scale + 0.169, yPosition / scale + 0.169); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = this;
	//BodyDef.userData = "Enemy"; // set the userdata for the bodydef so that we can check what is colliding
	dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	//box2dShape
	//need to setup the vertices for the body manually becasue we want to cut the edges of the square to prevent an error with box2d where moving across multiple flat bodies can cause you to become stuck
	b2Vec2 verticices[8];
	verticices[7].Set(-1.8f / scale, 4.f / scale);
	verticices[6].Set(-2.f / scale, 3.f / scale);
	verticices[5].Set(-2.f / scale, -3.f / scale);
	verticices[4].Set(-1.8f / scale, -4.f / scale);
	verticices[3].Set(1.8f / scale, -4.f / scale);
	verticices[2].Set(2.f / scale, -3.f / scale);
	verticices[1].Set(2.f / scale, 3.f / scale);
	verticices[0].Set(1.8f / scale, 4.f / scale);

	//Shape.SetAsBox(2.f / scale, 4.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
	Shape.Set(verticices, 8); //set shape to use our vertices

	//create the fixture
	FixtureDef.shape = &Shape;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.filter.categoryBits = ENEMY; //categoiry
	FixtureDef.filter.maskBits = PLAYER | WALL | PLAYERPROJECTILE | ITEM; //won't collide with other enemies, but will with player's, projectiles and walls.
	dynamicBody->CreateFixture(&FixtureDef);
}

string Skeleton::getName()
{
	return "EnemySkeleton";
}

void Skeleton::update(PlayerCharacter *player)
{
	if (isStone == false) { //update actions if not stone
		LookForPlayer(player);
		//if trigger rise is ture and the skeelton hasn't already risen, rise
		if (triggerRise == true && risen == false) {
			rise();
		}
		else if (playerLeft == true) {
			//xPosition = xPosition - 1.f;
			moveLeft();
		}
		else if (playerRight == true) {
			//xPosition = xPosition + 1.f;
			moveRight();
		}
		else {
			dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // set idle
		}
	}
	else { //else stop moving (stone)
		dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // set idle
	}
	//rectangle.setPosition(xPosition, yPosition);
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;
	
	rectangle.setPosition(dynamicBody->GetPosition().x * scale, dynamicBody->GetPosition().y * scale);

	checkDead(); //check if the skeelton has ran out of health

}

//functions that will "look" for the player.
void Skeleton::LookForPlayer(PlayerCharacter *player)
{
	//if the player's position is within the sight range of the skeleton. 
	//sightrange is own position + / - Vision range (checks both sides) 
	int temp1 = xPosition - visionRange;
	int temp2 = xPosition + visionRange;
	if (player->xPosition > xPosition && player->xPosition < xPosition + visionRange || player->xPosition < xPosition && player->xPosition > xPosition - visionRange  ) {
		if (player->yPosition > yPosition && player->yPosition < yPosition + visionRange || player->yPosition < yPosition && player->yPosition > yPosition - visionRange) {
			
			std::cout << "PlayerSeen" << endl;

			if (triggerRise == false) {
				triggerRise = true; // set triggerrise to true so that update can do the rise function
			}
			else if (risen == true) { // if the risen animation has completed
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
	}
	else {
		//if player not seen, make sure the player seen left and right are false
		playerLeft = false;
		playerRight = false;
	}
}

void Skeleton::rise()
{
	if (risen == false) {
		if (timer.getElapsedTime().asSeconds() > 0.7f) {
			timer.restart();
		}
		else if (timer.getElapsedTime().asSeconds() > 0.1f && timer.getElapsedTime().asSeconds() < 0.2f) {
			//update the part of the sprite sheet to use
			textureSubRect.left = 240;
			textureSubRect.top = 555;
			textureSubRect.width = 40;
			textureSubRect.height = 25;
			rectangle.setTextureRect(textureSubRect);
		}
		else if (timer.getElapsedTime().asSeconds() > 0.2f && timer.getElapsedTime().asSeconds() < 0.3f) {
			//update the part of the sprite sheet to use
			textureSubRect.left = 177;
			textureSubRect.top = 535;
			textureSubRect.width = 40;
			textureSubRect.height = 35;
			rectangle.setTextureRect(textureSubRect);

			//update the size of the rectangle
			rectangle.setSize(sf::Vector2f(6.f, 2.5f));
			rectangle.setOrigin(sf::Vector2f(3.f, 1.25f));

		}
		else if (timer.getElapsedTime().asSeconds() > 0.3f && timer.getElapsedTime().asSeconds() < 0.4f) {
			//update the part of the sprite sheet to use
			textureSubRect.left = 100;
			textureSubRect.top = 519;
			textureSubRect.width = 49;
			textureSubRect.height = 58;
			rectangle.setTextureRect(textureSubRect);

			//update the size of the rectangle
			rectangle.setSize(sf::Vector2f(5.f, 3.f));
			rectangle.setOrigin(sf::Vector2f(2.5f, 1.5f));

		}
		else if (timer.getElapsedTime().asSeconds() > 0.4f && timer.getElapsedTime().asSeconds() < 0.5f) {
			//update the part of the sprite sheet to use
			textureSubRect.left = 16;
			textureSubRect.top = 506;
			textureSubRect.width = 54;
			textureSubRect.height = 73;
			rectangle.setTextureRect(textureSubRect);

			//update the size of the rectangle
			rectangle.setSize(sf::Vector2f(4.5f, 4.f));
			rectangle.setOrigin(sf::Vector2f(2.25f, 2.f));

		}
		else if (timer.getElapsedTime().asSeconds() > 0.5f && timer.getElapsedTime().asSeconds() < 0.6f) {
			//update the part of the sprite sheet to use
			textureSubRect.left = 484;
			textureSubRect.top = 396;
			textureSubRect.width = 56;
			textureSubRect.height = 66;
			rectangle.setTextureRect(textureSubRect);

			//update the size of the rectangle
			rectangle.setSize(sf::Vector2f(4.5f, 7.5f));
			rectangle.setOrigin(sf::Vector2f(2.225f, 3.75f));

			
		}
		else if (timer.getElapsedTime().asSeconds() > 0.6f && timer.getElapsedTime().asSeconds() < 0.7f) {
			//update the part of the sprite sheet to use
			textureSubRect.left = 554;
			textureSubRect.top = 396;
			textureSubRect.width = 47;
			textureSubRect.height = 66;
			rectangle.setTextureRect(textureSubRect);

			//update the size of the rectangle
			rectangle.setSize(sf::Vector2f(4.f, 8.f));
			rectangle.setOrigin(sf::Vector2f(2.f, 4.f));

			risen = true; // set risen to be true, becasue the animation has compelted
		}
	}
}

void Skeleton::moveRight()
{
	//xPosition = xPosition + 1.f; // update the position
	dynamicBody->SetLinearVelocity(b2Vec2(2, 0));
	if (timer.getElapsedTime().asSeconds() > 0.5f) { //make sure some time has passed
		
		//check which frame of the aniamtion we are on and change animation to match
		//distances between each sprite on sheet are different, so have to manually set each position
		if (walkAnimationFrame == 0) {
			//set the new sprite position
			textureSubRect.left = 6;
			textureSubRect.width = 42;
			textureSubRect.top = 90;
			textureSubRect.height = 69;

			
			//set next frame
			walkAnimationFrame = 1;
		}
		else if (walkAnimationFrame == 1) {
			//set the new sprite position
			textureSubRect.left = 74;
			textureSubRect.width = 42;
			textureSubRect.top = 90;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 2;
		}
		else if (walkAnimationFrame == 2) {
			//set the new sprite position
			textureSubRect.left = 146;
			textureSubRect.width = 38;
			textureSubRect.top = 90;
			textureSubRect.height = 66;

			//set next frame
			walkAnimationFrame = 3;
		}
		else if (walkAnimationFrame == 3) {
			//set the new sprite position
			textureSubRect.left = 216;
			textureSubRect.width = 49;
			textureSubRect.top = 90;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 4;
		}
		else if (walkAnimationFrame == 4) {
			//set the new sprite position
			textureSubRect.left = 295;
			textureSubRect.width = 41;
			textureSubRect.top = 88;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 5;
		}
		else if (walkAnimationFrame == 5) {
			//set the new sprite position
			textureSubRect.left = 372;
			textureSubRect.width = 43;
			textureSubRect.top = 88;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 6;
		}
		else if (walkAnimationFrame == 6) {
			//set the new sprite position
			textureSubRect.left = 436;
			textureSubRect.width = 47;
			textureSubRect.top = 87;
			textureSubRect.height = 66;

			//set next frame
			walkAnimationFrame = 7;
		}
		else if (walkAnimationFrame == 7) {
			//set the new sprite position
			textureSubRect.left = 507;
			textureSubRect.width = 41;
			textureSubRect.top = 87;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 8;
		}
		else if (walkAnimationFrame == 8) {
			//set the new sprite position
			textureSubRect.left = 576;
			textureSubRect.width = 37;
			textureSubRect.top = 87;
			textureSubRect.height = 70;

			//set next frame - reset back to the first walking frame
			walkAnimationFrame = 1;
		}

		//set the texture rect if we have updated it. do at end so that it'll update no matter which frame
		rectangle.setTextureRect(textureSubRect);
		
	}

}

//same method as above but for going left
//also the "left" of the textureRect is actually the right and we do minus width
//to flip the sprite so it goes left
void Skeleton::moveLeft()
{
	//xPosition = xPosition - 1.f; // update the position
	dynamicBody->SetLinearVelocity(b2Vec2(-2, 0));

	if (timer.getElapsedTime().asSeconds() > 0.5f) { //make sure some time has passed

 //check which frame of the aniamtion we are on and change animation to match
 //distances between each sprite on sheet are different, so have to manually set each position
		if (walkAnimationFrame == 0) {
			//set the new sprite position
			textureSubRect.left = 48;
			textureSubRect.width = -42;
			textureSubRect.top = 90;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 1;
		}
		else if (walkAnimationFrame == 1) {
			//set the new sprite position
			textureSubRect.left = 116;
			textureSubRect.width = -42;
			textureSubRect.top = 90;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 2;
		}
		else if (walkAnimationFrame == 2) {
			//set the new sprite position
			textureSubRect.left = 184;
			textureSubRect.width = -38;
			textureSubRect.top = 90;
			textureSubRect.height = 66;

			//set next frame
			walkAnimationFrame = 3;
		}
		else if (walkAnimationFrame == 3) {
			//set the new sprite position
			textureSubRect.left = 265;
			textureSubRect.width = -49;
			textureSubRect.top = 90;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 4;
		}
		else if (walkAnimationFrame == 4) {
			//set the new sprite position
			textureSubRect.left = 336;
			textureSubRect.width = -41;
			textureSubRect.top = 88;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 5;
		}
		else if (walkAnimationFrame == 5) {
			//set the new sprite position
			textureSubRect.left = 415;
			textureSubRect.width = -43;
			textureSubRect.top = 88;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 6;
		}
		else if (walkAnimationFrame == 6) {
			//set the new sprite position
			textureSubRect.left = 483;
			textureSubRect.width = -47;
			textureSubRect.top = 87;
			textureSubRect.height = 66;

			//set next frame
			walkAnimationFrame = 7;
		}
		else if (walkAnimationFrame == 7) {
			//set the new sprite position
			textureSubRect.left = 548;
			textureSubRect.width = -41;
			textureSubRect.top = 87;
			textureSubRect.height = 69;

			//set next frame
			walkAnimationFrame = 8;
		}
		else if (walkAnimationFrame == 8) {
			//set the new sprite position
			textureSubRect.left = 613;
			textureSubRect.width = -37;
			textureSubRect.top = 87;
			textureSubRect.height = 70;

			//set next frame - reset back to the first walking frame
			walkAnimationFrame = 1;
		}

		//set the texture rect if we have updated it. do at end so that it'll update no matter which frame
		rectangle.setTextureRect(textureSubRect);
	}

}

void Skeleton::attack()
{
	
}

void Skeleton::checkDead()
{
	if (health <= 0) { // if ran out of health
		dead = true;
	}
}
