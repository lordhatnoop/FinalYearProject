#include "Medusa.h"
#include "TextureLoader.h"
#include "soundManager.h"
#include <iostream>
Medusa::~Medusa()
{
}
Medusa::Medusa(int x, int y)
{
	//set the positions toi be those passed
	xPosition = x;
	yPosition = y;

	//setup the skeletons Stats
	visionRange = 30.f;
	health = 1;

	//call createSfml to create the sprite
	createSFML();

	timer.restart(); //start timer
	petrifyTimer.restart();
	//set what the sound will be by loading the appropriate buffer from the sound manager
	//sound.setBuffer(soundManager.medusaAttack);
}

void Medusa::createSFML()
{
	//create the base sfml for the Medusa
	rectangle.setSize(sf::Vector2f(6.f, 9.f));
	rectangle.setOrigin(sf::Vector2f(3.f, 4.f)); //set the rect origin position. adjusted slightly to the right so that the box2d and sfml match up
	//rectangle.setFillColor(sf::Color::Yellow); // sets the colour of the rectangle to be Yellow // placeholder until texture
	rectangle.setPosition(xPosition, yPosition); // set the postion of the rectangle to be the position passed

	//setup the start position of the texture Subrect (rectangle that is the part of the sprite sheet we are using
	//becasue the sizes of the sprites are incosistent, if we used the sizes fro each sprite frame, the size would keep changing, instead use the largets from all of the frames used, since the extra few pixels on the smaller sprites will be blank/ transparent, and the result will be a sprite that is the same size as the largest, and the transparent frames won't show anything
	textureSubRect.left = 5; //left side of the rectangle
	textureSubRect.top = 93; // top of the rectangle for the sprite
	textureSubRect.height = 73; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
	textureSubRect.width = 55; // width

	rectangle.setTextureRect(textureSubRect); // set to use the texture SubRectangle
	rectangle.setTexture(&textureLoader.medusaTexture); //set to use the medusa texture from the texture loader.
}

void Medusa::createCollisionBox(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(xPosition / scale , yPosition / scale ); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = this;

	dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	//box2dShape
	//need to setup the vertices for the body manually becasue we want to cut the edges of the square to prevent an error with box2d where moving across multiple flat bodies can cause you to become stuck
	b2Vec2 verticices[8];
	verticices[7].Set(-2.2f / scale, 4.5f / scale);
	verticices[6].Set(-3.f / scale, 3.5f / scale);
	verticices[5].Set(-3.f / scale, -3.5f / scale);
	verticices[4].Set(-2.2f / scale, -4.5f / scale);
	verticices[3].Set(2.2f / scale, -4.5f / scale);
	verticices[2].Set(3.f / scale, -3.5f / scale);
	verticices[1].Set(3.f / scale, 3.5f / scale);
	verticices[0].Set(2.2f / scale, 4.5f / scale);

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

string Medusa::getName()
{
	return "Medusa";
}

void Medusa::update(PlayerCharacter * player)
{
	if (isStone == false) { //only update actions when not stone
		//look for the player (get which side they are ona nd if the enemy can see them)
		LookForPlayer(player);
		if (playerInAttackRange == true) {
			if (petrifyTimer.getElapsedTime().asSeconds() > 5.f) { //make sure some time has passed since the last attack 
				attack(player);
			}
			dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // stop velocity so we don't keep sliding
		}
		//if player left or right are true (player is seen on either of those sides) do the one of the below
		else if (playerLeft == true) {
			//xPosition = xPosition - 1.f;
			moveLeft();
		}
		else if (playerRight == true) {

			moveRight();
		}
		else { //else idle
			dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // set idle
		}
	}
	else { //else stop moving (stone)
		dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // set idle
	}
		//update the sfml
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;

	rectangle.setPosition(xPosition, yPosition);
	

	checkDead(); //check if dead
}

//look for the player in a certain distance and if spotted, check whether left or right and update appropriate bool, else set both bools false
void Medusa::LookForPlayer(PlayerCharacter * player)
{
	if (player->xPosition > xPosition && player->xPosition < xPosition + visionRange || player->xPosition < xPosition && player->xPosition > xPosition - visionRange) {
		if (player->yPosition > yPosition && player->yPosition < yPosition + 5.f || player->yPosition < yPosition && player->yPosition > yPosition - 5.f) {

			std::cout << "PlayerSeen" << endl;
			if (xPosition - player->xPosition <= 10 && xPosition - player->xPosition > 0 
				|| xPosition - player->xPosition >= -10 && xPosition - player->xPosition < 0) { //if within 5 either way
				if (yPosition - player->yPosition <= 2 && yPosition - player->yPosition > 0 || yPosition - player->yPosition >= -2 && yPosition - player->yPosition < 0) {
					playerInAttackRange = true; //in attack range
				}
				else { playerInAttackRange = false;  }
			}
			else { playerInAttackRange = false; } //otherwise not
			//still check for which direction player is after these above so that we know which way to attack

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

void Medusa::moveRight()
{
	dynamicBody->SetLinearVelocity(b2Vec2(2, 0));
	if (timer.getElapsedTime().asSeconds() > 0.5f) { //make sure some time has passed
	//check which frame of the aniamtion we are on and change animation to match
	 //distances between each sprite on sheet are different, so have to manually set each position

		textureSubRect.width = 55; //set width positive for going right. will always be 55 becasue of using highest value to make sprites same size
		//don't cchange height cause don't want it upside down
		if (walkAnimationFrame == 0) {
			//set the new sprite position
			textureSubRect.left = 5;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 1) {
			textureSubRect.left = 72;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 2) {
			textureSubRect.left = 134;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 3) {
			textureSubRect.left = 199;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 4) {
			textureSubRect.left = 261;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 5) {
			textureSubRect.left = 321;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 6) {
			textureSubRect.left = 384;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 7) {
			textureSubRect.left = 448;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 8) {
			textureSubRect.left = 514;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 9) {
			textureSubRect.left = 579;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 10) {
			textureSubRect.left = 640;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 11) {
			textureSubRect.left = 714;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 12) {
			textureSubRect.left = 792;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 13) {
			textureSubRect.left = 859;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 14) {
			textureSubRect.left = 931;
			textureSubRect.top = 93;
		}
		//after we've decided which animation to do, update the count so next time through we'll change
		walkAnimationFrame++;
		if (walkAnimationFrame == 15) {
			walkAnimationFrame = 0; //reset counter once we get to the end
		}
		
		rectangle.setTextureRect(textureSubRect); //update the rectangle to use the new texturesubrect info
	}

}

//same as right but negative values for width to flip the image and the velocity to walk left
//uses the same walk animation counter as right so that if it turns around, it carries on the animation smoothly
void Medusa::moveLeft()
{
	dynamicBody->SetLinearVelocity(b2Vec2(-2, 0));
	//if (timer.getElapsedTime().asSeconds() > 0.5f) { //make sure some time has passed
													 //check which frame of the aniamtion we are on and change animation to match
													 //distances between each sprite on sheet are different, so have to manually set each position

		textureSubRect.width = -55; //set width negative for flipped and going left. will always be 55 becasue of using highest value to make sprites same size
		//don't cchange height cause don't want it upside down
		if (walkAnimationFrame == 0) {
			//set the new sprite position
			textureSubRect.left = 60;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 1) {
			textureSubRect.left = 127;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 2) {
			textureSubRect.left = 189;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 3) {
			textureSubRect.left = 254;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 4) {
			textureSubRect.left = 316;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 5) {
			textureSubRect.left = 376;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 6) {
			textureSubRect.left = 439;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 7) {
			textureSubRect.left = 503;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 8) {
			textureSubRect.left = 569;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 9) {
			textureSubRect.left = 634;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 10) {
			textureSubRect.left = 695;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 11) {
			textureSubRect.left = 769;
			textureSubRect.top = 93;
		}
		else if (walkAnimationFrame == 12) {
			textureSubRect.left = 847;
			textureSubRect.top = 94;
		}
		else if (walkAnimationFrame == 13) {
			textureSubRect.left = 914;
			textureSubRect.top = 95;
		}
		else if (walkAnimationFrame == 14) {
			textureSubRect.left = 986;
			textureSubRect.top = 93;
		}
		//after we've decided which animation to do, update the count so next time through we'll change
		walkAnimationFrame++;
		if (walkAnimationFrame == 15) {
			walkAnimationFrame = 0; //reset counter once we get to the end
		}

		rectangle.setTextureRect(textureSubRect); //update the rectangle to use the new texturesubrect info
		timer.restart(); //restart the timer
	//}
}

void Medusa::attack(PlayerCharacter *player)
{
	/*
	if (sound.getStatus() != sound.Playing) { //if the sound isn't already playing
		sound.play(); //play the attack sound
	}*/
	if (timer.getElapsedTime().asSeconds() >= 0.2f) {
		if (attackAnimationFrame == 0) {
			if (soundManager.medusaAttack.getStatus() != soundManager.medusaAttack.Playing) {
				soundManager.medusaAttack.play(); //start sound when we start the animation
			}
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = 76.9;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 6.9;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;
		}
		else if (attackAnimationFrame == 1) {

			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = 167;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 96.9;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;

		}
		else if (attackAnimationFrame == 2) {
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = 268;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 197.9;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;
		}
		else if (attackAnimationFrame == 3) {
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = -380.1;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 310;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;
		}
		else if (attackAnimationFrame == 4) {
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = -485;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 414.9;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;
		}
		else if (attackAnimationFrame == 5) {
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = -597;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 526.9;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;
		}
		else if (attackAnimationFrame == 6) {
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = -719;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 648.9;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;

		}
		else if (attackAnimationFrame == 7) {
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = -840;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 769.9;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;
		}
		else if (attackAnimationFrame == 8) {
			//check which side so we can flip if needed
			if (playerLeft) {
				textureSubRect.left = -958.1;
				textureSubRect.width = -70.1; //facing left, so flip 
			}
			else if (playerRight) {
				textureSubRect.left = 888;
				textureSubRect.width = 70.1; // facing right, so normal
			}
			textureSubRect.top = 478;
			textureSubRect.height = 70;

			if (player->playerInvincible == false) {
				//set the player to stone on  the last frame of the aniamtion so that it doesn't hgappen until aniamtion is complete
				player->isStone = true;// set the player's bool so it knows to react
				player->playerHealth--; //take damage
				player->petrifiedClock->restart(); //restart the petrified clock so that we can have it count the time petrified// do here becasue it was just the easiest place to only reset when turning player to stone

				petrifyTimer.restart();// restart the timer once animation is complete so that we will know have to wait some time before ther next petrify attack (becasue of timer in update)
			}
		}


		
		timer.restart();
		attackAnimationFrame++; //move to next frame
		if (attackAnimationFrame == 9) {
			attackAnimationFrame = 0;
		}
		rectangle.setTextureRect(textureSubRect); //use the updated frame
	}
}


void Medusa::checkDead()
{
	if (health <= 0) { // if ran out of health
		dead = true;
	}
}