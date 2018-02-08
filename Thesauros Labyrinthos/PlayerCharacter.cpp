#include "PlayerCharacter.h"
#include "textureLoader.h"
PlayerCharacter::PlayerCharacter(int x, int y)
{
	xPosition = x;
	yPosition = y;
	createSFML();
	//createCollisionBox(myWorld);
	torch = new TorchLight(xPosition,yPosition);
	updateclock = new sf::Clock;
	invincibilityClock = new sf::Clock;
}

void PlayerCharacter::createSFML()
{
	rectangle.setSize(sf::Vector2f(6.f, 6.f));
	rectangle.setOrigin(sf::Vector2f(3.f, 3.f));
	rectangle.setFillColor(sf::Color::Green); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(xPosition, yPosition); // set the postion of the rectangle to be the position passed
	//rectangle.setTextureRect(25,25)
	//rectangle.setTexture(&texture, false);
}

void PlayerCharacter::createCollisionBox(b2World &myWorld)
{

	//if (Box2dCreated == false) { // only do this if box2d hasn't been created. Player isn't deleted with level, and easier to do this rather than keep


		//bodyDef
		BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
		BodyDef.position.Set(xPosition / scale + 0.169, yPosition / scale + 0.169); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
		BodyDef.angle = 0;
		BodyDef.fixedRotation = true; // prevent rotation
		BodyDef.userData = this;
		//	BodyDef.userData = "playerCharacter"; // set the Userdata so that we can check what is colliding
		dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

		//box2dShape
		Shape.SetAsBox(3.f / scale, 3.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

		//create the fixture
		FixtureDef.shape = &Shape;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.0f;
		FixtureDef.filter.categoryBits = PLAYER; // set the category to be player
		FixtureDef.filter.maskBits = ENEMY | WALL; //set to collide with walls and enemies
		dynamicBody->CreateFixture(&FixtureDef);

		Box2dCreated = true; // set to true so that this doesn't happen again
	//}
}

string PlayerCharacter::getName()
{
	return "PlayerCharacter";
}

void PlayerCharacter::update(float dt)
{
	
	//if the player touched an enemy, call the collide with enemy function
	if (m_contactingEnemy == true) {
		collidingWithEnemy();
		m_contactingEnemy = false; // stop contacting enemy 
	}
	//if the player is invincible
	if (playerInvincible == true) {
		InvincibleTimer(); // call the invincibility function
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
		playerHealth--;//reduce playerHealth by 1
		
	}
	//move player left by reducing the xposition and setting the posiiton of the rectangle again.
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//xPosition -= 5;
		//dynamicBody->ApplyForce(b2Vec2(-5, 0), b2Vec2(0, 0), true);
		dynamicBody->SetLinearVelocity(b2Vec2(-5, -1));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//xPosition += 5;
		//dynamicBody->ApplyForce(b2Vec2(5, 0), b2Vec2(0, 0), true);
		dynamicBody->SetLinearVelocity(b2Vec2(5 , -1));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//yPosition += 5;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//yPosition -= 5;
		if (canJump == true) { // make sure we can jump first
			dynamicBody->ApplyLinearImpulse(b2Vec2(0, -4), b2Vec2(0, 0), true); // apply an impulse to propel player upard as a jump
		}
	}
	else {
		dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // if no buttons are being pressed, no velocity
	}
		
	
	
	//rectangle.setPosition(xPosition, yPosition);
	//update the xPOsition and Yposiition so that it can still be sued for the torch
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;
	//update the rectangle position to be the new position if there is one
	rectangle.setPosition(dynamicBody->GetPosition().x * scale, dynamicBody->GetPosition().y * scale);
	//rectangle.setRotation((180 / b2_pi) * dynamicBody->GetAngle()); // sets the rectangle's angle to follow the body so that the graphics and collision box sync up. have to times the angle by (180 / b2_pi) because box2d use radians and we have to change that to degrees for sfml.
	
	//limit how frequently the torch fuel will countdown based on time 
	if (updateclock->getElapsedTime().asSeconds() > 1) {
		torchCountdown(); // call the torch countdown to remove some fuel from the player
		updateclock->restart(); // restart the timer so that we can check when enough time has passed again
	}
	//call the torch update so that it updates along with the player
	torch->update(xPosition,yPosition,maxTorchFuel,currentTorchFuel);

	

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
		dynamicBody->ApplyLinearImpulse(b2Vec2(5, 0), b2Vec2(0, 0), true); //apply an impulse to player, to knock them back
		playerInvincible = true; // set the player invinicble so can't keep taking damage
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
	}
}

