#include "Medusa.h"
#include "TextureLoader.h"

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
	Shape.SetAsBox(3.f / scale, 4.5f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

	//create the fixture
	FixtureDef.shape = &Shape;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.filter.categoryBits = ENEMY; //category
	FixtureDef.filter.maskBits = PLAYER | WALL | PLAYERPROJECTILE; //won't collide with other enemies, but will with player's, projectiles and walls.
	dynamicBody->CreateFixture(&FixtureDef);
}

string Medusa::getName()
{
	return "Medusa";
}

void Medusa::update(PlayerCharacter * player)
{
	//look for the player (get which side they are ona nd if the enemy can see them)
	LookForPlayer(player);
	if (playerInAttackRange == true) {
		attack(player);
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

	//update the sfml
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;

	rectangle.setPosition(xPosition, yPosition);
}

//look for the player in a certain distance and if spotted, check whether left or right and update appropriate bool, else set both bools false
void Medusa::LookForPlayer(PlayerCharacter * player)
{
	if (player->xPosition > xPosition && player->xPosition < xPosition + visionRange || player->xPosition < xPosition && player->xPosition > xPosition - visionRange) {
		if (player->yPosition > yPosition && player->yPosition < yPosition + visionRange || player->yPosition < yPosition && player->yPosition > yPosition - visionRange) {

			std::cout << "PlayerSeen" << endl;
			if (xPosition - player->xPosition <= 10 && xPosition - player->xPosition > 0
				|| xPosition - player->xPosition >= -10 && xPosition - player->xPosition < 0) { //if within 5 either way
				playerInAttackRange = true; //in attack range
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
	dynamicBody->SetLinearVelocity(b2Vec2(3, 0));
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
	dynamicBody->SetLinearVelocity(b2Vec2(-3, 0));
	if (timer.getElapsedTime().asSeconds() > 0.5f) { //make sure some time has passed
													 //check which frame of the aniamtion we are on and change animation to match
													 //distances between each sprite on sheet are different, so have to manually set each position

		textureSubRect.width = -55; //set width negative for flipped and going left. will always be 55 becasue of using highest value to make sprites same size
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
		timer.restart(); //restart the timer
	}
}

void Medusa::attack(PlayerCharacter *player)
{
	player->isStone = true;// set the player's bool so it knows to react
	if (timer.getElapsedTime().asSeconds() > 0.5f) { //make sure some time has passed
		if (attackAnimationFrame == 0) {

		}

	}
}


