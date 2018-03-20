#include "PlayerCharacter.h"
#include "textureLoader.h"
#include "ItemHolder.h"
PlayerCharacter::PlayerCharacter(int x, int y)
{
	treasure = 0;
	xPosition = x;
	yPosition = y;
	createSFML();
	//createCollisionBox(myWorld);
	torch = new TorchLight(xPosition,yPosition);

	//start the clocks
	updateclock = new sf::Clock;
	invincibilityClock = new sf::Clock;
	petrifiedClock = new sf::Clock;
	CurrentItemClock = new sf::Clock;
	animationTimer.restart(); 

	//testing items - push them to aquired so we can use them
	//AquiredItems.push_back(std::shared_ptr<HermesHelm>(new HermesHelm()));
	AquiredItems.push_back(std::shared_ptr<HermesBoots>(new HermesBoots()));
	//AquiredItems.push_back(std::shared_ptr<BombItem>(new BombItem()));
//	AquiredItems.push_back(std::shared_ptr<RopeItem>(new RopeItem())); //start the player with ropes
	//AquiredItems.push_back(std::shared_ptr<FlameCloakItem>(new FlameCloakItem()));  // testing fire cloak
}

void PlayerCharacter::createSFML()
{
	rectangle.setSize(sf::Vector2f(3.f, 6.f));
	rectangle.setOrigin(sf::Vector2f(1.5f, 3.f));
	//rectangle.setFillColor(sf::Color::Green); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setTexture(&textureLoader.playerTexture); //set the texture with the textureLoader

	//setup start position on sprite sheet
	textureSubRect.left = 12.7f;
	textureSubRect.width = 32.f;
	textureSubRect.top = 12.f;
	textureSubRect.height = 64.f;
	rectangle.setTextureRect(textureSubRect);//tell rectangle to use the setup positions

	rectangle.setPosition(xPosition, yPosition); // set the postion of the rectangle to be the position passed
	//rectangle.setTextureRect(25,25)
	//rectangle.setTexture(&texture, false);

	//setup the shield circle
	shieldCircle.setPosition(xPosition, yPosition); //position
	shieldCircle.setOrigin(6.f, 6.f); //origin = half of size to center it on player
	shieldCircle.setRadius(6); //radius of the circle
	shieldCircle.setScale(0.7, 1); // set the x scale a bit smaller to mold the circle slightly


	//setup the pertrifed overlay - translucent brown overlay to make us look like stone
	petrifyOverlay.setSize(sf::Vector2f(3.f, 6.f));
	petrifyOverlay.setOrigin(1.5f, 3.f);
	petrifyOverlay.setFillColor(sf::Color(139, 69, 19, 100)); //translucent brown. will only be drawn when we are stone
}

void PlayerCharacter::createCollisionBox(b2World &myWorld)
{

	//if (Box2dCreated == false) { // only do this if box2d hasn't been created. Player isn't deleted with level, and easier to do this rather than keep


		//bodyDef
		BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
		BodyDef.position.Set(xPosition / scale + 0.169, yPosition / scale + 0.17); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
		BodyDef.angle = 0;
		BodyDef.fixedRotation = true; // prevent rotation
		BodyDef.userData = this;
		//	BodyDef.userData = "playerCharacter"; // set the Userdata so that we can check what is colliding
		dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

		//box2dShape
		//need to setup the vertices for the body manually becasue we want to cut the edges of the square to prevent an error with box2d where moving across multiple flat bodies can cause you to become stuck
		b2Vec2 verticices[8];
		verticices[7].Set(-1.2f / scale, 3.f / scale);
		verticices[6].Set(-1.5f / scale, 2.f / scale);
		verticices[5].Set(-1.5f / scale, -2.f / scale);
		verticices[4].Set(-1.5f / scale, -3.f / scale);
		verticices[3].Set(1.5f / scale, -3.f / scale);
		verticices[2].Set(1.5f / scale, -2.f / scale);
		verticices[1].Set(1.5f / scale, 2.f / scale);
		verticices[0].Set(1.2f / scale, 3.f / scale);

		//Shape.SetAsBox(1.5f / scale, 3.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
		Shape.Set(verticices, 8);
		//create the fixture
		FixtureDef.shape = &Shape;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.0f;
		FixtureDef.filter.categoryBits = PLAYER; // set the category to be player
		FixtureDef.filter.maskBits = ENEMY | WALL| TREASURE; //set to collide with walls and enemies
		dynamicBody->CreateFixture(&FixtureDef);

		//setup the sensors
		//add three sensors so that we can detect when the play is touching the ground or clung to the side of a wall to allow jumping (for side, it's lore wise going to be wall climbing). let's us make sure can't jump when touching ceiling (contact with top of player)
		//foot sensor
		Shape.SetAsBox(0.75f / scale, 0.3f / scale, b2Vec2(0, 3.375f /scale),0); //setup the size of the sensor and set it's center at the bottom of the player
		footSensor.isSensor = true;// set as a sensor
		footSensor.shape = &Shape;
		footSensor.filter.categoryBits = PLAYER; // set the category to be player
		footSensor.filter.maskBits = WALL; //set to collide with walls only since that's what we want it to detect as a sensor
		dynamicBody->CreateFixture(&footSensor); //add the sensor to the body

		//left sensor
		Shape.SetAsBox(0.3f / scale, 1.5f / scale, b2Vec2(-1.5f /scale, 0.f ), 0); //setup the size of the sensor and set it's center at the left of the player
		leftSensor.isSensor = true;// set as a sensor
		leftSensor.shape = &Shape;
		leftSensor.filter.categoryBits = PLAYER; // set the category to be player
		leftSensor.filter.maskBits =  WALL; //set to collide with walls 
		dynamicBody->CreateFixture(&leftSensor); //add the sensor to the body

		//right sensor
		Shape.SetAsBox(0.3f / scale, 1.5f / scale, b2Vec2(1.5f / scale, 0.f), 0); //setup the size of the sensor and set it's center at the right of the player
		rightSensor.isSensor = true;// set as a sensor
		rightSensor.shape = &Shape;
		rightSensor.filter.categoryBits = PLAYER; // set the category to be player
		rightSensor.filter.maskBits = WALL; //set to collide with walls 
		dynamicBody->CreateFixture(&leftSensor); //add the sensor to the body
		//Box2dCreated = true; // set to true so that this doesn't happen again
	//}
}

string PlayerCharacter::getName()
{
	return "PlayerCharacter";
}

void PlayerCharacter::update(float dt, b2World &myWorld)
{
	if (isStone == false) { //so long as the player isn't stone, let them take action
		//if the player touched an enemy, call the collide with enemy function
		if (m_contactingEnemy == true) {
			collidingWithEnemy();
			m_contactingEnemy = false; // stop contacting enemy 
		}
		//if the player is invincible and it's not because of the shield
		if (playerInvincible == true && shielding == false) {
			InvincibleTimer(); // call the invincibility function
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
			playerHealth--;//reduce playerHealth by 1

		}
		//move player left by reducing the xposition and setting the posiiton of the rectangle again.
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			//xPosition -= 5;
			//dynamicBody->ApplyForce(b2Vec2(-5, 0), b2Vec2(0, 0), true);
			//dynamicBody->SetLinearVelocity(b2Vec2(-5, -1));
			walkLeft(); //call the walk left function
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			//xPosition += 5;
			//dynamicBody->ApplyForce(b2Vec2(5, 0), b2Vec2(0, 0), true);
			//dynamicBody->SetLinearVelocity(b2Vec2(5 , -1));
			walkRight(); //walk right function
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			//yPosition += 5;
		}
		else {
			dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // if no buttons are being pressed, no velocity
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { //attack
			arrowVector.push_back(std::shared_ptr<playerArrow>(new playerArrow(xPosition, yPosition, facingLeftORRight))); //push an arrow to the back of the vector.
			arrowVector.back()->createHitBox(myWorld);
		}

		//not else if so that we can jump while moving
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			//yPosition -= 5;
		//check bools for varius status so that we know what we can and can't do before attempting to do it
			if (itemStatuses.hermesHelmActive == true) { // if hermes helm is active - allow for flight. check for this first as priority over jumping
				//use velocity for flight not impulse cause we want it to be smoother than a jump
				b2Vec2 v = dynamicBody->GetLinearVelocity(); //get the current velocity so that we don't cancle out movement velocity
				v.y = -10.f; //set the y component to fly
				dynamicBody->SetLinearVelocity(v); //apply the velocity, x should be unchanged, allowing for nomrla movement while flying
			}
			else if (canJump == true) { // else check if we can jump and jump if we can
				dynamicBody->ApplyLinearImpulse(b2Vec2(0, -0.25), b2Vec2(0, 0), true); // apply an impulse to propel player upard as a jump
				temp = dynamicBody->GetLinearVelocity();
			}
		}

		if (canClimb == true) { //check if can CLimb is true so we know whether we are touching a rope we can climb (set by collision listener)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { //if we are pressing the W (up) key while can climb is true, climb the rope
				//yPosition -= 5;

				dynamicBody->SetLinearVelocity(b2Vec2(0, -10.f)); //set negative velocity so that we move upwards
			}
		}

		//ITEM USE
		ItemUse(myWorld);
		
		//shield use
		playerShield();
		
	}
	else if (isStone == true) { //else if player is stone
		Petrified(); //run the petrified code (timer for how long to be petrified)
	}


	//rectangle.setPosition(xPosition, yPosition);
	//update the xPOsition and Yposiition so that it can still be sued for the torch
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;
	//update the rectangle position to be the new position if there is one
	rectangle.setPosition(dynamicBody->GetPosition().x * scale, dynamicBody->GetPosition().y * scale);
	shieldCircle.setPosition(dynamicBody->GetPosition().x * scale, dynamicBody->GetPosition().y * scale); //also update the circle
	petrifyOverlay.setPosition(dynamicBody->GetPosition().x * scale, dynamicBody->GetPosition().y * scale); //and the petrify overlay

	itemStatuses.position = sf::Vector2f(xPosition, yPosition); //keep the item position up to date if needed for the items
	itemStatuses.playerFacingLeftORRight = facingLeftORRight; //set the facing left or right bool.


	//limit how frequently the torch fuel will countdown based on time 
	if (updateclock->getElapsedTime().asSeconds() > 1) {
		torchCountdown(); // call the torch countdown to remove some fuel from the player
		updateclock->restart(); // restart the timer so that we can check when enough time has passed again
	}
	//call the torch update so that it updates along with the player
	torch->update(xPosition,yPosition,maxTorchFuel,currentTorchFuel);

	//ITEMS VECTOR UPDATE
	if(activeItem != nullptr) { //so long as active item isn't null
		activeItem->update(&itemStatuses); //update it.
		if (activeItem->active == false) {
			activeItem->destroy(myWorld); //destroy the item // call here to pass world
			activeItem = nullptr; //once active item has been detsoryed and isn't active anymore, it can become nullptr again so we stop updating it
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { // if pressed q
		if (itemsOnCooldown == false) { // only allow swapping when items aren't on cooldown( just to prevent issues)
			currentItem++; //update the current item
		}
	}
	if (currentItem > AquiredItems.size() - 1) { // check current item isn't too large
		currentItem = 0; //if it is reset it back to 0
	}
}

void PlayerCharacter::torchCountdown()
{
	if (currentTorchFuel > 0) {
		currentTorchFuel -= 0.5; // just minus 0.5 perupdate for a test
	}
}

void PlayerCharacter::collidingWithEnemy()
{
	//so long as the player isn't invincible
	if (playerInvincible == false) {
		playerHealth--; //reduce the playerHealth
		dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(5, 0), true); //apply an impulse to player, to knock them back
		playerInvincible = true; // set the player invinicble so can't keep taking damage
		playerHit = false; // used to differntaite between types of invincibility 
		invincibilityClock->restart(); // restart the clock so we can time it
	}
}

void PlayerCharacter::InvincibleTimer()
{
	float timePassed = invincibilityClock->getElapsedTime().asSeconds();

	//want player to flash while invincible, so keep swapping alpha
	if (timePassed < 0.2f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 128));
	}
	else if (timePassed > 0.2f && timePassed < 0.4f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 0.4f & timePassed < 0.6f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 128));
	}
	else if (timePassed > 0.6f && timePassed < 0.8f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 0.8f && timePassed < 1.f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 1.f & timePassed < 1.2f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 128));
	}
	else if (timePassed > 1.2f && timePassed < 1.4f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 1.4f & timePassed < 1.6f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 128));
	}
	else if (timePassed > 1.6f && timePassed < 1.8f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 1.8f & timePassed < 2.f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 128));
	}
	else if (timePassed > 2.f && timePassed < 2.2f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 2.2f && timePassed < 2.4f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 2.4f && timePassed < 2.6f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 2.6f && timePassed < 2.8f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}
	else if (timePassed > 2.8f && timePassed < 3.f) {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
	}

	
	//once three seconds have passed, remove invincibility
	if (invincibilityClock->getElapsedTime().asSeconds() > 3) {
		playerInvincible = false;
		playerHit = false;
	}
}

void PlayerCharacter::walkRight()
{
	if (itemStatuses.hermesBootsActive == true) { // if the sprint item is active
		dynamicBody->SetLinearVelocity(b2Vec2(8, -1)); //double speed
	}
	else {
		dynamicBody->SetLinearVelocity(b2Vec2(4, -1)); //else normal speed
	}

	//check the counter for current frame, set the position on sprite sheet, then update counter and recatangle
	//sprite sheet has inconsistenet sizes for each frame. just use the largest avlue for the width and height to add a few blank frames to the smaller ones so that the sprite doesn't keep visibly chanign shape.
	if (animationTimer.getElapsedTime().asSeconds() > 0.025) {
		if (walkAnimationCounter == 1) {
			textureSubRect.left = 12.7f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 2;
		}
		else if (walkAnimationCounter == 2) {
			textureSubRect.left = 322.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 3;
		}
		else if (walkAnimationCounter == 3) {
			textureSubRect.left = 401.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 13.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 4;
		}
		else if (walkAnimationCounter == 4) {
			textureSubRect.left = 480.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 13.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 5;
		}
		else if (walkAnimationCounter == 5) {
			textureSubRect.left = 570.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 6;
		}
		else if (walkAnimationCounter == 6) {
			textureSubRect.left = 650.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 7;
		}
		else if (walkAnimationCounter == 7) {
			textureSubRect.left = 730.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 8;
		}
		else if (walkAnimationCounter == 8) {
			textureSubRect.left = 816.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 13.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 9;
		}
		else if (walkAnimationCounter == 9) {
			textureSubRect.left = 899.f;
			textureSubRect.width = 41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 2;
		}

		rectangle.setTextureRect(textureSubRect); // set to use the new texture rect positions
		animationTimer.restart();// reset so we can check again
	}
		facingLeftORRight = true; // set true for facing right
	
}

//same as walk right, but inversed velocity and and texture rect positins start from the right and minus the width to flip the image
void PlayerCharacter::walkLeft()
{
	if (itemStatuses.hermesBootsActive == true) { // if the sprint item is active
		dynamicBody->SetLinearVelocity(b2Vec2(-8, -1)); //double speed
	}
	else {
		dynamicBody->SetLinearVelocity(b2Vec2(-4, -1)); //else normal speed
	}

	if (animationTimer.getElapsedTime().asSeconds() > 0.025) {
		//check the counter for current frame, set the position on sprite sheet, then update counter and recatangle
		if (walkAnimationCounter == 1) {
			textureSubRect.left = 44.7f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 2;
		}
		else if (walkAnimationCounter == 2) {
			textureSubRect.left = 354.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 3;
		}
		else if (walkAnimationCounter == 3) {
			textureSubRect.left = 433.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 13.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 4;
		}
		else if (walkAnimationCounter == 4) {
			textureSubRect.left = 521.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 13.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 5;
		}
		else if (walkAnimationCounter == 5) {
			textureSubRect.left = 601.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 6;
		}
		else if (walkAnimationCounter == 6) {
			textureSubRect.left = 682.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 7;
		}
		else if (walkAnimationCounter == 7) {
			textureSubRect.left = 765.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 8;
		}
		else if (walkAnimationCounter == 8) {
			textureSubRect.left = 851.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 13.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 9;
		}
		else if (walkAnimationCounter == 9) {
			textureSubRect.left = 933.f;
			textureSubRect.width = -41.f;
			textureSubRect.top = 12.f;
			textureSubRect.height = 64.f;

			walkAnimationCounter = 2;
		}
		
		rectangle.setTextureRect(textureSubRect); // set to use the new texture rect positions
		animationTimer.restart();
	}
	facingLeftORRight = false; //set false for facing left
}

void PlayerCharacter::Petrified()
{
	//clock is restarted in medusa.cpp when she attacks
	int petrifiedTime = petrifiedClock->getElapsedTime().asSeconds();
	if (petrifiedTime >= 2) {
		isStone = false; //no longer stone
		playerHealth--; //take damage
	}
}

//function for checking if we can use items and allowing for use if we can 
void PlayerCharacter::ItemUse(b2World &myWorld)
{
	//check for items on cooldown
	if (CurrentItemClock->getElapsedTime().asSeconds() > AquiredItems[currentItem]->coolDownTimer) {// if the time since last use is longer than the item cooldown
		itemsOnCooldown = false; // not on cooldown
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		if (itemsOnCooldown == false) { //so long as items aren't on cooldown
										//set the active item to be the current item when we press the button to use items
										//done so that we don't have to loop through the aquired vector checking which items are active to update, and instead can just update whatever is attached to active item
			activeItem = AquiredItems[currentItem];
			//update the position of the item
			activeItem->xPosition = xPosition;
			activeItem->yPosition = yPosition;
		
			activeItem->itemDamage = activeItem->itemDamage * ItemDamageMultiplier; //multiply the item damage by the multplier to apply any item damage upgrades 
			activeItem->createSfml();
			activeItem->createBox2D(myWorld);
			
			activeItem->durationTimer->restart(); //resatrt the items timer so that we get accurate duration
			activeItem->active = true;

			// ITEMS ON COOLDOWN UPDATE - used item so need to change this
			itemsOnCooldown = true;
			CurrentItemClock->restart();
		}
	}
}


void PlayerCharacter::playerShield()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { //if left shift is pressed
		if(itemStatuses.aegisShieldAquired == true){ // allow shielding if the player has the aegis shield
			if (shieldEnergy > 0) {
				//turn both shielding and playerinvincible to true so that we get the invincible effect but not the timer for the invincibility 
				shielding = true;
				playerInvincible = true;
				shieldCircle.setFillColor(sf::Color(175, 238, 238, 180)); // set the shield circle colour to be  light blue with a low aplha so it's somewhat see through
				//remove some shield energy
				shieldEnergy = shieldEnergy - 0.5f;
			}
		}
	}
	else if (playerHit == false) { //make sure player hit is false before changing invincibility. this is to make sure we don't remove the player hit invincibility
		playerInvincible = false;
		shieldCircle.setFillColor(sf::Color(0, 0, 100, 0)); //set completely see through so we can't see it (inactive)
	}
}
